#include "../library.h"
#include "../engine/engineMath/engineMath.h"
#include "../engine/engineUtil/engineUtil.h"
#include "../engine/engineGraphic/engineGraphic.h"
#include "../engine/engineCartridge/engineCartridge.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest1{
	struct engineCartridgePage super;

	struct pageCartridgeTest1BufferCompare{
		int sw;
		int sh;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexTest;

	int faceIndex;
	int faceNum;

	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest1 *this){
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal("test.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);

	this->step = 0;
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageCartridgeTest1 *this){
	this->step++;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeTest1 *this){
	struct pageCartridgeTest1BufferCompare bufferCompare;
	bufferCompare.sw = global.screen.w;
	bufferCompare.sh = global.screen.h;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct pageCartridgeTest1BufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct pageCartridgeTest1BufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		int vertIndex = engineGraphicBufferVretIndexGet();
		int faceIndex = engineGraphicBufferFaceIndexGet();
		int faceNum = 0;
		engineGraphicBufferPushVert(-1.0, -1.0, 0.0);
		engineGraphicBufferPushVert(-1.0,  1.0, 0.0);
		engineGraphicBufferPushVert( 1.0,  1.0, 0.0);
		engineGraphicBufferPushVert( 1.0, -1.0, 0.0);
		engineGraphicBufferPushTexc(0.0, 0.0);
		engineGraphicBufferPushTexc(0.0, 1.0);
		engineGraphicBufferPushTexc(1.0, 1.0);
		engineGraphicBufferPushTexc(1.0, 0.0);
		engineGraphicBufferPushFace(vertIndex, 0, 1, 2); faceNum++;
		engineGraphicBufferPushFace(vertIndex, 2, 3, 0); faceNum++;
		this->faceIndex = faceIndex;
		this->faceNum = faceNum;

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeTest1 *this){
	engineGraphicEngineClearAll();

	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempMat1;
	engineMathMat4Frustum(&tempMat1, -2.0, 2.0, -3.0, 3.0, 2.0, 1000.0);
	engineMathMat4Translate(&tempMat1, 0.0, 0.0, -3.0);
	engineMathMat4RotateX(&tempMat1, ENGINEMATH_DEG2RAD * this->step);

	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindTexture(this->egoIdTexTest);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	engineGraphicEngineSetMatrix(&tempMat1);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);

	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest1 *this){
}

// 破棄
static void dispose(struct pageCartridgeTest1 *this){
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
static struct engineCartridgePage *pageTest1Create(){
	struct pageCartridgeTest1 *this = (struct pageCartridgeTest1*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTest1));

	struct engineCartridgePage *cartridge = (struct engineCartridgePage*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePage*))init;
	cartridge->calc = (void(*)(struct engineCartridgePage*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePage*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePage*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePage*))dispose;
	return cartridge;
}

// ページ状態初期化
void pageTest1PushPage(){
	engineCartridgePageManagerPush(pageTest1Create());
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

