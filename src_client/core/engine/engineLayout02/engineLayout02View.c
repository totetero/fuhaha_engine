#include "../../library.h"
#include "../engineUtil/engineUtil.h"
#include "./engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02View *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);
}

// タッチ処理
static bool touch(struct engineLayout02View *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	isCancel = (isCancel || ((struct engineLayout02View*)this)->interact.setting.isDisable);

	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02View *this){
	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// 描画
static void draw(struct engineLayout02View *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// 一時停止
static void pause(struct engineLayout02View *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02View *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewDetouch((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02View", this);
}

// ----------------------------------------------------------------

// 表示要素構造体 作成
struct engineLayout02View *engineLayout02ViewCreate(){
	struct engineLayout02View *this = (struct engineLayout02View*)engineUtilMemoryInfoCalloc("engineLayout02View", 1, sizeof(struct engineLayout02View));
	init(this);

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

