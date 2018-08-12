#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPagerPage2Implement{
	struct pageTest2PartsPagerPage2 super;
	struct pageTest2Status *stat;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPagerPage2Implement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// フレーム作成
	struct engineLayoutViewPartsFrame *viewFrame = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);

	// スクローラマスク作成
	struct engineLayoutViewPartsMaskRect *viewScrollerMask = engineLayoutViewPartsMaskRectCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewScrollerMask, 10, 10, 10, 10);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewScrollerMask);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewScrollerMask);
	// スクローラ作成
	struct engineLayoutViewPartsSwipeScroller *viewScroller = engineLayoutViewPartsSwipeScrollerCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewScrollerMask, (struct engineLayoutView*)viewScroller);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewScrollerMask, (struct engineLayoutView*)viewScroller);
	viewScroller->swipe.parentSwipe = &this->stat->viewPager->swipe;
	viewScroller->super.interact.setting.isMoveableY = false;
	viewScroller->inner.h = 10;
	// 垂直軸スクロールバー作成
	struct engineLayoutViewPartsRect *viewScrollBarY = engineLayoutViewPartsRectCreateWhite();
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewScrollBarY, 6);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);

	// スクローラ内部テキスト描画
	double viewTextTop = viewScroller->inner.h;
	double viewTextHeight = 20;
	viewScroller->inner.h += viewTextHeight + 10;
	struct engineLayoutViewPartsFontText *viewText = engineLayoutViewPartsFontTextCreateDefault("2ページ目");
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)viewText, 0, 0, viewTextTop, viewTextHeight);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewText);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewText);

	// ページャーマスク作成
	double viewPagerMaskTop = viewScroller->inner.h;
	double viewPagerMaskHeight = 200;
	viewScroller->inner.h += viewPagerMaskHeight + 10;
	struct engineLayoutViewPartsMaskRect *viewPagerMask = engineLayoutViewPartsMaskRectCreate();
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)viewPagerMask, 0, 10, viewPagerMaskTop, viewPagerMaskHeight);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewPagerMask);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewPagerMask);
	// スクローラ内部ページャー作成
	struct engineLayoutViewPartsSwipePager *viewPager = engineLayoutViewPartsSwipePagerCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)viewPager);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)viewPager);
	viewPager->swipe.parentSwipe = &viewScroller->swipe;
	viewPager->isLoop = true;
	// バナー1作成
	struct engineLayoutViewPartsButtonBox *viewBanner1 = engineLayoutViewPartsButtonBoxCreateText("バナー1");
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner1);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner1);
	// バナー2作成
	struct engineLayoutViewPartsButtonBox *viewBanner2 = engineLayoutViewPartsButtonBoxCreateText("バナー2");
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner2);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner2);
	// バナー3作成
	struct engineLayoutViewPartsButtonBox *viewBanner3 = engineLayoutViewPartsButtonBoxCreateText("バナー3");
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner3);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner3);

	// スクロール要素作成
	for(int i = 0; i < 5; i++){
		char label[32];
		snprintf(label, sizeof(label), "ボタン%d", i + 1);
		double viewScrollItemTop = viewScroller->inner.h;
		double viewScrollItemHeight = 100;
		viewScroller->inner.h += viewScrollItemHeight + 10;
		struct engineLayoutViewPartsButtonBox *viewScrollItem = engineLayoutViewPartsButtonBoxCreateText(label);
		engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)viewScrollItem, 0, 10, viewScrollItemTop, viewScrollItemHeight);
		engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
		engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
	}
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct pageTest2PartsPagerPage2Implement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct pageTest2PartsPagerPage2Implement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct pageTest2PartsPagerPage2Implement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct pageTest2PartsPagerPage2Implement *this){
}

// 描画
static void draw(struct pageTest2PartsPagerPage2Implement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2PartsPagerPage2Implement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct pageTest2PartsPagerPage2Implement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("pageTest2PartsPagerPage2", this);
}

// ----------------------------------------------------------------

// ページ2構造体 作成
struct pageTest2PartsPagerPage2 *pageTest2PartsPagerPage2Create(struct pageTest2Status *stat){
	struct pageTest2PartsPagerPage2Implement *this = (struct pageTest2PartsPagerPage2Implement*)engineUtilMemoryInfoCalloc("pageTest2PartsPagerPage2", 1, sizeof(struct pageTest2PartsPagerPage2Implement));
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
	return (struct pageTest2PartsPagerPage2*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

