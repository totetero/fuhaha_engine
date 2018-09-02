#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体 ユーティリティの初期化
void engineLayoutViewGearInit(struct engineLayoutView *this){
	engineLayoutViewGearFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewGearPositionInit((struct engineLayoutView*)this);
	engineLayoutViewGearGraphicObjectInit((struct engineLayoutView*)this);
	this->color.r = 1.0;
	this->color.g = 1.0;
	this->color.b = 1.0;
	this->color.a = 1.0;
}

// 表示要素構造体 ユーティリティの破棄
void engineLayoutViewGearDispose(struct engineLayoutView *this){
	engineLayoutViewGearGraphicObjectDispose((struct engineLayoutView*)this);
	engineLayoutViewGearPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewGearFamilyDispose((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体 デフォルト関数
bool engineLayoutViewDefaultTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewGearChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewGearInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultCalc(struct engineLayoutView *this, bool isCancel){
	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultPause(struct engineLayoutView *this){
	// 子要素一時停止
	engineLayoutViewGearChildrenPause((struct engineLayoutView*)this);
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultDispose(struct engineLayoutView *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutView default", this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutView *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// 表示要素構造体 作成
struct engineLayoutView *engineLayoutViewCreate(){
	struct engineLayoutView *this = (struct engineLayoutView*)engineUtilMemoryInfoCalloc("engineLayoutView", 1, sizeof(struct engineLayoutView));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutView*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

