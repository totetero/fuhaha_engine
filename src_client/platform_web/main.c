#include "library.h"
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "game.h"

void webPluginUtilInit(void);
void webPluginSoundInit(void);
void webPluginEnptyInit(void);
void webFuhahaSurfaceInit(void);
void webFuhahaCtrlInit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static int init(void){
	if(glfwInit() != GL_TRUE){return -1;}
	if(glfwOpenWindow(320, 480, 5, 6, 5, 0, 16, 1, GLFW_WINDOW) != GL_TRUE){return -1;}
    return 0;
}

static void mainloop(void){	
	gameSurfaceDrawFrame();
	glfwSwapBuffers();
}

// main関数 処理はここから始まる
int main(void){
	if(init() == 0){
		webPluginUtilInit();
		webPluginSoundInit();
		webPluginEnptyInit();
		gameSurfaceCreated();
		webFuhahaSurfaceInit();
		webFuhahaCtrlInit();
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

