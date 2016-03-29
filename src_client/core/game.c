#include "platform.h"

#include "game.h"
#include "engine/engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

void gameSurfaceCreated(){
	e3dGameInit();
}

void gameSurfaceChanged(int width, int height, double pixelRatio){
}

void gameSurfaceDrawFrame(){
	e3dSampleCode();
}

void gameSurfacePause(){
}

void gameSurfaceDestroy(){
	e3dGameFinal();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

