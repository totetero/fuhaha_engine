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

// タッチ処理
static bool touch(struct pageTest2PartsPopupSampleImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

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

// バッファ更新確認
static bool shouldBufferCreate(struct pageTest2PartsPopupSampleImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct pageTest2PartsPopupSampleImplement *this){
}

// 描画
static void draw(struct pageTest2PartsPopupSampleImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2PartsPopupSampleImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct pageTest2PartsPopupSampleImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("pageTest2PartsPopupSample", this);
}

// ----------------------------------------------------------------

// サンプルポップアップ構造体 作成
struct pageTest2PartsPopupSample *pageTest2PartsPopupSampleCreate(struct pageTest2CartridgePopup *popup, struct pageTest2Status *stat){
	struct pageTest2PartsPopupSampleImplement *this = (struct pageTest2PartsPopupSampleImplement*)engineUtilMemoryInfoCalloc("pageTest2PartsPopupSample", 1, sizeof(struct pageTest2PartsPopupSampleImplement));
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
	return (struct pageTest2PartsPopupSample*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

