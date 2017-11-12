#include "../../library.h"
#include "../engineUtil/engineUtil.h"
#include "./engineLayout02.h"
#include "../../game.h"

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

void engineLayout02ViewUtilPositionSetHorizontalCentering(struct engineLayout02View *this){
	if(localGlobal.isModeDraw){return;}
	this->position.style.horizontalCentering.isActive = true;
	localGlobal.generationCount++;
}

void engineLayout02ViewUtilPositionSetVerticalCentering(struct engineLayout02View *this){
	if(localGlobal.isModeDraw){return;}
	this->position.style.verticalCentering.isActive = true;
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
void engineLayout02ViewUtilPositionUnsetHorizontalCentering(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.horizontalCentering.isActive = false; localGlobal.generationCount++;}
void engineLayout02ViewUtilPositionUnsetVerticalCentering(struct engineLayout02View *this){if(localGlobal.isModeDraw){return;} this->position.style.verticalCentering.isActive = false; localGlobal.generationCount++;}

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
	if(this->children.parent != NULL){
		parentX = engineLayout02ViewUtilPositionGetX(this->children.parent);
		parentY = engineLayout02ViewUtilPositionGetY(this->children.parent);
		parentW = engineLayout02ViewUtilPositionGetW(this->children.parent);
		parentH = engineLayout02ViewUtilPositionGetH(this->children.parent);
		double paddingLt = (this->children.parent->position.style.paddingLeft.isActive ? this->children.parent->position.style.paddingLeft.value : 0);
		double paddingRt = (this->children.parent->position.style.paddingRight.isActive ? this->children.parent->position.style.paddingRight.value : 0);
		double paddingTp = (this->children.parent->position.style.paddingTop.isActive ? this->children.parent->position.style.paddingTop.value : 0);
		double paddingBm = (this->children.parent->position.style.paddingBottom.isActive ? this->children.parent->position.style.paddingBottom.value : 0);
		parentX += paddingLt;
		parentY += paddingTp;
		parentW -= paddingLt + paddingRt;
		parentH -= paddingTp + paddingBm;
	}
	double marginLt = (this->position.style.marginLeft.isActive ? this->position.style.marginLeft.value : 0);
	double marginRt = (this->position.style.marginRight.isActive ? this->position.style.marginRight.value : 0);
	double marginTp = (this->position.style.marginTop.isActive ? this->position.style.marginTop.value : 0);
	double marginBm = (this->position.style.marginBottom.isActive ? this->position.style.marginBottom.value : 0);
	parentX += marginLt;
	parentY += marginTp;
	parentW -= marginLt + marginRt;
	parentH -= marginTp + marginBm;

	// 自要素レイアウト計算 水平軸
	bool isLt = this->position.style.left.isActive;
	bool isRt = this->position.style.right.isActive;
	bool isWh = this->position.style.width.isActive;
	double valLt = this->position.style.left.value;
	double valRt = this->position.style.right.value;
	double valWh = this->position.style.width.value;
	if(this->position.style.horizontalCentering.isActive){
		this->position.layout.w = (isWh ? valWh : ((isLt && isRt) ? (valLt + valRt) : 0));
		this->position.layout.x = parentX + parentW * 0.5 - (isLt ? valLt : (isRt ? (this->position.layout.w - isRt) : (this->position.layout.w * 0.5)));
	}else{
		this->position.layout.w = (isWh ? valWh : ((isLt && isRt && parentW > valLt + valRt) ? (parentW - valLt - valRt) : 0));
		this->position.layout.x = parentX + (isLt ? valLt : (isRt ? (parentW - valRt - this->position.layout.w) : 0));
	}

	// 自要素レイアウト計算 垂直軸
	bool isTp = this->position.style.top.isActive;
	bool isBm = this->position.style.bottom.isActive;
	bool isHt = this->position.style.height.isActive;
	double valTp = this->position.style.top.value;
	double valBm = this->position.style.bottom.value;
	double valHt = this->position.style.height.value;
	if(this->position.style.verticalCentering.isActive){
		this->position.layout.h = (isHt ? valHt : ((isTp && isBm) ? (valTp + valBm) : 0));
		this->position.layout.y = parentY + parentH * 0.5 - (isTp ? valTp : (isBm ? (this->position.layout.h - isBm) : (this->position.layout.h * 0.5)));
	}else{
		this->position.layout.h = (isHt ? valHt : ((isTp && isBm && parentH > valTp + valBm) ? (parentH - valTp - valBm) : 0));
		this->position.layout.y = parentY + (isTp ? valTp : (isBm ? (parentH - valBm - this->position.layout.h) : 0));
	}
}

double engineLayout02ViewUtilPositionGetX(struct engineLayout02View *this){calcLayout(this); return this->position.layout.x;}
double engineLayout02ViewUtilPositionGetY(struct engineLayout02View *this){calcLayout(this); return this->position.layout.y;}
double engineLayout02ViewUtilPositionGetW(struct engineLayout02View *this){calcLayout(this); return this->position.layout.w;}
double engineLayout02ViewUtilPositionGetH(struct engineLayout02View *this){calcLayout(this); return this->position.layout.h;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

