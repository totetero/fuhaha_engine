#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout02/engineLayout02.h"

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
void engineLayout02ViewUtilPositionModeSetCalc(){
	if(!localGlobal.isModeDraw){return;}
	localGlobal.isModeDraw = false;
}

// 表示要素構造体位置関係 描画モード設定
void engineLayout02ViewUtilPositionModeSetDraw(){
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

// 表示要素構造体位置関係 初期化
void engineLayout02ViewUtilPositionInit(struct engineLayout02View *this){
	if(localGlobal.isModeDraw){return;}
	localGlobal.generationCount++;
}

// 表示要素構造体位置関係 破棄
void engineLayout02ViewUtilPositionDispose(struct engineLayout02View *this){
	if(localGlobal.isModeDraw){return;}
	localGlobal.generationCount++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 レイアウト設定

void engineLayout02ViewUtilPositionSetLeft(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.right.isActive && this->position.style.width.isActive){return;}
	this->position.style.left.isActive = true;
	this->position.style.left.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetRight(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.left.isActive && this->position.style.width.isActive){return;}
	this->position.style.right.isActive = true;
	this->position.style.right.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetTop(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.bottom.isActive && this->position.style.height.isActive){return;}
	this->position.style.top.isActive = true;
	this->position.style.top.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetBottom(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.top.isActive && this->position.style.height.isActive){return;}
	this->position.style.bottom.isActive = true;
	this->position.style.bottom.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetWidth(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.left.isActive && this->position.style.right.isActive){return;}
	this->position.style.width.isActive = true;
	this->position.style.width.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetHeight(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	if(this->position.style.top.isActive && this->position.style.bottom.isActive){return;}
	this->position.style.height.isActive = true;
	this->position.style.height.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetMarginLeft(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginLeft.isActive = true;
	this->position.style.marginLeft.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetMarginRight(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginRight.isActive = true;
	this->position.style.marginRight.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetMarginTop(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginTop.isActive = true;
	this->position.style.marginTop.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetMarginBottom(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.marginBottom.isActive = true;
	this->position.style.marginBottom.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetPaddingLeft(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingLeft.isActive = true;
	this->position.style.paddingLeft.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetPaddingRight(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingRight.isActive = true;
	this->position.style.paddingRight.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetPaddingTop(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingTop.isActive = true;
	this->position.style.paddingTop.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetPaddingBottom(struct engineLayout02View *this, double value){
	if(localGlobal.isModeDraw){return;}
	this->position.style.paddingBottom.isActive = true;
	this->position.style.paddingBottom.value = value;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetCenteringHorizontal(struct engineLayout02View *this){
	if(localGlobal.isModeDraw){return;}
	this->position.style.centeringHorizontal.isActive = true;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetCenteringVertical(struct engineLayout02View *this){
	if(localGlobal.isModeDraw){return;}
	this->position.style.centeringVertical.isActive = true;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetTransformOrigin(struct engineLayout02View *this, double originX, double originY){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		engineMathMat4Identity(&this->position.style.transform.matrix);
	}
	this->position.style.transform.originX = originX;
	this->position.style.transform.originY = originY;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetTransformMatrix(struct engineLayout02View *this, struct engineMathMatrix44 *matrix){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		this->position.style.transform.originX = 0.5;
		this->position.style.transform.originY = 0.5;
	}
	engineMathMat4Copy(&this->position.style.transform.matrix, matrix);
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetTransformScale(struct engineLayout02View *this, double x, double y, double z){
	if(localGlobal.isModeDraw){return;}
	if(!this->position.style.transform.isActive){
		this->position.style.transform.isActive = true;
		this->position.style.transform.originX = 0.5;
		this->position.style.transform.originY = 0.5;
		engineMathMat4Identity(&this->position.style.transform.matrix);
	}
	engineMathMat4Scale(&this->position.style.transform.matrix, x, y, z);
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetTransformRotate(struct engineLayout02View *this, double rad){
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

void engineLayout02ViewUtilPositionUnsetLeft(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.left.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetRight(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.right.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetTop(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.top.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetBottom(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.bottom.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetWidth(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.width.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetHeight(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.height.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetMarginLeft(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.marginLeft.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetMarginRight(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.marginRight.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetMarginTop(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.marginTop.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetMarginBottom(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.marginBottom.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetPaddingLeft(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingLeft.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetPaddingRight(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingRight.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetPaddingTop(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingTop.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetPaddingBottom(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.paddingBottom.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetCenteringHorizontal(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.centeringHorizontal.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetCenteringVertical(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.centeringVertical.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetTransform(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.transform.isActive = false; localGlobal.generationCount++;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 レイアウト取得

static void calcLayout(struct engineLayout02View *this){
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
	if(this->family.parent != NULL && this->family.parent != engineLayout02ViewUtilFamilyRootGet()){
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
	bool isLt = this->position.style.left.isActive;
	bool isRt = this->position.style.right.isActive;
	bool isWh = this->position.style.width.isActive;
	double valLt = this->position.style.left.value;
	double valRt = this->position.style.right.value;
	double valWh = this->position.style.width.value;
	if(this->position.style.centeringHorizontal.isActive){
		this->position.layout.w = (isWh ? valWh : ((isLt && isRt) ? (valLt + valRt) : 0));
		this->position.layout.x = currentX + currentW * 0.5 - (isLt ? valLt : (isRt ? (this->position.layout.w - isRt) : (this->position.layout.w * 0.5)));
	}else{
		this->position.layout.w = (isWh ? valWh : ((isLt && isRt && currentW > valLt + valRt) ? (currentW - valLt - valRt) : 0));
		this->position.layout.x = currentX + (isLt ? valLt : (isRt ? (currentW - valRt - this->position.layout.w) : 0));
	}

	// 自要素レイアウト計算 垂直軸
	bool isTp = this->position.style.top.isActive;
	bool isBm = this->position.style.bottom.isActive;
	bool isHt = this->position.style.height.isActive;
	double valTp = this->position.style.top.value;
	double valBm = this->position.style.bottom.value;
	double valHt = this->position.style.height.value;
	if(this->position.style.centeringVertical.isActive){
		this->position.layout.h = (isHt ? valHt : ((isTp && isBm) ? (valTp + valBm) : 0));
		this->position.layout.y = currentY + currentH * 0.5 - (isTp ? valTp : (isBm ? (this->position.layout.h - isBm) : (this->position.layout.h * 0.5)));
	}else{
		this->position.layout.h = (isHt ? valHt : ((isTp && isBm && currentH > valTp + valBm) ? (currentH - valTp - valBm) : 0));
		this->position.layout.y = currentY + (isTp ? valTp : (isBm ? (currentH - valBm - this->position.layout.h) : 0));
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
		// 変形座標計算
		this->position.layout.transform.point[0].x = 0;
		this->position.layout.transform.point[0].y = 0;
		this->position.layout.transform.point[0].z = 0;
		this->position.layout.transform.point[1].x = 0;
		this->position.layout.transform.point[1].y = this->position.layout.h;
		this->position.layout.transform.point[1].z = 0;
		this->position.layout.transform.point[2].x = this->position.layout.w;
		this->position.layout.transform.point[2].y = this->position.layout.h;
		this->position.layout.transform.point[2].z = 0;
		this->position.layout.transform.point[3].x = this->position.layout.w;
		this->position.layout.transform.point[3].y = 0;
		this->position.layout.transform.point[3].z = 0;
		engineMathVec3MultiplyMat4(&this->position.layout.transform.point[0], &this->position.layout.transform.point[0], &this->position.layout.transform.matrix);
		engineMathVec3MultiplyMat4(&this->position.layout.transform.point[1], &this->position.layout.transform.point[1], &this->position.layout.transform.matrix);
		engineMathVec3MultiplyMat4(&this->position.layout.transform.point[2], &this->position.layout.transform.point[2], &this->position.layout.transform.matrix);
		engineMathVec3MultiplyMat4(&this->position.layout.transform.point[3], &this->position.layout.transform.point[3], &this->position.layout.transform.matrix);
		// 行列変形フラグ
		this->position.layout.transform.isActive = true;
	}else{
		this->position.layout.transform.isActive = false;
	}
}

double engineLayout02ViewUtilPositionGetX(struct engineLayout02View *this){calcLayout(this); return this->position.layout.x;}
double engineLayout02ViewUtilPositionGetY(struct engineLayout02View *this){calcLayout(this); return this->position.layout.y;}
double engineLayout02ViewUtilPositionGetW(struct engineLayout02View *this){calcLayout(this); return this->position.layout.w;}
double engineLayout02ViewUtilPositionGetH(struct engineLayout02View *this){calcLayout(this); return this->position.layout.h;}

void engineLayout02ViewUtilPositionTransformCalcMatrix(struct engineLayout02View *this, struct engineMathMatrix44 *dstMat, struct engineMathMatrix44 *srcMat){
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

bool engineLayout02ViewUtilPositionTransformIsInner(struct engineLayout02View *this, double x, double y){
	if(this->position.layout.transform.isActive){
		// すべての辺に対し対象点との外積が全て同一符号なら内部
		struct engineMathVector3 *v0 = &this->position.layout.transform.point[0];
		struct engineMathVector3 *v1 = &this->position.layout.transform.point[1];
		struct engineMathVector3 *v2 = &this->position.layout.transform.point[2];
		struct engineMathVector3 *v3 = &this->position.layout.transform.point[3];
		double crossZ01 = (x - v0->x) * (v1->y - v0->y) - (v1->x - v0->x) * (y - v0->y);
		double crossZ12 = (x - v1->x) * (v2->y - v1->y) - (v2->x - v1->x) * (y - v1->y);
		double crossZ23 = (x - v2->x) * (v3->y - v2->y) - (v3->x - v2->x) * (y - v2->y);
		double crossZ30 = (x - v3->x) * (v0->y - v3->y) - (v0->x - v3->x) * (y - v3->y);
		return (crossZ01 * crossZ12 > 0 && crossZ12 * crossZ23 > 0 && crossZ23 * crossZ30 > 0);
	}else{
		// 矩形内部判定
		double x0 = this->position.layout.x;
		double y0 = this->position.layout.y;
		double x1 = x0 + this->position.layout.w;
		double y1 = y0 + this->position.layout.h;
		return (x0 < x && x < x1 && y0 < y && y < y1);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 表示要素構造体 設定ショートカット

void engineLayout02ViewUtilPositionSetLtRtTpBm(struct engineLayout02View *this, double lt, double rt, double tp, double bm){
	engineLayout02ViewUtilPositionSetLeft(this, lt);
	engineLayout02ViewUtilPositionSetRight(this, rt);
	engineLayout02ViewUtilPositionSetTop(this, tp);
	engineLayout02ViewUtilPositionSetBottom(this, bm);
}

void engineLayout02ViewUtilPositionSetLtTpWtHt(struct engineLayout02View *this, double lt, double tp, double wt, double ht){
	engineLayout02ViewUtilPositionSetLeft(this, lt);
	engineLayout02ViewUtilPositionSetTop(this, tp);
	engineLayout02ViewUtilPositionSetWidth(this, wt);
	engineLayout02ViewUtilPositionSetHeight(this, ht);
}

void engineLayout02ViewUtilPositionSetMargin(struct engineLayout02View *this, double margin){
	engineLayout02ViewUtilPositionSetMarginLeft(this, margin);
	engineLayout02ViewUtilPositionSetMarginRight(this, margin);
	engineLayout02ViewUtilPositionSetMarginTop(this, margin);
	engineLayout02ViewUtilPositionSetMarginBottom(this, margin);
}

void engineLayout02ViewUtilPositionSetMarginHorizontal(struct engineLayout02View *this, double margin){
	engineLayout02ViewUtilPositionSetMarginLeft(this, margin);
	engineLayout02ViewUtilPositionSetMarginRight(this, margin);
}

void engineLayout02ViewUtilPositionSetMarginVertical(struct engineLayout02View *this, double margin){
	engineLayout02ViewUtilPositionSetMarginTop(this, margin);
	engineLayout02ViewUtilPositionSetMarginBottom(this, margin);
}

void engineLayout02ViewUtilPositionSetPadding(struct engineLayout02View *this, double padding){
	engineLayout02ViewUtilPositionSetPaddingLeft(this, padding);
	engineLayout02ViewUtilPositionSetPaddingRight(this, padding);
	engineLayout02ViewUtilPositionSetPaddingTop(this, padding);
	engineLayout02ViewUtilPositionSetPaddingBottom(this, padding);
}

void engineLayout02ViewUtilPositionSetPaddingHorizontal(struct engineLayout02View *this, double padding){
	engineLayout02ViewUtilPositionSetPaddingLeft(this, padding);
	engineLayout02ViewUtilPositionSetPaddingRight(this, padding);
}

void engineLayout02ViewUtilPositionSetPaddingVertical(struct engineLayout02View *this, double padding){
	engineLayout02ViewUtilPositionSetPaddingTop(this, padding);
	engineLayout02ViewUtilPositionSetPaddingBottom(this, padding);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

