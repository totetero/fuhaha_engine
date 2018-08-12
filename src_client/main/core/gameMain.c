#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 各ページカートリッジ装填のコンストラクタ
void pageTest1PushPage(void);
void pageTest2PushPage(void);

// 最初のページカートリッジ作成
void createFirstCartridge(void){
	pageTest2PushPage();
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
	engineUtilFrameCountCalc();
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

