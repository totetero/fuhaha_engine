#include "library.h"
#include "platform.h"
#include "plugin/pluginData.h"
#include "engine/engineMath/engineMath.h"
#include "engine/engineGraphic/engineGraphic.h"
#include "game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// グローバル変数作成
struct global global = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameSurfaceCreated(void){
	engineGraphicGameInit();
}

// opengl画面サイズ変更
void gameSurfaceChanged(uint16_t width, uint16_t height, double pixelRatio){
	global.ctrl.ww = width;
	global.ctrl.wh = height;
	glViewport(0, 0, width * pixelRatio, height * pixelRatio);
	if(true){
		global.ctrl.sw = 320;
		global.ctrl.sh = global.ctrl.sw * global.ctrl.wh / global.ctrl.ww;
		if(global.ctrl.sh < global.ctrl.sw){global.ctrl.sh = global.ctrl.sw;}
	}else{
		global.ctrl.sw = width;
		global.ctrl.sh = height;
	}
}

// opengl描画
void gameSurfaceDrawFrame(void){
	engineGraphicSampleCode();
}

// opengl中断
void gameSurfacePause(void){
}

// opengl終了
void gameSurfaceDestroy(void){
	engineGraphicGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

