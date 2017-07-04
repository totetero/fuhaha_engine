#include "../library.h"
#include "../engine/engineMath/engineMath.h"
#include "../engine/engineUtil/engineUtil.h"
#include "../engine/engineGraphic/engineGraphic.h"
#include "../engine/engineCartridge/engineCartridge.h"
#include "../engine/engineLayout02/engineLayout02.h"
#include "../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest3{
	struct engineCartridgePage super;

	struct engineLayout02View *viewRoot;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest3 *this){
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageCartridgeTest3 *this){
	// ビュー処理
	engineLayout02ViewUtilPositionModeSetCalc();
	if(this->viewRoot == NULL){
		// ビュー作成
		this->viewRoot = engineLayout02ViewCreate();
		engineLayout02ViewUtilPositionSetLeft(this->viewRoot, 0);
		engineLayout02ViewUtilPositionSetRight(this->viewRoot, 0);
		engineLayout02ViewUtilPositionSetTop(this->viewRoot, 0);
		engineLayout02ViewUtilPositionSetBottom(this->viewRoot, 0);

		struct engineLayout02ViewPartsFrame *viewTest = engineLayout02ViewPartsFrameCreate();
		engineLayout02ViewUtilPositionSetLeft((struct engineLayout02View*)viewTest, 10);
		engineLayout02ViewUtilPositionSetRight((struct engineLayout02View*)viewTest, 10);
		engineLayout02ViewUtilPositionSetTop((struct engineLayout02View*)viewTest, 10);
		engineLayout02ViewUtilPositionSetBottom((struct engineLayout02View*)viewTest, 10);
		engineLayout02ViewUtilChildrenAdd(this->viewRoot, (struct engineLayout02View*)viewTest);
	}
	engineLayout02ViewCalc((struct engineLayout02View*)this->viewRoot);
	engineLayout02ViewUtilPositionModeSetDraw();
}

// ----------------------------------------------------------------

// 描画
static void draw(struct pageCartridgeTest3 *this){
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Ortho(&tempMat1, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);

	struct engineMathVector4 tempColor1;
	tempColor1.r = 1.0;
	tempColor1.g = 1.0;
	tempColor1.b = 1.0;
	tempColor1.a = 1.0;

	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);

	engineLayout02ViewDraw((struct engineLayout02View*)this->viewRoot, &tempMat1, &tempColor1);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest3 *this){
	engineLayout02ViewPause((struct engineLayout02View*)this->viewRoot);
}

// 破棄
static void dispose(struct pageCartridgeTest3 *this){
	engineLayout02ViewDispose((struct engineLayout02View*)this->viewRoot);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
static struct engineCartridgePage *pageTest3Create(){
	struct pageCartridgeTest3 *this = (struct pageCartridgeTest3*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTest3));

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
void pageTest3PushPage(){
	engineCartridgePageManagerPush(pageTest3Create());
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

