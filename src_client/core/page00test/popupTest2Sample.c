#include "../library.h"
#include "../engine/engineMath/engineMath.h"
#include "../engine/engineUtil/engineUtil.h"
#include "../engine/engineCtrl/engineCtrl.h"
#include "../engine/engineGraphic/engineGraphic.h"
#include "../engine/engineCartridge/engineCartridge.h"
#include "../engine/engineParts/engineParts.h"
#include "../game.h"
#include "pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct popupCartridgeTest2Sample{
	struct engineCartridgePopup super;
	struct engineCartridgePopupManager popupManager;
	struct pageTest2Status *stat;

	struct engineGraphicTransManager transManager;
	struct engineGraphicTrans transRoot;
	struct engineGraphicTrans transBack;
	struct engineGraphicTrans transWindow;
	struct engineGraphicImageRect imgBack;
	struct engineGraphicImageFrame imgFrame;
	struct engineGraphicImageText imgTextTitle;
	struct engineGraphicFont fontTest;
	struct enginePartsButtonBox btnBoxOk;
	struct enginePartsButtonBox btnBoxClose;
	struct enginePartsButton btnOuter;

	int bufferStatus;
	int bufferScreen;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;

	int result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeTest2Sample *this){
	engineCartridgePopupManagerInit(&this->popupManager);
	engineGraphicTransInit(&this->transRoot);
	engineGraphicTransInit(&this->transBack);
	engineGraphicTransInit(&this->transWindow);
	engineGraphicImageRectInit(&this->imgBack);
	engineGraphicImageFrameInit(&this->imgFrame);
	engineGraphicImageTextInit(&this->imgTextTitle);
	engineGraphicFontInit(&this->fontTest);
	enginePartsButtonBoxInit(&this->btnBoxOk);
	enginePartsButtonBoxInit(&this->btnBoxClose);
	enginePartsButtonInit(&this->btnOuter);
	this->btnOuter.inner = false;
}

// ----------------------------------------------------------------

// ボタン計算
static bool calcButton(struct popupCartridgeTest2Sample *this){
	// ボタン押下確認
	if(this->btnBoxOk.super.trigger){
		this->btnBoxOk.super.trigger = false;
		// ポップアップ閉じる
		this->result = 1;
		this->super.isExist = false;
		return true;
	}

	// ボタン押下確認
	if(this->btnBoxClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnBoxClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		// ポップアップ閉じる
		this->result = 0;
		this->super.isExist = false;
		return true;
	}

	return false;
}

// 計算
static void calc(struct popupCartridgeTest2Sample *this){
	// 遷移中の処理スキップ
	if(this->super.openStep != 0){return;}
	// ポップアップ処理
	if(engineCartridgePopupManagerCalc(&this->popupManager)){return;}

	// タッチ処理
	enginePartsButtonBoxCalc(&this->btnBoxOk);
	enginePartsButtonBoxCalc(&this->btnBoxClose);
	enginePartsButtonCalc(&this->btnOuter);

	// ボタン処理
	if(calcButton(this)){return;}
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeTest2Sample *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// トランスマネージャーリセット
		engineGraphicTransManagerReset(&this->transManager);
		engineGraphicTransSetBuffer(&this->transRoot, &this->stat->egoIdTexSystem, &this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
		// トランス作成
		double ww = 150;
		double wh = 100;
		double wx = ww * -0.5;
		double wy = wh * -0.5;
		engineGraphicImageRectCreateArrayWhite(&this->imgBack, 0, 0, global.screen.w, global.screen.h);
		engineGraphicImageFrameCreateArray(&this->imgFrame, wx, wy, ww, wh);
		engineGraphicImageTextCreateArray(&this->imgTextTitle, wx + ww * 0.5, wy + 10 + 15 * 0.5, "タイトル");
		engineGraphicFontSet(&this->fontTest, PLUGINTEXTUREFONTSETID_DEFAULT, "あい");
		enginePartsButtonBoxCreateArray(&this->btnBoxOk, "ボタン1");
		enginePartsButtonBoxCreateArray(&this->btnBoxClose, "ボタン2");
		enginePartsButtonBoxSetPosition(&this->btnBoxOk, wx + ww * 0.5 - 60 - 5, wy + wh - 40, 60, 30);
		enginePartsButtonBoxSetPosition(&this->btnBoxClose, wx + ww * 0.5 + 5, wy + wh - 40, 60, 30);
		enginePartsButtonSetPosition(&this->btnOuter, (global.screen.w - ww) * 0.5, (global.screen.h - wh) * 0.5, ww, wh);
		// トランス設定
		engineGraphicTransSetTranslate(&this->transWindow, global.screen.w * 0.5, global.screen.h * 0.5, 0);
		engineGraphicTransSetColorRgba(&this->imgBack.trans, 0.0, 0.0, 0.0, 1.0);
		engineGraphicTransSetColorRgba(&this->imgTextTitle.trans, 0.0, 0.0, 0.0, 1.0);
		engineGraphicTransSetColorRgba(&this->fontTest.trans, 1.0, 1.0, 1.0, 1.0);
		// トランスマネージャー登録
		engineGraphicTransManagerPush(&this->transManager, &this->transRoot);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->transBack, &this->transRoot, 0);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->transWindow, &this->transRoot, 0);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->imgBack.trans, &this->transBack, 0);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->imgFrame.trans, &this->transWindow, 1);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->imgTextTitle.trans, &this->transWindow, 2);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->fontTest.trans, &this->transWindow, 2);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->btnBoxOk.trans, &this->transWindow, 2);
		engineGraphicTransManagerPushProperty(&this->transManager, &this->btnBoxClose.trans, &this->transWindow, 2);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeTest2Sample *this){
	// 描画準備
	createBuffer(this);

	// 2D描画準備
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);

	// トランス設定
	engineGraphicTransSetScale(&this->transWindow, this->super.openSize, this->super.openSize, 1.0);
	engineGraphicTransSetColorRgba(&this->transWindow, 1.0, 1.0, 1.0, this->super.openAlpha);
	engineGraphicTransSetColorRgba(&this->transBack, 1.0, 1.0, 1.0, this->super.backAlpha);

	// トランス描画
	struct engineMathVector4 tempCol1;
	engineMathVec4Set(&tempCol1, 1.0, 1.0, 1.0, 1.0);
	engineGraphicTransManagerDraw(&this->transManager, &this->stat->matOrtho, &tempCol1);

	// ポップアップ描画
	engineCartridgePopupManagerDraw(&this->popupManager);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct popupCartridgeTest2Sample *this){
	engineCartridgePopupManagerPause(&this->popupManager);
}

// 破棄
static void dispose(struct popupCartridgeTest2Sample *this){
	engineCartridgePopupManagerDispose(&this->popupManager);
	engineGraphicTransManagerDispose(&this->transManager);
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ポップアップカートリッジ作成
struct engineCartridgePopup *popupTest2SampleCreate(struct pageTest2Status *stat){
	struct popupCartridgeTest2Sample *this = (struct popupCartridgeTest2Sample*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeTest2Sample));
	this->stat = stat;

	struct engineCartridgePopup *cartridge = (struct engineCartridgePopup*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePopup*))init;
	cartridge->calc = (void(*)(struct engineCartridgePopup*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePopup*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePopup*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePopup*))dispose;
	return cartridge;
}

// ポップアップ結果
int popupTest2SampleResult(struct engineCartridgePopup *that){
	struct popupCartridgeTest2Sample *this = (struct popupCartridgeTest2Sample*)that;
	return this->result;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

