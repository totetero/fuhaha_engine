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
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// フレーム作成
	struct engineLayoutPartsFrame *viewFrame = engineLayoutPartsFrameCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewFrame);

	// テキスト描画
	struct engineLayoutPartsFontText *viewText = engineLayoutPartsFontTextCreateDefault("1ページ目");
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)viewText, 0, 0, 20, 20);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewText);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewText);

	// レイアウト作成
	struct engineLayoutView *viewImageLayout = engineLayoutViewCreate();
	engineLayoutViewGearPositionSetLtRtTpBm((struct engineLayoutView*)viewImageLayout, 0, 0, 50, 0);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewImageLayout);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewFrame, (struct engineLayoutView*)viewImageLayout);
	// 画像作成
	struct engineLayoutPartsRect *viewImage = engineLayoutPartsRectCreate("test.png", 64, 64, 0, 0, 64, 64);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)viewImage, 120);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)viewImage, 120);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewImageLayout, (struct engineLayoutView*)viewImage);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewImageLayout, (struct engineLayoutView*)viewImage);

}

// ----------------------------------------------------------------

// ページ1構造体 作成
struct pageTest2PartsPagerPage1 *pageTest2PartsPagerPage1Create(struct pageTest2Status *stat){
	struct pageTest2PartsPagerPage1Implement *this = (struct pageTest2PartsPagerPage1Implement*)engineUtilMemoryInfoCalloc("pageTest2PartsPagerPage1", 1, sizeof(struct pageTest2PartsPagerPage1Implement));
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
	return (struct pageTest2PartsPagerPage1*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

