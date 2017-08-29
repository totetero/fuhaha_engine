#include "library.h"
#include "engine/engineCtrl/engineCtrl.h"
#include "engine/engineGraphic/engineGraphic.h"
#include "engine/engineCartridge/engineCartridge.h"
#include "game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameMainSurfaceCreated(void){
	engineGraphicGameInit();
	engineCartridgePageManagerInit();
}

// opengl描画
void gameMainSurfaceDrawFrame(void){
	// 計算
	engineCtrlTouchCalc();
	engineCartridgePageManagerCalc();
	// 描画
	engineCartridgePageManagerDraw();
}

// opengl中断
void gameMainSurfacePause(void){
	engineCartridgePageManagerPause();
}

// opengl終了
void gameMainSurfaceDestroy(void){
	engineCartridgePageManagerDispose();
	engineGraphicGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

