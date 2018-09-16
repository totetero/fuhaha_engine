#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsTemplateImplement{
	struct engineLayoutPartsTemplate super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsTemplateImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutPartsTemplateImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewGearChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewGearInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutPartsTemplateImplement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutPartsTemplateImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutPartsTemplateImplement *this){
}

// 描画
static void draw(struct engineLayoutPartsTemplateImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutPartsTemplateImplement *this){
	// 子要素一時停止
	engineLayoutViewGearChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutPartsTemplateImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// テンプレート構造体 作成
struct engineLayoutPartsTemplate *engineLayoutPartsTemplateCreate(void){
	struct engineLayoutPartsTemplateImplement *this = (struct engineLayoutPartsTemplateImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineLayoutPartsTemplateImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutPartsTemplate*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

