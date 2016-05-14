#include "library.h"
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "game.h"

void webPlatformScreenInit(void);
void webPluginUtilInit(void);
void webPluginDataInit(void);
void webPluginTextureInit(void);
void webPluginControllerInit(void);
void webPluginSoundInit(void);
void webPluginEnptyInit(void);

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
		webPluginTextureInit();
		webPluginControllerInit();
		webPluginSoundInit();
		webPluginEnptyInit();
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

