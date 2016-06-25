#include "../../library.h"
#include "../engineMath/engineMath.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3Dエンジン初期化処理
void engineGraphicGameInit(void){
	engineGraphicEngineInit();
	engineGraphicObjectReload();
}

// 3Dエンジン終了処理
void engineGraphicGameExit(void){
	engineGraphicBufferClean();
	engineGraphicObjectDispose();
	engineGraphicEngineExit();
}

// 3Dエンジン例文
void engineGraphicSampleCode(void){
	static bool isInit = false;
	static engineGraphicObjectTexId egoIdImage = 0;
	static engineGraphicObjectVBOId egoIdVert = 0;
	static engineGraphicObjectVBOId egoIdClor = 0;
	static engineGraphicObjectVBOId egoIdTexc = 0;
	static engineGraphicObjectIBOId egoIdFace = 0;
	static int faceIndex = 0;
	static int faceNum = 0;
	static int step = 0;
	if(!isInit){
		isInit = true;
		int vertIndex = engineGraphicBufferVretIndexGet();
		faceIndex = engineGraphicBufferFaceIndexGet();
		engineGraphicBufferBegin();
		engineGraphicBufferPushVert(-1.0, -1.0, 0.0);
		engineGraphicBufferPushVert(-1.0,  1.0, 0.0);
		engineGraphicBufferPushVert( 1.0,  1.0, 0.0);
		engineGraphicBufferPushVert( 1.0, -1.0, 0.0);
		engineGraphicBufferPushClor(1.0, 1.0, 1.0);
		engineGraphicBufferPushClor(0.0, 1.0, 1.0);
		engineGraphicBufferPushClor(1.0, 0.0, 1.0);
		engineGraphicBufferPushClor(1.0, 1.0, 0.0);
		engineGraphicBufferPushTexc(0.0, 0.0);
		engineGraphicBufferPushTexc(0.0, 1.0);
		engineGraphicBufferPushTexc(1.0, 1.0);
		engineGraphicBufferPushTexc(1.0, 0.0);
		engineGraphicBufferPushFace(vertIndex, 0, 1, 2); faceNum++;
		engineGraphicBufferPushFace(vertIndex, 2, 3, 0); faceNum++;
		engineGraphicBufferEnd(&egoIdVert, &egoIdClor, &egoIdTexc, &egoIdFace);
		egoIdImage = engineGraphicObjectTexCreate("test.png", ENGINEGRAPHICOBJECTTEXTYPE_NEAREST);
	}
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Frustum(&tempMat1, -2.0, 2.0, -3.0, 3.0, 2.0, 1000.0);
	engineMathMat4Translate(&tempMat1, 0.0, 0.0, -3.0);
	engineMathMat4RotateX(&tempMat1, step++ * 0.1);
	engineGraphicEngineClearAll();
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindTexture(egoIdImage);
	engineGraphicEngineBindVertVBO(egoIdVert);
	//engineGraphicEngineBindClorVBO(egoIdClor);
	engineGraphicEngineBindTexcVBO(egoIdTexc);
	engineGraphicEngineBindFaceIBO(egoIdFace);
	engineGraphicEngineSetMatrix(&tempMat1);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	engineGraphicEngineDrawIndex(faceIndex * 3, faceNum * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

