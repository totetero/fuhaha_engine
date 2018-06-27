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
	struct{int x; int y;} temp;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsSwipeScrollerImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	engineLayoutViewUtilGraphicObjectInit((struct engineLayoutView*)this);
	this->super.super.interact.setting.isTouchable = true;

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
	if(this->super.super.interact.status.isActive && this->super.super.interact.status.isMove){
		// スクロール中
		this->super.velocity.x = this->super.swipe.x - this->temp.x + this->super.velocity.x * 0.3;
		this->super.velocity.y = this->super.swipe.y - this->temp.y + this->super.velocity.y * 0.3;
		this->super.position.x += this->super.swipe.x - this->temp.x;
		this->super.position.y += this->super.swipe.y - this->temp.y;
	}else if(engineMathAbs(this->super.velocity.x) > 0.01 || engineMathAbs(this->super.velocity.y) > 0.01){
		// 速度が生きてる
		this->super.velocity.x *= this->super.super.interact.status.isActive ? 0.5 : 0.9;
		this->super.velocity.y *= this->super.super.interact.status.isActive ? 0.5 : 0.9;
		this->super.position.x += this->super.velocity.x;
		this->super.position.y += this->super.velocity.y;
	}

	// 旧タッチ位置更新
	this->temp.x = this->super.swipe.x;
	this->temp.y = this->super.swipe.y;

	// 位置大きさ取得
	double innerW = this->super.inner.w;
	double innerH = this->super.inner.h;
	double outerW = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
	double outerH = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
	double barX = 0;
	double barY = 0;
	double barW = outerW;
	double barH = outerH;
	this->super.super.interact.setting.isMoveableX = (innerW > outerW);
	this->super.super.interact.setting.isMoveableY = (innerH > outerH);

	// x軸調整
	if(this->super.super.interact.setting.isMoveableX){
		double minX = 0;
		double maxX = innerW - outerW;
		if(this->super.position.x < -maxX){this->super.position.x = -maxX;}
		if(this->super.position.x > -minX){this->super.position.x = -minX;}
		barX = -this->super.position.x * outerW / innerW;
		barW = outerW * outerW / innerW;
	}else{
		this->super.position.x = 0;
		innerW = outerW;
	}

	// y軸調整
	if(this->super.super.interact.setting.isMoveableY){
		double minY = 0;
		double maxY = innerH - outerH;
		if(this->super.position.y < -maxY){this->super.position.y = -maxY;}
		if(this->super.position.y > -minY){this->super.position.y = -minY;}
		barY = -this->super.position.y * outerH / innerH;
		barH = outerH * outerH / innerH;
	}else{
		this->super.position.y = 0;
		innerH = outerH;
	}

	// 位置設定
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewInner, this->super.position.x, this->super.position.y, innerW, innerH);
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewBarX, barX, 0, barW, outerH);
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewBarY, 0, barY, outerW, barH);

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
	engineLayoutViewUtilGraphicObjectDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
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

