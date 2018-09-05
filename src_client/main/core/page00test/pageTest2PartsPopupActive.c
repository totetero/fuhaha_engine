#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPopupActiveImplement{
	struct pageTest2PartsPopupActive super;
	struct engineCartridgeLayoutPopup *popup;
	struct pageTest2Status *stat;

	struct engineLayoutPartsMaskRect *viewDisplayMask;
	struct pageTest2PartsPopupActiveDisplay *viewDisplay;
	struct engineLayoutPartsFrame *viewTitle;
	struct engineLayoutPartsButtonBox *viewButtonClose;
	struct engineLayoutPartsArrowKey *viewArrowKey;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPopupActiveImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// ディスプレイマスク作成
	this->viewDisplayMask = engineLayoutPartsMaskRectCreate();
	engineLayoutViewGearPositionSetLtRtTpBm((struct engineLayoutView*)this->viewDisplayMask, 10, 10, 60, 154);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewDisplayMask);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewDisplayMask);
	//// ディスプレイバック作成
	//struct engineLayoutPartsRect *viewDisplayBack = engineLayoutPartsRectCreateWhite();
	//engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)viewDisplayBack);
	//engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)viewDisplayBack);
	// ディスプレイ作成
	this->viewDisplay = pageTest2PartsPopupActiveDisplayCreate(this->stat);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)this->viewDisplay);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)this->viewDisplay);

	// フレーム作成
	this->viewTitle = engineLayoutPartsFrameCreate();
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)this->viewTitle, 10, 80, 10, 40);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewTitle);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewTitle);

	// テキスト描画
	struct engineLayoutPartsFontText *viewTestText = engineLayoutPartsFontTextCreateDefault("アクティブポップアップ");
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewTitle, (struct engineLayoutView*)viewTestText);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewTitle, (struct engineLayoutView*)viewTestText);

	// 閉じるボタン描画
	this->viewButtonClose = engineLayoutPartsButtonBoxCreateText("close");
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewGearPositionSetTop((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->viewButtonClose, 60);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->viewButtonClose, 40);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonClose);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonClose);

	// 十字キー作成
	this->viewArrowKey = engineLayoutPartsArrowKeyCreate();
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->viewArrowKey, 0);
	engineLayoutViewGearPositionSetBottom((struct engineLayoutView*)this->viewArrowKey, 0);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewArrowKey);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewArrowKey);

	// ディスプレイに十字キーを紐づける
	this->viewDisplay->viewArrowKey = this->viewArrowKey;
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2PartsPopupActiveImplement *this, bool isCancel){
	// 遷移中のディスプレイフェード
	engineLayoutViewGearPositionUnsetTransform((struct engineLayoutView*)this->viewDisplayMask);
	engineLayoutViewGearPositionSetTransformScale((struct engineLayoutView*)this->viewDisplayMask, 1.0, this->popup->super.openAlpha);

	// 遷移中の要素位置
	double titleTop = (this->popup->super.openAlpha - 1) * (engineLayoutViewGearPositionGetH((struct engineLayoutView*)this->viewTitle) + 10) + 10;
	double buttonCloseRight = (this->popup->super.openAlpha - 1) * (engineLayoutViewGearPositionGetW((struct engineLayoutView*)this->viewButtonClose) + 10) + 10;
	double arrowKeyLeft = (this->popup->super.openAlpha - 1) * engineLayoutViewGearPositionGetW((struct engineLayoutView*)this->viewArrowKey);
	engineLayoutViewGearPositionSetTop((struct engineLayoutView*)this->viewTitle, titleTop);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->viewButtonClose, buttonCloseRight);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->viewArrowKey, arrowKeyLeft);

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);

	// ポップアップ遷移中の処理スキップ
	if(this->popup->super.openStep != 0){return;}

	// ボタン処理 閉じる
	if(engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonClose)){
		// ポップアップを閉じる
		this->popup->super.isExist = false;
		return;
	}
}

// ----------------------------------------------------------------

// アクティブポップアップ構造体 作成
struct pageTest2PartsPopupActive *pageTest2PartsPopupActiveCreate(struct engineCartridgeLayoutPopup *popup, struct pageTest2Status *stat){
	struct pageTest2PartsPopupActiveImplement *this = (struct pageTest2PartsPopupActiveImplement*)engineUtilMemoryInfoCalloc("pageTest2PartsPopupActive", 1, sizeof(struct pageTest2PartsPopupActiveImplement));
	this->popup = popup;
	this->stat = stat;
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct pageTest2PartsPopupActive*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

