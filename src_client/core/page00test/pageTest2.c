#include "../library.h"
#include "../define/texpos.h"
#include "../engine/engineMath/engineMath.h"
#include "../engine/engineUtil/engineUtil.h"
#include "../engine/engineCtrl/engineCtrl.h"
#include "../engine/engineGraphic/engineGraphic.h"
#include "../engine/engineCartridge/engineCartridge.h"
#include "../engine/engineLayout01/engineLayout01.h"
#include "../game.h"
#include "pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest2{
	struct engineCartridgePage super;
	struct engineCartridgePopupManager popupManager;
	struct pageTest2Status *stat;

	struct engineLayout01TransManager transManager;
	struct engineLayout01Trans transRoot;
	struct engineLayout01Trans transBox;
	struct engineLayout01ImageRect imgRectTest;
	struct engineLayout01ImageFrame imgFrameTest;
	struct engineLayout01ImageText imgTextTest;
	struct engineLayout01ImageNumber imgNumberTest;
	struct engineLayout01ButtonBox btnBoxTest;
	struct engineLayout01ButtonPicker btnPickerTest;

	int bufferStatus;
	int bufferScreen;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;

	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest2 *this){
	engineCartridgePopupManagerInit(&this->popupManager);
	engineLayout01TransInit(&this->transRoot);
	engineLayout01TransInit(&this->transBox);
	engineLayout01ImageRectInit(&this->imgRectTest);
	engineLayout01ImageFrameInit(&this->imgFrameTest);
	engineLayout01ImageTextInit(&this->imgTextTest);
	engineLayout01ImageNumberInit(&this->imgNumberTest);
	engineLayout01ButtonBoxInit(&this->btnBoxTest);
	engineLayout01ButtonPickerInit(&this->btnPickerTest, 0);

	// 画像読み込み
	this->stat->egoIdTexSystem = engineGraphicObjectTexCreateLocal("img/system.png", ENGINEGRAPHICOBJECTTEXTYPE_LINEAR);

	this->step = 0;
}

// ----------------------------------------------------------------

static void openPopupSample(struct pageCartridgeTest2 *this);

// サンプルポップアップ完了時
static void onClosePopupSample(struct engineCartridgePopup *cartridge){
	int result = popupTest2SampleResult(cartridge);
	trace("popup %d", result);
	// test もう一回ポップアップ
	if(result == 1){openPopupSample(cartridge->param);}
}

// サンプルポップアップ展開
static void openPopupSample(struct pageCartridgeTest2 *this){
	struct engineCartridgePopup *cartridge = popupTest2SampleCreate(this->stat);
	cartridge->param = this;
	cartridge->onClose = onClosePopupSample;
	engineCartridgePopupManagerPush(&this->popupManager, cartridge);
}

// ----------------------------------------------------------------

