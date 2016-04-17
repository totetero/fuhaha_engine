#include "engine/engine.h"
#include "game.h"
#include "platform.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// グローバル変数作成
struct global global = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameSurfaceCreated(){
	e3dGameInit();
}

// opengl画面サイズ変更
void gameSurfaceChanged(int width, int height, double pixelRatio){
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
void gameSurfaceDrawFrame(){
	e3dSampleCode();
}

// opengl中断
void gameSurfacePause(){
}

// opengl終了
void gameSurfaceDestroy(){
	e3dGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

