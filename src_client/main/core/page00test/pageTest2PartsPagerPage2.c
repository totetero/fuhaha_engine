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
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// フレーム作成
	struct engineLayoutPartsFrame *viewFrame = engineLayoutPartsFrameCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);

	// スクローラマスク作成
	struct engineLayoutPartsMaskRect *viewScrollerMask = engineLayoutPartsMaskRectCreate();
	engineLayoutViewGearPositionSetLtRtTpBm((struct engineLayoutView*)viewScrollerMask, 10, 10, 10, 10);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewScrollerMask);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewScrollerMask);
	// スクローラ作成
	struct engineLayoutPartsSwipeScroller *viewScroller = engineLayoutPartsSwipeScrollerCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewScrollerMask, (struct engineLayoutView*)viewScroller);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewScrollerMask, (struct engineLayoutView*)viewScroller);
	viewScroller->swipe.parentSwipe = &this->stat->viewPager->swipe;
	viewScroller->super.interact.setting.isMoveableY = false;
	viewScroller->inner.h = 10;
	// 垂直軸スクロールバー作成
	struct engineLayoutPartsRect *viewScrollBarY = engineLayoutPartsRectCreateWhite();
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewGearPositionSetTop((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewGearPositionSetBottom((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)viewScrollBarY, 6);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);

	// スクローラ内部テキスト描画
	double viewTextTop = viewScroller->inner.h;
	double viewTextHeight = 20;
	viewScroller->inner.h += viewTextHeight + 10;
	struct engineLayoutPartsFontText *viewText = engineLayoutPartsFontTextCreateDefault("2ページ目");
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)viewText, 0, 0, viewTextTop, viewTextHeight);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewText);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewText);

	// ページャーマスク作成
	double viewPagerMaskTop = viewScroller->inner.h;
	double viewPagerMaskHeight = 200;
	viewScroller->inner.h += viewPagerMaskHeight + 10;
	struct engineLayoutPartsMaskRect *viewPagerMask = engineLayoutPartsMaskRectCreate();
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)viewPagerMask, 0, 10, viewPagerMaskTop, viewPagerMaskHeight);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewPagerMask);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewPagerMask);
	// スクローラ内部ページャー作成
	struct engineLayoutPartsSwipePager *viewPager = engineLayoutPartsSwipePagerCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)viewPager);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)viewPager);
	viewPager->swipe.parentSwipe = &viewScroller->swipe;
	viewPager->isLoop = true;
	// バナー1作成
	struct engineLayoutPartsButtonBox *viewBanner1 = engineLayoutPartsButtonBoxCreateText("バナー1");
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner1);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner1);
	// バナー2作成
	struct engineLayoutPartsButtonBox *viewBanner2 = engineLayoutPartsButtonBoxCreateText("バナー2");
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner2);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner2);
	// バナー3作成
	struct engineLayoutPartsButtonBox *viewBanner3 = engineLayoutPartsButtonBoxCreateText("バナー3");
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner3);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewPager->viewInner, (struct engineLayoutView*)viewBanner3);

	// スクロール要素作成
	for(int i = 0; i < 5; i++){
		char label[32];
		snprintf(label, sizeof(label), "ボタン%d", i + 1);
		double viewScrollItemTop = viewScroller->inner.h;
		double viewScrollItemHeight = 100;
		viewScroller->inner.h += viewScrollItemHeight + 10;
		struct engineLayoutPartsButtonBox *viewScrollItem = engineLayoutPartsButtonBoxCreateText(label);
		engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)viewScrollItem, 0, 10, viewScrollItemTop, viewScrollItemHeight);
		engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
		engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
	}
}

// ----------------------------------------------------------------

// ページ2構造体 作成
struct pageTest2PartsPagerPage2 *pageTest2PartsPagerPage2Create(struct pageTest2Status *stat){
	struct pageTest2PartsPagerPage2Implement *this = (struct pageTest2PartsPagerPage2Implement*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2PartsPagerPage2Implement));
	this->stat = stat;
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct pageTest2PartsPagerPage2*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

