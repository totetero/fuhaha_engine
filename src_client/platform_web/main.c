#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>
#include <emscripten/emscripten.h>

#include "game.h"
void webPlatformScreenInit(void);
void webPluginUtilInit(void);
void webPluginDataInit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static int init(){
	if(glfwInit() != GL_TRUE){return -1;}
	if(glfwOpenWindow(320, 480, 5, 6, 5, 0, 16, 1, GLFW_WINDOW) != GL_TRUE){return -1;}
    return 0;
}

static void mainloop(){	
	gameSurfaceDrawFrame();
	glfwSwapBuffers();
}

// main関数 処理はここから始まる
int main(){
	if(init() == 0){
		webPluginUtilInit();
		webPluginDataInit();
		gameSurfaceCreated();
		webPlatformScreenInit();
		emscripten_set_main_loop(mainloop, 0, 1);
	}
	glfwTerminate();
	gameSurfacePause();
	gameSurfaceDestroy();
	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

