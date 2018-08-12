#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPagerPage1Implement{
	struct pageTest2PartsPagerPage1 super;
	struct pageTest2Status *stat;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPagerPage1Implement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// フレーム作成
	struct engineLayoutViewPartsFrame *viewFrame = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);

	// テキスト描画
	struct engineLayoutViewPartsFontText *viewText = engineLayoutViewPartsFontTextCreateDefault("1ページ目");
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)viewText, 0, 0, 20, 20);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewText);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewText);

	// レイアウト作成
	struct engineLayoutView *viewImageLayout = engineLayoutViewCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewImageLayout, 0, 0, 50, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewImageLayout);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewImageLayout);
	// 画像作成
	struct engineLayoutViewPartsRect *viewImage = engineLayoutViewPartsRectCreate("test.png", 64, 64, 0, 0, 64, 64);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewImage, 120);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewImage, 120);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewImageLayout, (struct engineLayoutView*)viewImage);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewImageLayout, (struct engineLayoutView*)viewImage);

}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct pageTest2PartsPagerPage1Implement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct pageTest2PartsPagerPage1Implement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct pageTest2PartsPagerPage1Implement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct pageTest2PartsPagerPage1Implement *this){
}

// 描画
static void draw(struct pageTest2PartsPagerPage1Implement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2PartsPagerPage1Implement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct pageTest2PartsPagerPage1Implement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("pageTest2PartsPagerPage1", this);
}

// ----------------------------------------------------------------

// ページ1構造体 作成
struct pageTest2PartsPagerPage1 *pageTest2PartsPagerPage1Create(struct pageTest2Status *stat){
	struct pageTest2PartsPagerPage1Implement *this = (struct pageTest2PartsPagerPage1Implement*)engineUtilMemoryInfoCalloc("pageTest2PartsPagerPage1", 1, sizeof(struct pageTest2PartsPagerPage1Implement));
	this->stat = stat;
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct pageTest2PartsPagerPage1*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

