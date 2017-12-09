#include "../core/library.h"
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "../core/game.h"

void webFuhahaSurfaceInit(void);
void webPluginUtilInit(void);
void webPluginTextureInit(void);
void webPluginSoundInit(void);
void webPluginEnptyInit(void);
void webFuhahaEventInit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static int init(void){
	if(glfwInit() != GL_TRUE){return -1;}
	if(glfwOpenWindow(320, 480, 5, 6, 5, 0, 16, 1, GLFW_WINDOW) != GL_TRUE){return -1;}
	return 0;
}

static void mainloop(void){	
	gameMainSurfaceDrawFrame();
	glfwSwapBuffers();
}

// main関数 処理はここから始まる
int main(void){
	if(init() == 0){
		webFuhahaSurfaceInit();
		webPluginUtilInit();
		webPluginTextureInit();
		webPluginSoundInit();
		webPluginEnptyInit();
		gameMainSurfaceCreated();
		gameMainEventInit();
		webFuhahaEventInit();
		emscripten_set_main_loop(mainloop, 0, 1);
	}
	glfwTerminate();
	gameMainSurfacePause();
	gameMainSurfaceDestroy();
	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

