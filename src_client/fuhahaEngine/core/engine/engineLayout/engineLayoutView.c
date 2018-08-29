#include "../../library.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体 ユーティリティの初期化
void engineLayoutViewUtilInit(struct engineLayoutView *this){
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	engineLayoutViewUtilGraphicObjectInit((struct engineLayoutView*)this);
	this->color.r = 1.0;
	this->color.g = 1.0;
	this->color.b = 1.0;
	this->color.a = 1.0;
}

// 表示要素構造体 ユーティリティの破棄
void engineLayoutViewUtilDispose(struct engineLayoutView *this){
	engineLayoutViewUtilGraphicObjectDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体 デフォルト関数
bool engineLayoutViewDefaultTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultCalc(struct engineLayoutView *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultPause(struct engineLayoutView *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 表示要素構造体 デフォルト関数
void engineLayoutViewDefaultDispose(struct engineLayoutView *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutView default", this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutView *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);
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
	view->graphicObject.shouldBufferCreate = engineLayoutViewUtilGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewUtilGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutView*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

