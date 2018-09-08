#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPagerPage3Implement{
	struct pageTest2PartsPagerPage3 super;
	struct pageTest2Status *stat;

	struct engineLayoutPartsButtonBox *viewTestButton1;
	struct engineLayoutPartsButtonBox *viewTestButton2;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPagerPage3Implement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// フレーム作成
	struct engineLayoutPartsFrame *viewTestFrame = engineLayoutPartsFrameCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewTestFrame);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewTestFrame);

	// テキスト描画
	struct engineLayoutPartsFontText *viewTestText = engineLayoutPartsFontTextCreateDefault("3ページ目");
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)viewTestText, 0, 0, 20, 20);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestText);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestText);

	// ボタン描画
	this->viewTestButton1 = engineLayoutPartsButtonBoxCreateText("サンプルポップアップ表示");
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)this->viewTestButton1, 10, 10, 50, 100);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton1);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton1);

	// ボタン描画
	this->viewTestButton2 = engineLayoutPartsButtonBoxCreateText("アクティブポップアップ表示");
	engineLayoutViewGearPositionSetLtRtTpHt((struct engineLayoutView*)this->viewTestButton2, 10, 10, 160, 100);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton2);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton2);
}

// ----------------------------------------------------------------

static void openPopupSample(struct pageTest2PartsPagerPage3Implement *this);

// サンプルポップアップ完了時
static void onClosePopupSample(struct engineCartridgeLayoutPopup *cartridge){
	int result = cartridge->result;
	trace("popup2 %d", result);
	// test もう一回ポップアップ
	if(result == 1){openPopupSample(cartridge->super.param);}
}

// サンプルポップアップ展開
static void openPopupSample(struct pageTest2PartsPagerPage3Implement *this){
	// ポップアップカートリッジ作成
	struct engineCartridgeLayoutPopup *cartridge = engineCartridgeLayoutPopupCreate();
	engineCartridgeLayoutPopupSetView(cartridge, (struct engineLayoutView*)pageTest2PartsPopupSampleCreate(cartridge, this->stat));
	cartridge->super.param = this;
	cartridge->super.onClose = (void(*)(struct engineCartridgePopup*))onClosePopupSample;
	// ポップアップカートリッジ装填
	engineCartridgePopupManagerPush(&this->stat->cartridge->popupManager, (struct engineCartridgePopup*)cartridge);
}

// ----------------------------------------------------------------

// アクティブポップアップ展開
static void openPopupActive(struct pageTest2PartsPagerPage3Implement *this){
	// ポップアップカートリッジ作成
	struct engineCartridgeLayoutPopup *cartridge = engineCartridgeLayoutPopupCreate();
	engineCartridgeLayoutPopupSetView(cartridge, (struct engineLayoutView*)pageTest2PartsPopupActiveCreate(cartridge, this->stat));
	// ポップアップカートリッジ装填
	engineCartridgePopupManagerPush(&this->stat->cartridge->popupManager, (struct engineCartridgePopup*)cartridge);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2PartsPagerPage3Implement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);

	// サンプルポップアップボタン処理
	if(engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->viewTestButton1)){
		// サンプルポップアップを開く
		openPopupSample(this);
	}

	// アクティブポップアップボタン処理
	if(engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->viewTestButton2)){
		// アクティブポップアップを開く
		openPopupActive(this);
	}
}

// ----------------------------------------------------------------

// ページ3構造体 作成
struct pageTest2PartsPagerPage3 *pageTest2PartsPagerPage3Create(struct pageTest2Status *stat){
	struct pageTest2PartsPagerPage3Implement *this = (struct pageTest2PartsPagerPage3Implement*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2PartsPagerPage3Implement));
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
	return (struct pageTest2PartsPagerPage3*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

