#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3Dエンジン初期化処理
void e3dGameInit(){
	e3dEngineInit();
	e3dObjectReload();
}

// 3Dエンジン終了処理
void e3dGameExit(){
	e3dBufferClean();
	e3dObjectDispose();
	e3dEngineExit();
}

// 3Dエンジン例文
void e3dSampleCode(){
	static bool isInit = false;
	static e3dObjectTexId e3dIdImage = 0;
	static e3dObjectVBOId e3dIdVert = 0;
	static e3dObjectVBOId e3dIdClor = 0;
	static e3dObjectVBOId e3dIdTexc = 0;
	static e3dObjectIBOId e3dIdFace = 0;
	static uint32_t faceIndex = 0;
	static uint32_t faceNum = 0;
	static uint32_t step = 0;
	if(!isInit){
		isInit = true;
		uint32_t vertIndex = e3dBufferVretIndexGet();
		faceIndex = e3dBufferFaceIndexGet();
		e3dBufferBegin();
		e3dBufferPushVert(-1.0, -1.0, 0.0);
		e3dBufferPushVert(-1.0,  1.0, 0.0);
		e3dBufferPushVert( 1.0,  1.0, 0.0);
		e3dBufferPushVert( 1.0, -1.0, 0.0);
		e3dBufferPushClor(1.0, 1.0, 1.0);
		e3dBufferPushClor(0.0, 1.0, 1.0);
		e3dBufferPushClor(1.0, 0.0, 1.0);
		e3dBufferPushClor(1.0, 1.0, 0.0);
		e3dBufferPushTexc(0.0, 0.0);
		e3dBufferPushTexc(0.0, 1.0);
		e3dBufferPushTexc(1.0, 1.0);
		e3dBufferPushTexc(1.0, 0.0);
		e3dBufferPushFace(vertIndex, 0, 1, 2); faceNum++;
		e3dBufferPushFace(vertIndex, 2, 3, 0); faceNum++;
		e3dBufferEnd(&e3dIdVert, &e3dIdClor, &e3dIdTexc, &e3dIdFace);
		e3dIdImage = e3dObjectTexCreate("test.png", E3DTEXTYPE_NEAREST);
	}
	struct mathMatrix44 tempMat1;
	mathMat4Frustum(&tempMat1, -2.0, 2.0, -3.0, 3.0, 2.0, 1000.0);
	mathMat4Translate(&tempMat1, 0.0, 0.0, -3.0);
	mathMat4RotateX(&tempMat1, step++ * 0.1);
	e3dClearAll();
	e3dSetDrawMode(E3DMODEDRAW_2D);
	e3dBindTexture(e3dIdImage);
	e3dBindVertVBO(e3dIdVert);
	//e3dBindClorVBO(e3dIdClor);
	e3dBindTexcVBO(e3dIdTexc);
	e3dBindFaceIBO(e3dIdFace);
	e3dSetMatrix(&tempMat1);
	e3dSetColor(1.0, 1.0, 1.0, 1.0);
	e3dDrawIndex(faceIndex * 3, faceNum * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

