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
	view->graphicObject.shouldBufferCreate = engineLayoutViewUtilGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewUtilGraphicObjectDefaultBufferCreate;
	return (struct pageTest2PartsPagerPage1*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

