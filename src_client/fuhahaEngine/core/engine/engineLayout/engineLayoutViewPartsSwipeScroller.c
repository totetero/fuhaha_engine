#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsSwipeScrollerImplement{
	struct engineLayoutViewPartsSwipeScroller super;
	struct{int x; int y;} posTemp;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsSwipeScrollerImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.interact.setting.isTouchable = true;

	// スクロール要素作成
	this->super.viewInner = engineLayoutViewCreate();
	this->super.viewBarX = engineLayoutViewCreate();
	this->super.viewBarY = engineLayoutViewCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewBarX);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewBarY);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewBarX);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewBarY);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsSwipeScrollerImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	return engineLayoutSwipeTouch(&this->super.swipe, (struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel);
}

// 計算
static void calc(struct engineLayoutViewPartsSwipeScrollerImplement *this){
	// 大きさ取得
	double innerW = this->super.inner.w;
	double innerH = this->super.inner.h;
	double outerW = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
	double outerH = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);

	// スクロール可否の確認
	this->super.super.interact.setting.isMoveableX = (innerW > outerW);
	this->super.super.interact.setting.isMoveableY = (innerH > outerH);
	if(!this->super.super.interact.setting.isMoveableX){innerW = outerW;}
	if(!this->super.super.interact.setting.isMoveableY){innerH = outerH;}

	// 領域外スクロールフラグ
	bool isBounce = true;
	// スクロール領域
	double minX = 0;
	double minY = 0;
	double maxX = innerW - outerW;
	double maxY = innerH - outerH;

	if(this->super.super.interact.status.isActive && this->super.super.interact.status.isMove){
		// スクロール中
		this->super.velocity.x = this->super.swipe.x - this->posTemp.x + this->super.velocity.x * 0.3;
		this->super.velocity.y = this->super.swipe.y - this->posTemp.y + this->super.velocity.y * 0.3;
		this->super.position.x += this->super.swipe.x - this->posTemp.x;
		this->super.position.y += this->super.swipe.y - this->posTemp.y;
	}else{
		if(engineMathAbs(this->super.velocity.x) > 0.01 || engineMathAbs(this->super.velocity.y) > 0.01){
			// 速度が生きてる
			this->super.velocity.x *= this->super.super.interact.status.isActive ? 0.5 : 0.9;
			this->super.velocity.y *= this->super.super.interact.status.isActive ? 0.5 : 0.9;
			this->super.position.x += this->super.velocity.x;
			this->super.position.y += this->super.velocity.y;
		}

		if(isBounce){
			// 領域外スクロール確認
			double posX0 = this->super.position.x;
			double posY0 = this->super.position.y;
			if(this->super.position.x > -minX){this->super.velocity.x *= 0.2; double posX1 = -minX; this->super.position.x = (engineMathAbs(posX1 - posX0) > 0.01) ? (posX0 + (posX1 - posX0) * 0.1) : posX1;}
			if(this->super.position.x < -maxX){this->super.velocity.x *= 0.2; double posX1 = -maxX; this->super.position.x = (engineMathAbs(posX1 - posX0) > 0.01) ? (posX0 + (posX1 - posX0) * 0.1) : posX1;}
			if(this->super.position.y > -minY){this->super.velocity.y *= 0.2; double posY1 = -minY; this->super.position.y = (engineMathAbs(posY1 - posY0) > 0.01) ? (posY0 + (posY1 - posY0) * 0.1) : posY1;}
			if(this->super.position.y < -maxY){this->super.velocity.y *= 0.2; double posY1 = -maxY; this->super.position.y = (engineMathAbs(posY1 - posY0) > 0.01) ? (posY0 + (posY1 - posY0) * 0.1) : posY1;}
		}
	}

	// x軸調整
	double barX = 0;
	double barW = outerW;
	if(this->super.super.interact.setting.isMoveableX){
		barX = -this->super.position.x * outerW / innerW;
		barW = outerW * outerW / innerW;
		if(!isBounce && this->super.position.x > -minX){this->super.position.x = -minX;}
		if(!isBounce && this->super.position.x < -maxX){this->super.position.x = -maxX;}
	}else{
		this->super.position.x = 0;
	}

	// y軸調整
	double barY = 0;
	double barH = outerH;
	if(this->super.super.interact.setting.isMoveableY){
		barY = -this->super.position.y * outerH / innerH;
		barH = outerH * outerH / innerH;
		if(!isBounce && this->super.position.y > -minY){this->super.position.y = -minY;}
		if(!isBounce && this->super.position.y < -maxY){this->super.position.y = -maxY;}
	}else{
		this->super.position.y = 0;
	}

	// 位置設定
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewInner, this->super.position.x, this->super.position.y, innerW, innerH);
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewBarX, barX, 0, barW, outerH);
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewBarY, 0, barY, outerW, barH);

	// 旧タッチ位置更新
	this->posTemp.x = this->super.swipe.x;
	this->posTemp.y = this->super.swipe.y;

	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsSwipeScrollerImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsSwipeScrollerImplement *this){
}

// 描画
static void draw(struct engineLayoutViewPartsSwipeScrollerImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsSwipeScrollerImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsSwipeScrollerImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsSwipeScroller", this);
}

// ----------------------------------------------------------------

// スクローラ構造体 作成
struct engineLayoutViewPartsSwipeScroller *engineLayoutViewPartsSwipeScrollerCreate(){
	struct engineLayoutViewPartsSwipeScrollerImplement *this = (struct engineLayoutViewPartsSwipeScrollerImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsSwipeScroller", 1, sizeof(struct engineLayoutViewPartsSwipeScrollerImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsSwipeScroller*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

