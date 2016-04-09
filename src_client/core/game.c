#include "platform.h"

#include "game.h"
#include "engine/engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// グローバル変数作成
struct global global = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

void gameSurfaceCreated(){
	e3dGameInit();
}

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

void gameSurfaceDrawFrame(){
	e3dSampleCode();
}

void gameSurfacePause(){
}

void gameSurfaceDestroy(){
	e3dGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

