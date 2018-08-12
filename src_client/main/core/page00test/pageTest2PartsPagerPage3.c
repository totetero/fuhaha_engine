#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPagerPage3Implement{
	struct pageTest2PartsPagerPage3 super;
	struct pageTest2Status *stat;

	struct engineLayoutViewPartsButtonBox *viewTestButton1;
	struct engineLayoutViewPartsButtonBox *viewTestButton2;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPagerPage3Implement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// フレーム作成
	struct engineLayoutViewPartsFrame *viewTestFrame = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewTestFrame);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewTestFrame);

	// テキスト描画
	struct engineLayoutViewPartsFontText *viewTestText = engineLayoutViewPartsFontTextCreateDefault("3ページ目");
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)viewTestText, 0, 0, 20, 20);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestText);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestText);

	// ボタン描画
	this->viewTestButton1 = engineLayoutViewPartsButtonBoxCreateText("サンプルポップアップ表示");
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)this->viewTestButton1, 10, 10, 50, 100);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton1);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton1);

	// ボタン描画
	this->viewTestButton2 = engineLayoutViewPartsButtonBoxCreateText("アクティブポップアップ表示");
	engineLayoutViewUtilPositionSetLtRtTpHt((struct engineLayoutView*)this->viewTestButton2, 10, 10, 160, 100);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton2);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton2);
}

// ----------------------------------------------------------------

static void openPopupSample(struct pageTest2PartsPagerPage3Implement *this);

// サンプルポップアップ完了時
static void onClosePopupSample(struct engineCartridgePopup *cartridge){
	int result = pageTest2CartridgePopupResultGet(cartridge);
	trace("popup %d", result);
	// test もう一回ポップアップ
	if(result == 1){openPopupSample(cartridge->param);}
}

// サンプルポップアップ展開
static void openPopupSample(struct pageTest2PartsPagerPage3Implement *this){
	// ポップアップカートリッジ作成
	struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePopup*, void*) = (struct engineLayoutView*(*)(struct pageTest2CartridgePopup*, void*))pageTest2PartsPopupSampleCreate;
	struct pageTest2CartridgePopup *cartridge = pageTest2CartridgePopupCreate(this->stat, viewCreate, NULL);
	cartridge->super.param = this;
	cartridge->super.onClose = onClosePopupSample;
	// ポップアップカートリッジ装填
	engineCartridgePopupManagerPush(&this->stat->cartridge->popupManager, (struct engineCartridgePopup*)cartridge);
}

// ----------------------------------------------------------------

// アクティブポップアップ展開
static void openPopupActive(struct pageTest2PartsPagerPage3Implement *this){
	// ポップアップカートリッジ作成
	struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePopup*, void*) = (struct engineLayoutView*(*)(struct pageTest2CartridgePopup*, void*))pageTest2PartsPopupActiveCreate;
	struct pageTest2CartridgePopup *cartridge = pageTest2CartridgePopupCreate(this->stat, viewCreate, NULL);
	// ポップアップカートリッジ装填
	engineCartridgePopupManagerPush(&this->stat->cartridge->popupManager, (struct engineCartridgePopup*)cartridge);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct pageTest2PartsPagerPage3Implement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct pageTest2PartsPagerPage3Implement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);

	// サンプルポップアップボタン処理
	if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewTestButton1)){
		// サンプルポップアップを開く
		openPopupSample(this);
	}

	// アクティブポップアップボタン処理
	if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewTestButton2)){
		// アクティブポップアップを開く
		openPopupActive(this);
	}
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct pageTest2PartsPagerPage3Implement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct pageTest2PartsPagerPage3Implement *this){
}

// 描画
static void draw(struct pageTest2PartsPagerPage3Implement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2PartsPagerPage3Implement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct pageTest2PartsPagerPage3Implement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("pageTest2PartsPagerPage3", this);
}

// ----------------------------------------------------------------

// ページ3構造体 作成
struct pageTest2PartsPagerPage3 *pageTest2PartsPagerPage3Create(struct pageTest2Status *stat){
	struct pageTest2PartsPagerPage3Implement *this = (struct pageTest2PartsPagerPage3Implement*)engineUtilMemoryInfoCalloc("pageTest2PartsPagerPage3", 1, sizeof(struct pageTest2PartsPagerPage3Implement));
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
	return (struct pageTest2PartsPagerPage3*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

