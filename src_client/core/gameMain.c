#include "library.h"
#include "engine/engineMath/engineMath.h"
#include "engine/engineGraphic/engineGraphic.h"
#include "game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameMainSurfaceCreated(void){
	engineGraphicGameInit();
}

// opengl描画
void gameMainSurfaceDrawFrame(void){
	engineGraphicSampleCode();
}

// opengl中断
void gameMainSurfacePause(void){
}

// opengl終了
void gameMainSurfaceDestroy(void){
	engineGraphicGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

