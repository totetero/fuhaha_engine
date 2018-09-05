#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPopupSampleImplement{
	struct pageTest2PartsPopupSample super;
	struct engineCartridgeLayoutPopup *popup;
	struct pageTest2Status *stat;

	struct engineLayoutPartsFrame *viewWindow;
	struct engineLayoutPartsButtonBox *viewButtonReopen;
	struct engineLayoutPartsButtonBox *viewButtonClose;
	struct engineLayoutPartsButtonOuter *viewButtonOuter;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPopupSampleImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// 外枠ボタン描画
	this->viewButtonOuter = engineLayoutPartsButtonOuterCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonOuter);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonOuter);

	// フレーム作成
	this->viewWindow = engineLayoutPartsFrameCreate();
	engineLayoutViewGearPositionSetTop((struct engineLayoutView*)this->viewWindow, 30);
	engineLayoutViewGearPositionSetBottom((struct engineLayoutView*)this->viewWindow, 30);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->viewWindow, 300);
	engineLayoutViewGearPositionSetClampVertical((struct engineLayoutView*)this->viewWindow, 160, 400, 0);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewButtonOuter, (struct engineLayoutView*)this->viewWindow);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewButtonOuter, (struct engineLayoutView*)this->viewWindow);
	this->viewWindow->super.interact.setting.isTouchable = true;

	// テキスト描画
	struct engineLayoutPartsFontText *viewTestText = engineLayoutPartsFontTextCreateDefault("サンプルポップアップ");
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)viewTestText, 10, 10, 10, 40);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)viewTestText);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)viewTestText);

	// 再オープンボタン描画
	this->viewButtonReopen = engineLayoutPartsButtonBoxCreateText("再オープン");
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->viewButtonReopen, 10);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->viewButtonReopen, 10);
	engineLayoutViewGearPositionSetBottom((struct engineLayoutView*)this->viewButtonReopen, 60);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->viewButtonReopen, 40);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonReopen);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonReopen);

	// 閉じるボタン描画
	this->viewButtonClose = engineLayoutPartsButtonBoxCreateText("close");
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewGearPositionSetBottom((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->viewButtonClose, 40);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonClose);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonClose);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2PartsPopupSampleImplement *this, bool isCancel){
	// 遷移中のウインドウフェード
	engineLayoutViewGearPositionUnsetTransform((struct engineLayoutView*)this->viewWindow);
	engineLayoutViewGearPositionSetTransformScale((struct engineLayoutView*)this->viewWindow, this->popup->super.openSize, this->popup->super.openSize);
	engineMathVec4Set(&this->viewWindow->super.color, 1.0, 1.0, 1.0, this->popup->super.openAlpha);

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);

	// ポップアップ遷移中の処理スキップ
	if(this->popup->super.openStep != 0){return;}

	// ボタン処理 再オープン
	if(engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonReopen)){
		// ポップアップを閉じる
		this->popup->result = 1;
		this->popup->super.isExist = false;
		return;
	}

	// ボタン処理 閉じる
	if(engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonClose) || engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonOuter)){
		// ポップアップを閉じる
		this->popup->result = 0;
		this->popup->super.isExist = false;
		return;
	}
}

// ----------------------------------------------------------------

// サンプルポップアップ構造体 作成
struct pageTest2PartsPopupSample *pageTest2PartsPopupSampleCreate(struct engineCartridgeLayoutPopup *popup, struct pageTest2Status *stat){
	struct pageTest2PartsPopupSampleImplement *this = (struct pageTest2PartsPopupSampleImplement*)engineUtilMemoryInfoCalloc("pageTest2PartsPopupSample", 1, sizeof(struct pageTest2PartsPopupSampleImplement));
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
	return (struct pageTest2PartsPopupSample*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

