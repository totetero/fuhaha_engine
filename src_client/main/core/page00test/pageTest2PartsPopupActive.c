#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPopupActiveImplement{
	struct pageTest2PartsPopupActive super;
	struct pageTest2CartridgePopup *popup;
	struct pageTest2Status *stat;

	struct engineLayoutViewPartsMaskRect *viewDisplayMask;
	struct pageTest2PartsPopupActiveDisplay *viewDisplay;
	struct engineLayoutViewPartsFrame *viewTitle;
	struct engineLayoutViewPartsButtonBox *viewButtonClose;
	struct engineLayoutViewPartsArrowKey *viewArrowKey;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPopupActiveImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// ディスプレイマスク作成
	this->viewDisplayMask = engineLayoutViewPartsMaskRectCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)this->viewDisplayMask, 10, 10, 60, 154);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewDisplayMask);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewDisplayMask);
	//// ディスプレイバック作成
	//struct engineLayoutViewPartsRect *viewDisplayBack = engineLayoutViewPartsRectCreateWhite();
	//engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)viewDisplayBack);
	//engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)viewDisplayBack);
	// ディスプレイ作成
	this->viewDisplay = pageTest2PartsPopupActiveDisplayCreate(this->stat);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)this->viewDisplay);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewDisplayMask, (struct engineLayoutView*)this->viewDisplay);

	// フレーム作成
	this->viewTitle = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)this->viewTitle, 10, 80, 10, 40);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewTitle);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewTitle);

	// テキスト描画
	struct engineLayoutViewPartsFontText *viewTestText = engineLayoutViewPartsFontTextCreateDefault("アクティブポップアップ");
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewTitle, (struct engineLayoutView*)viewTestText);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewTitle, (struct engineLayoutView*)viewTestText);

	// 閉じるボタン描画
	this->viewButtonClose = engineLayoutViewPartsButtonBoxCreateText("close");
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)this->viewButtonClose, 10);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)this->viewButtonClose, 60);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)this->viewButtonClose, 40);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonClose);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewButtonClose);

	// 十字キー作成
	this->viewArrowKey = engineLayoutViewPartsArrowKeyCreate();
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)this->viewArrowKey, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewArrowKey, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewArrowKey);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->viewArrowKey);

	// ディスプレイに十字キーを紐づける
	this->viewDisplay->viewArrowKey = this->viewArrowKey;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct pageTest2PartsPopupActiveImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct pageTest2PartsPopupActiveImplement *this, bool isCancel){
	// 遷移中のディスプレイフェード
	engineLayoutViewUtilPositionUnsetTransform((struct engineLayoutView*)this->viewDisplayMask);
	engineLayoutViewUtilPositionSetTransformScale((struct engineLayoutView*)this->viewDisplayMask, 1.0, this->popup->super.openAlpha);

	// 遷移中の要素位置
	double titleTop = (this->popup->super.openAlpha - 1) * (engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this->viewTitle) + 10) + 10;
	double buttonCloseRight = (this->popup->super.openAlpha - 1) * (engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this->viewButtonClose) + 10) + 10;
	double arrowKeyLeft = (this->popup->super.openAlpha - 1) * engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this->viewArrowKey);
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)this->viewTitle, titleTop);
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)this->viewButtonClose, buttonCloseRight);
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)this->viewArrowKey, arrowKeyLeft);

	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);

	// ポップアップ遷移中の処理スキップ
	if(this->popup->super.openStep != 0){return;}

	// ボタン処理 閉じる
	if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewButtonClose)){
		// ポップアップを閉じる
		this->popup->super.isExist = false;
		return;
	}
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct pageTest2PartsPopupActiveImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct pageTest2PartsPopupActiveImplement *this){
}

// 描画
static void draw(struct pageTest2PartsPopupActiveImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2PartsPopupActiveImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct pageTest2PartsPopupActiveImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("pageTest2PartsPopupActive", this);
}

// ----------------------------------------------------------------

// アクティブポップアップ構造体 作成
struct pageTest2PartsPopupActive *pageTest2PartsPopupActiveCreate(struct pageTest2CartridgePopup *popup, struct pageTest2Status *stat){
	struct pageTest2PartsPopupActiveImplement *this = (struct pageTest2PartsPopupActiveImplement*)engineUtilMemoryInfoCalloc("pageTest2PartsPopupActive", 1, sizeof(struct pageTest2PartsPopupActiveImplement));
	this->popup = popup;
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
	return (struct pageTest2PartsPopupActive*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

