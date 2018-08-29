#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPopupSampleImplement{
	struct pageTest2PartsPopupSample super;
	struct pageTest2CartridgePopup *popup;
	struct pageTest2Status *stat;

	struct engineLayoutViewPartsFrame *viewWindow;
	struct engineLayoutViewPartsButtonBox *viewButtonReopen;
	struct engineLayoutViewPartsButtonBox *viewButtonClose;
	struct engineLayoutViewPartsButtonOuter *viewButtonOuter;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPopupSampleImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// 外枠ボタン描画
	this->viewButtonOuter = engineLayoutViewPartsButtonOuterCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonOuter);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonOuter);

	// フレーム作成
	this->viewWindow = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)this->viewWindow, 30);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewWindow, 30);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)this->viewWindow, 300);
	engineLayoutViewUtilPositionSetClampVertical((struct engineLayoutView*)this->viewWindow, 160, 400, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewButtonOuter, (struct engineLayoutView*)this->viewWindow);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewButtonOuter, (struct engineLayoutView*)this->viewWindow);
	this->viewWindow->super.interact.setting.isTouchable = true;

	// テキスト描画
	struct engineLayoutViewPartsFontText *viewTestText = engineLayoutViewPartsFontTextCreateDefault("サンプルポップアップ");
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)viewTestText, 10, 10, 10, 40);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)viewTestText);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)viewTestText);

	// 再オープンボタン描画
	this->viewButtonReopen = engineLayoutViewPartsButtonBoxCreateText("再オープン");
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)this->viewButtonReopen, 10);
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)this->viewButtonReopen, 10);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewButtonReopen, 60);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)this->viewButtonReopen, 40);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonReopen);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonReopen);

	// 閉じるボタン描画
	this->viewButtonClose = engineLayoutViewPartsButtonBoxCreateText("close");
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)this->viewButtonClose, 40);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonClose);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewWindow, (struct engineLayoutView*)this->viewButtonClose);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2PartsPopupSampleImplement *this, bool isCancel){
	// 遷移中のウインドウフェード
	engineLayoutViewUtilPositionUnsetTransform((struct engineLayoutView*)this->viewWindow);
	engineLayoutViewUtilPositionSetTransformScale((struct engineLayoutView*)this->viewWindow, this->popup->super.openSize, this->popup->super.openSize);
	engineMathVec4Set(&this->viewWindow->super.color, 1.0, 1.0, 1.0, this->popup->super.openAlpha);

	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);

	// ポップアップ遷移中の処理スキップ
	if(this->popup->super.openStep != 0){return;}

	// ボタン処理 再オープン
	if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonReopen)){
		// ポップアップを閉じる
		pageTest2CartridgePopupResultSet(this->popup, 1);
		this->popup->super.isExist = false;
		return;
	}

	// ボタン処理 閉じる
	if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonClose) || engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonOuter)){
		// ポップアップを閉じる
		pageTest2CartridgePopupResultSet(this->popup, 0);
		this->popup->super.isExist = false;
		return;
	}
}

// ----------------------------------------------------------------

// サンプルポップアップ構造体 作成
struct pageTest2PartsPopupSample *pageTest2PartsPopupSampleCreate(struct pageTest2CartridgePopup *popup, struct pageTest2Status *stat){
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
	view->graphicObject.shouldBufferCreate = engineLayoutViewUtilGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewUtilGraphicObjectDefaultBufferCreate;
	return (struct pageTest2PartsPopupSample*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

