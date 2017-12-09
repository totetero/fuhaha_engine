#include "library.h"
#include "engine/engineCtrl/engineCtrl.h"
#include "engine/engineGraphic/engineGraphic.h"
#include "engine/engineCartridge/engineCartridge.h"
#include "game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 各ページカートリッジ装填のコンストラクタ
void pageTest1PushPage(void);
void pageTest2PushPage(void);
void pageTest3PushPage(void);

// 最初のページカートリッジ作成
void createFirstCartridge(void){
	pageTest1PushPage();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameMainSurfaceCreated(void){
	engineGraphicGameInit();
	engineCartridgePageManagerInit(createFirstCartridge);
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

// 戻るボタンの制御を行うか
bool gameMainEventKeyIsBack(void){return false;}

// 傾きを使うかどうか
bool gameMainEventIsAcceleration(void){return false;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

