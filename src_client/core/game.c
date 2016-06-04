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
	global.window.w = width;
	global.window.h = height;
	glViewport(0, 0, width * pixelRatio, height * pixelRatio);
	if(false){
		// スクリーンサイズ固定 
		global.screen.w = 320;
		global.screen.h = 480;
	}else if(true){
		// スクリーン横幅固定
		global.screen.w = 320;
		global.screen.h = global.screen.w * global.window.h / global.window.w;
		if(global.screen.h < global.screen.w){global.screen.h = global.screen.w;}
	}else{
		// スクリーンサイズをウインドウサイズに合わせる
		global.screen.w = width;
		global.screen.h = height;
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

