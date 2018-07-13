#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	bool isModeDraw;
	struct{
		int x;
		int y;
		int w;
		int h;
	} screen;
	int generationCount;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体位置関係 計算モード設定
void engineLayoutViewUtilPositionModeSetCalc(){
	if(!localGlobal.isModeDraw){return;}
	localGlobal.isModeDraw = false;
}

// 表示要素構造体位置関係 描画モード設定
void engineLayoutViewUtilPositionModeSetDraw(){
	if(localGlobal.isModeDraw){return;}
	localGlobal.isModeDraw = true;

	// スクリーンサイズ更新確認
	int screenX = 0;
	int screenY = 0;
	int screenW = global.screen.w;
	int screenH = global.screen.h;
	bool shouldLayout = false;
	shouldLayout = shouldLayout || (localGlobal.screen.x != screenX);
	shouldLayout = shouldLayout || (localGlobal.screen.y != screenY);
	shouldLayout = shouldLayout || (localGlobal.screen.w != screenW);
	shouldLayout = shouldLayout || (localGlobal.screen.h != screenH);
	if(shouldLayout){
		localGlobal.screen.x = screenX;
		localGlobal.screen.y = screenY;
		localGlobal.screen.w = screenW;
		localGlobal.screen.h = screenH;
		localGlobal.generationCount++;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// レイアウト計算
static void calcLayout(struct engineLayoutView *this){
	// 情報更新確認 
	bool isInitialLayout = (this->position.layout.generationCount == 0);
	bool isDrawUpdate = (localGlobal.isModeDraw && this->position.layout.generationCount != localGlobal.generationCount);
	if(!isInitialLayout && !isDrawUpdate){return;}
	this->position.layout.generationCount = localGlobal.generationCount;

	// 親要素レイアウト取得
	double parentX = localGlobal.screen.x;
	double parentY = localGlobal.screen.y;
	double parentW = localGlobal.screen.w;
	double parentH = localGlobal.screen.h;
	struct engineMathMatrix44 *parentMatrix = NULL;
	double currentX = parentX;
	double currentY = parentY;
	double currentW = parentW;
	double currentH = parentH;
	if(this->family.parent != NULL){
		calcLayout(this->family.parent);
		parentMatrix = this->family.parent->position.layout.transform.isActive ? &this->family.parent->position.layout.transform.matrix : NULL;
		currentX = parentX = this->family.parent->position.layout.x;
		currentY = parentY = this->family.parent->position.layout.y;
		currentW = parentW = this->family.parent->position.layout.w;
		currentH = parentH = this->family.parent->position.layout.h;
		double paddingLt = (this->family.parent->position.style.paddingLeft.isActive ? this->family.parent->position.style.paddingLeft.value : 0);
		double paddingRt = (this->family.parent->position.style.paddingRight.isActive ? this->family.parent->position.style.paddingRight.value : 0);
		double paddingTp = (this->family.parent->position.style.paddingTop.isActive ? this->family.parent->position.style.paddingTop.value : 0);
		double paddingBm = (this->family.parent->position.style.paddingBottom.isActive ? this->family.parent->position.style.paddingBottom.value : 0);
		currentX += paddingLt;
		currentY += paddingTp;
		currentW -= paddingLt + paddingRt;
		currentH -= paddingTp + paddingBm;
	}
	double marginLt = (this->position.style.marginLeft.isActive ? this->position.style.marginLeft.value : 0);
	double marginRt = (this->position.style.marginRight.isActive ? this->position.style.marginRight.value : 0);
	double marginTp = (this->position.style.marginTop.isActive ? this->position.style.marginTop.value : 0);
	double marginBm = (this->position.style.marginBottom.isActive ? this->position.style.marginBottom.value : 0);
	currentX += marginLt;
	currentY += marginTp;
	currentW -= marginLt + marginRt;
	currentH -= marginTp + marginBm;

	// 自要素レイアウト計算 水平軸
	// isLt isRt isWh
	//   x    x    x 親サイズに合わせる
	//   x    x    o 中央揃え サイズ固定
	//   x    o    x 右揃え サイズ0
	//   x    o    o 右揃え サイズ固定
	//   o    x    x 左揃え サイズ0
	//   o    x    o 左揃え サイズ固定
	//   o    o    x 左右固定 サイズクリッピング
	//   o    o    o 左右固定 サイズクリッピング 幅無視
	bool isLt = this->position.style.left.isActive;
	bool isRt = this->position.style.right.isActive;
	bool isWh = this->position.style.width.isActive;
	double valLt = this->position.style.left.value;
	double valRt = this->position.style.right.value;
	double valWh = this->position.style.width.isRatio ? (currentW * this->position.style.width.value) : this->position.style.width.value;
	if(isLt && isRt && this->position.style.clampHorizontal.isActive){
		// 左右固定 サイズクリッピング
		double fixW = currentW - valLt - valRt;
		double fixX = currentX + valLt;
		double min = this->position.style.clampHorizontal.min;
		double max = this->position.style.clampHorizontal.max;
		int align = this->position.style.clampHorizontal.align;
		this->position.layout.w = fixW;
		if(min > 0 && this->position.layout.w < min){this->position.layout.w = min;}
		if(max > 0 && max > min && this->position.layout.w > max){this->position.layout.w = max;}
		this->position.layout.x = fixX + (fixW - this->position.layout.w) * ((align > 0) ? 0.0 : (align == 0) ? 0.5 : 1.0);
	}else if(isLt && isRt){
		// 左右固定
		this->position.layout.w = currentW - valLt - valRt;
		this->position.layout.x = currentX + valLt;
	}else if(isLt){
		// 左揃え
		this->position.layout.w = isWh ? valWh : 0;
		this->position.layout.x = currentX + valLt;
	}else if(isRt){
		// 右揃え
		this->position.layout.w = isWh ? valWh : 0;
		this->position.layout.x = currentX + currentW - valRt - this->position.layout.w;
	}else if(isWh){
		// 中央揃え
		this->position.layout.w = valWh;
		this->position.layout.x = currentX + (currentW - this->position.layout.w) * 0.5;
	}else{
		// 親サイズに合わせる
		this->position.layout.w = currentW;
		this->position.layout.x = currentX;
	}

	// 自要素レイアウト計算 垂直軸
	// isTp isBm isHt
	//   x    x    x 親サイズに合わせる
	//   x    x    o 中央揃え サイズ固定
	//   x    o    x 下揃え サイズ0
	//   x    o    o 下揃え サイズ固定
	//   o    x    x 上揃え サイズ0
	//   o    x    o 上揃え サイズ固定
	//   o    o    x 上下固定 サイズクリッピング
	//   o    o    o 上下固定 サイズクリッピング 高さ無視
	bool isTp = this->position.style.top.isActive;
	bool isBm = this->position.style.bottom.isActive;
	bool isHt = this->position.style.height.isActive;
	double valTp = this->position.style.top.value;
	double valBm = this->position.style.bottom.value;
	double valHt = this->position.style.height.isRatio ? (currentH * this->position.style.height.value) : this->position.style.height.value;
	if(isTp && isBm && this->position.style.clampVertical.isActive){
		// 左右固定 サイズクリッピング
		double fixH = currentH - valTp - valBm;
		double fixY = currentY + valTp;
		double min = this->position.style.clampVertical.min;
		double max = this->position.style.clampVertical.max;
		int align = this->position.style.clampVertical.align;
		this->position.layout.h = fixH;
		if(min > 0 && this->position.layout.h < min){this->position.layout.h = min;}
		if(max > 0 && max > min && this->position.layout.h > max){this->position.layout.h = max;}
		this->position.layout.y = fixY + (fixH - this->position.layout.h) * ((align > 0) ? 0.0 : (align == 0) ? 0.5 : 1.0);
	}else if(isTp && isBm){
		// 左右固定
		this->position.layout.h = currentH - valTp - valBm;
		this->position.layout.y = currentY + valTp;
	}else if(isTp){
		// 左揃え
		this->position.layout.h = isHt ? valHt : 0;
		this->position.layout.y = currentY + valTp;
	}else if(isBm){
		// 右揃え
		this->position.layout.h = isHt ? valHt : 0;
		this->position.layout.y = currentY + currentH - valBm - this->position.layout.h;
	}else if(isHt){
		// 中央揃え
		this->position.layout.h = valHt;
		this->position.layout.y = currentY + (currentH - this->position.layout.h) * 0.5;
	}else{
		// 親サイズに合わせる
		this->position.layout.h = currentH;
		this->position.layout.y = currentY;
	}

	// 行列変形計算
	if(parentMatrix != NULL || this->position.style.transform.isActive){
		struct engineMathMatrix44 *layoutMatrix = &this->position.layout.transform.matrix;
		// 親要素行列取得
		if(parentMatrix != NULL){
			engineMathMat4Copy(layoutMatrix, parentMatrix);
		}else{
			engineMathMat4Identity(layoutMatrix);
			engineMathMat4Translate(layoutMatrix, parentX, parentY, 0.0);
		}
		// 自要素行列計算
		double x0 = this->position.layout.w * this->position.style.transform.originX;
		double y0 = this->position.layout.h * this->position.style.transform.originY;
		double x1 = this->position.layout.x - parentX;
		double y1 = this->position.layout.y - parentY;
		engineMathMat4Translate(layoutMatrix, x0 + x1, y0 + y1, 0.0);
		if(this->position.style.transform.isActive){engineMathMat4Multiply(layoutMatrix, layoutMatrix, &this->position.style.transform.matrix);}
		engineMathMat4Translate(layoutMatrix, -x0, -y0, 0.0);
		// 行列変形フラグ
		this->position.layout.transform.isActive = true;
	}else{
		this->position.layout.transform.isActive = false;
	}
}

// レイアウトを明示的に計算
void engineLayoutViewUtilPositionCalcLayoutAll(struct engineLayoutView *this){
	// レイアウトを計算する
	calcLayout(this);
	// 子要素に対して処理を適用する
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		engineLayoutViewUtilPositionCalcLayoutAll(temp);
		temp = temp->family.next;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体位置関係 初期化
void engineLayoutViewUtilPositionInit(struct engineLayoutView *this){
	// 内部判定関数の設定
	this->position.isInner = engineLayoutViewUtilPositionTransformIsInner;

	if(localGlobal.isModeDraw){return;}
	localGlobal.generationCount++;
}

// 表示要素構造体位置関係 破棄
void engineLayoutViewUtilPositionDispose(struct engineLayoutView *this){
	if(localGlobal.isModeDraw){return;}
	localGlobal.generationCount++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 レイアウト設定

void engineLayoutViewUtilPositionSetLeft(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.right.isActive && this->position.style.width.isActive){return;}
	this->position.style.left.isActive = true;
	this->position.style.left.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetRight(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.left.isActive && this->position.style.width.isActive){return;}
	this->position.style.right.isActive = true;
	this->position.style.right.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetTop(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.bottom.isActive && this->position.style.height.isActive){return;}
	this->position.style.top.isActive = true;
	this->position.style.top.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetBottom(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.top.isActive && this->position.style.height.isActive){return;}
	this->position.style.bottom.isActive = true;
	this->position.style.bottom.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetWidthValue(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.left.isActive && this->position.style.right.isActive){return;}
	this->position.style.width.isActive = true;
	this->position.style.width.value = value;
	this->position.style.width.isRatio = false;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetHeightValue(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.top.isActive && this->position.style.bottom.isActive){return;}
	this->position.style.height.isActive = true;
	this->position.style.height.value = value;
	this->position.style.height.isRatio = false;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetWidthRatio(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.left.isActive && this->position.style.right.isActive){return;}
	this->position.style.width.isActive = true;
	this->position.style.width.value = value;
	this->position.style.width.isRatio = true;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetHeightRatio(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.top.isActive && this->position.style.bottom.isActive){return;}
	this->position.style.height.isActive = true;
	this->position.style.height.value = value;
	this->position.style.height.isRatio = true;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetMarginLeft(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginLeft.isActive = true;
	this->position.style.marginLeft.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetMarginRight(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginRight.isActive = true;
	this->position.style.marginRight.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetMarginTop(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginTop.isActive = true;
	this->position.style.marginTop.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetMarginBottom(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginBottom.isActive = true;
	this->position.style.marginBottom.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetPaddingLeft(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingLeft.isActive = true;
	this->position.style.paddingLeft.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetPaddingRight(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingRight.isActive = true;
	this->position.style.paddingRight.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetPaddingTop(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingTop.isActive = true;
	this->position.style.paddingTop.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetPaddingBottom(struct engineLayoutView *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingBottom.isActive = true;
	this->position.style.paddingBottom.value = value;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetClampHorizontal(struct engineLayoutView *this, double min, double max, int align){
	if(localGlobal.isModeDraw){return;}
	this->position.style.clampHorizontal.isActive = true;
	this->position.style.clampHorizontal.min = min;
	this->position.style.clampHorizontal.max = max;
	this->position.style.clampHorizontal.align = align;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetClampVertical(struct engineLayoutView *this, double min, double max, int align){
	if(localGlobal.isModeDraw){return;}
	this->position.style.clampVertical.isActive = true;
	this->position.style.clampVertical.min = min;
	this->position.style.clampVertical.max = max;
	this->position.style.clampVertical.align = align;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetTransformOrigin(struct engineLayoutView *this, double originX, double originY){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		engineMathMat4Identity(&this->position.style.transform.matrix);
	}
	this->position.style.transform.originX = originX;
	this->position.style.transform.originY = originY;
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetTransformMatrix(struct engineLayoutView *this, struct engineMathMatrix44 *matrix){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		this->position.style.transform.originX = 0.5;
		this->position.style.transform.originY = 0.5;
	}
	engineMathMat4Copy(&this->position.style.transform.matrix, matrix);
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetTransformScale(struct engineLayoutView *this, double x, double y){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		this->position.style.transform.originX = 0.5;
		this->position.style.transform.originY = 0.5;
		engineMathMat4Identity(&this->position.style.transform.matrix);
	}
	engineMathMat4Scale(&this->position.style.transform.matrix, x, y, 1.0);
	localGlobal.generationCount++;
}

void engineLayoutViewUtilPositionSetTransformRotate(struct engineLayoutView *this, double rad){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		this->position.style.transform.originX = 0.5;
		this->position.style.transform.originY = 0.5;
		engineMathMat4Identity(&this->position.style.transform.matrix);
	}
	engineMathMat4RotateZ(&this->position.style.transform.matrix, rad);
	localGlobal.generationCount++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 レイアウト解除

void engineLayoutViewUtilPositionUnsetLeft(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.left.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetRight(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.right.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetTop(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.top.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetBottom(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.bottom.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetWidth(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.width.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetHeight(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.height.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetMarginLeft(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.marginLeft.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetMarginRight(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.marginRight.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetMarginTop(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.marginTop.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetMarginBottom(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.marginBottom.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetPaddingLeft(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingLeft.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetPaddingRight(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingRight.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetPaddingTop(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingTop.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetPaddingBottom(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingBottom.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetClampHorizontal(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.clampHorizontal.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetClampVertical(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.clampVertical.isActive = false; localGlobal.generationCount++;}
void engineLayoutViewUtilPositionUnsetTransform(struct engineLayoutView *this){if(localGlobal.isModeDraw){return;} this->position.style.transform.isActive = false; localGlobal.generationCount++;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 レイアウト取得

double engineLayoutViewUtilPositionGetX(struct engineLayoutView *this){calcLayout(this); return this->position.layout.x;}
double engineLayoutViewUtilPositionGetY(struct engineLayoutView *this){calcLayout(this); return this->position.layout.y;}
double engineLayoutViewUtilPositionGetW(struct engineLayoutView *this){calcLayout(this); return this->position.layout.w;}
double engineLayoutViewUtilPositionGetH(struct engineLayoutView *this){calcLayout(this); return this->position.layout.h;}

// スクリーン座標系の行列をローカル座標系に変換
void engineLayoutViewUtilPositionTransformCalcMatrix(struct engineLayoutView *this, struct engineMathMatrix44 *dstMat, struct engineMathMatrix44 *srcMat){
	calcLayout(this);
	if(this->position.layout.transform.isActive){
		engineMathMat4Multiply(dstMat, srcMat, &this->position.layout.transform.matrix);
	}else{
		double x = this->position.layout.x;
		double y = this->position.layout.y;
		engineMathMat4Copy(dstMat, srcMat);
		engineMathMat4Translate(dstMat, x, y, 0.0);
	}
}

// スクリーン座標系の点をローカル座標系に変換
void engineLayoutViewUtilPositionTransformCalcInvert(struct engineLayoutView *this, struct engineMathVector3 *position){
	calcLayout(this);
	if(this->position.layout.transform.isActive){
		if(this->position.layout.generationCountInvert != this->position.layout.generationCount){
			this->position.layout.generationCountInvert = this->position.layout.generationCount;
			engineMathMat4Copy(&this->position.layout.transform.invert, &this->position.layout.transform.matrix);
			engineMathMat4Invert(&this->position.layout.transform.invert);
		}
		engineMathVec3MultiplyMat4(position, position, &this->position.layout.transform.invert);
	}else{
		position->x -= this->position.layout.x;
		position->y -= this->position.layout.y;
	}
}


bool engineLayoutViewUtilPositionTransformIsInner(struct engineLayoutView *this, double x, double y){
	// ローカル座標変換
	struct engineMathVector3 tempVec1;
	engineMathVec3Set(&tempVec1, x, y, 0);
	engineLayoutViewUtilPositionTransformCalcInvert(this, &tempVec1);
	// 矩形内部判定
	return (0 < tempVec1.x && tempVec1.x < this->position.layout.w && 0 < tempVec1.y && tempVec1.y < this->position.layout.h);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 設定ショートカット

void engineLayoutViewUtilPositionSetLtRtTpBm(struct engineLayoutView *this, double lt, double rt, double tp, double bm){
	engineLayoutViewUtilPositionSetLeft(this, lt);
	engineLayoutViewUtilPositionSetRight(this, rt);
	engineLayoutViewUtilPositionSetTop(this, tp);
	engineLayoutViewUtilPositionSetBottom(this, bm);
}

void engineLayoutViewUtilPositionSetLtTpWtHt(struct engineLayoutView *this, double lt, double tp, double wt, double ht){
	engineLayoutViewUtilPositionSetLeft(this, lt);
	engineLayoutViewUtilPositionSetTop(this, tp);
	engineLayoutViewUtilPositionSetWidthValue(this, wt);
	engineLayoutViewUtilPositionSetHeightValue(this, ht);
}

void engineLayoutViewUtilPositionSetMargin(struct engineLayoutView *this, double margin){
	engineLayoutViewUtilPositionSetMarginLeft(this, margin);
	engineLayoutViewUtilPositionSetMarginRight(this, margin);
	engineLayoutViewUtilPositionSetMarginTop(this, margin);
	engineLayoutViewUtilPositionSetMarginBottom(this, margin);
}

void engineLayoutViewUtilPositionSetMarginHorizontal(struct engineLayoutView *this, double margin){
	engineLayoutViewUtilPositionSetMarginLeft(this, margin);
	engineLayoutViewUtilPositionSetMarginRight(this, margin);
}

void engineLayoutViewUtilPositionSetMarginVertical(struct engineLayoutView *this, double margin){
	engineLayoutViewUtilPositionSetMarginTop(this, margin);
	engineLayoutViewUtilPositionSetMarginBottom(this, margin);
}

void engineLayoutViewUtilPositionSetPadding(struct engineLayoutView *this, double padding){
	engineLayoutViewUtilPositionSetPaddingLeft(this, padding);
	engineLayoutViewUtilPositionSetPaddingRight(this, padding);
	engineLayoutViewUtilPositionSetPaddingTop(this, padding);
	engineLayoutViewUtilPositionSetPaddingBottom(this, padding);
}

void engineLayoutViewUtilPositionSetPaddingHorizontal(struct engineLayoutView *this, double padding){
	engineLayoutViewUtilPositionSetPaddingLeft(this, padding);
	engineLayoutViewUtilPositionSetPaddingRight(this, padding);
}

void engineLayoutViewUtilPositionSetPaddingVertical(struct engineLayoutView *this, double padding){
	engineLayoutViewUtilPositionSetPaddingTop(this, padding);
	engineLayoutViewUtilPositionSetPaddingBottom(this, padding);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