// タッチ処理の計算
static void calcTouch(struct pageCartridgeTest2 *this){
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_TEST1);
	if(t != NULL){
		if(t->dn){
			if(!t->active){
				// タッチ開始時
				engineCtrlTouchOwn();
			}

			// テスト タッチ位置確認
			trace("myon %d %d", t->screen.x, t->screen.y);
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}

// ボタン計算
static bool calcButton(struct pageCartridgeTest2 *this){
	// テストボタン押下確認
	if(this->btnBoxTest.super.trigger){
		this->btnBoxTest.super.trigger = false;
		// ポップアップ展開
		openPopupSample(this);
		return true;
	}

	// 戻るボタン押下確認
	if(global.key.bk.triggerInactive){
		global.key.bk.triggerInactive = false;
		// 終了
		platformMainSurfaceExit();
	}

	return false;
}

// 計算
static void calc(struct pageCartridgeTest2 *this){
	// ポップアップ処理
	if(engineCartridgePopupManagerCalc(&this->popupManager)){return;}

	// タッチ処理
	engineLayout01ButtonBoxCalc(&this->btnBoxTest);
	engineLayout01ButtonPickerCalc(&this->btnPickerTest);
	calcTouch(this);

	// ボタン処理
	if(calcButton(this)){return;}

	this->step++;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeTest2 *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// トランスマネージャーリセット
		engineLayout01TransManagerReset(&this->transManager);
		engineLayout01TransSetBuffer(&this->transRoot, &this->stat->egoIdTexSystem, &this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
		// トランス作成
		double ww = 80;
		double wh = 80;
		double wx = ww * -0.5;
		double wy = wh * -0.5;
		char *textList[] = {"あいう", "エオカ", "abc"};
		engineLayout01ImageRectCreateArray(&this->imgRectTest, TEXSIZ_SYSTEM_WH, 0, 0, 320, 180, TEXPOS_SYSTEM_FONTABCD_XYWH);
		engineLayout01ImageFrameCreateArray(&this->imgFrameTest, wx, wy, ww, wh);
		engineLayout01ImageTextCreateArray(&this->imgTextTest, wx + ww * 0.5, wy + wh * 0.5 - 10, "ボックス\nみょん");
		engineLayout01ImageNumberCreateArray(&this->imgNumberTest, wx + ww * 0.5, wy + wh * 0.5 + 20, 100);
		engineLayout01ButtonBoxCreateArray(&this->btnBoxTest, "ポップアップ");
		engineLayout01ButtonBoxSetPosition(&this->btnBoxTest, (global.screen.w - 120) * 0.5, global.screen.h - 40, 120, 30);
		engineLayout01ButtonPickerCreateArray(&this->btnPickerTest, textList, 3);
		engineLayout01ButtonPickerSetPosition(&this->btnPickerTest, 10, 10, 100);
		// トランス設定
		engineLayout01TransSetTranslate(&this->transBox, global.screen.w * 0.5, 80, 0);
		engineLayout01TransSetColorRgba(&this->imgTextTest.trans, 0.0, 0.0, 0.0, 1.0);
		engineLayout01TransSetColorRgba(&this->imgNumberTest.trans, 0.0, 0.0, 0.0, 1.0);
		// トランスマネージャー登録
		engineLayout01TransManagerPush(&this->transManager, &this->transRoot);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->transBox, &this->transRoot, 0);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->imgRectTest.trans, &this->transRoot, 0);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->imgFrameTest.trans, &this->transBox, 0);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->imgTextTest.trans, &this->transBox, 0);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->imgNumberTest.trans, &this->transBox, 0);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->btnBoxTest.trans, &this->transRoot, 0);
		engineLayout01TransManagerPushProperty(&this->transManager, &this->btnPickerTest.trans, &this->transRoot, 0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeTest2 *this){
	// 描画準備
	createBuffer(this);

	// 射影行列作成
	engineMathMat4Ortho(&this->stat->matOrtho, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);

	// 2D描画準備
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);

	// トランス設定
	engineLayout01TransSetRotate(&this->transBox, 0.0, 0.0, ENGINEMATH_DEG2RAD * this->step);

	// トランス描画
	struct engineMathVector4 tempCol1;
	engineMathVec4Set(&tempCol1, 1.0, 1.0, 1.0, 1.0);
	engineLayout01TransManagerDraw(&this->transManager, &this->stat->matOrtho, &tempCol1);

	// ポップアップ描画
	engineCartridgePopupManagerDraw(&this->popupManager);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest2 *this){
	engineCartridgePopupManagerPause(&this->popupManager);
}

// 破棄
static void dispose(struct pageCartridgeTest2 *this){
	engineCartridgePopupManagerDispose(&this->popupManager);
	engineLayout01TransManagerDispose(&this->transManager);
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicObjectTexDispose(this->stat->egoIdTexSystem);
	engineUtilMemoryFree(this->stat);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
static struct engineCartridgePage *pageTest2Create(struct pageTest2Status *stat){
	struct pageCartridgeTest2 *this = (struct pageCartridgeTest2*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTest2));
	this->stat = stat;

	struct engineCartridgePage *cartridge = (struct engineCartridgePage*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePage*))init;
	cartridge->calc = (void(*)(struct engineCartridgePage*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePage*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePage*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePage*))dispose;
	return cartridge;
}

// ページ状態初期化
void pageTest2PushPage(){
	struct pageTest2Status *stat = (struct pageTest2Status*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2Status));
	engineCartridgePageManagerPush(pageTest2Create(stat));
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

