#include "../../library.h"
#include "platform.h"
#include "../engineMath/engineMath.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	double *buffVert;
	double *buffClor;
	double *buffTexc;
	uint16_t *buffFace;
	uint32_t buffVertIndex;
	uint32_t buffClorIndex;
	uint32_t buffTexcIndex;
	uint32_t buffFaceIndex;
	uint32_t buffVertLength;
	uint32_t buffClorLength;
	uint32_t buffTexcLength;
	uint32_t buffFaceLength;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バッファ作成開始
void engineGraphicBufferBegin(){
	localGlobal.buffVertIndex = 0;
	localGlobal.buffClorIndex = 0;
	localGlobal.buffTexcIndex = 0;
	localGlobal.buffFaceIndex = 0;
}

// バッファ作成完了
void engineGraphicBufferEnd(engineGraphicObjectVBOId *egoIdVert, engineGraphicObjectVBOId *egoIdClor, engineGraphicObjectVBOId *egoIdTexc, engineGraphicObjectIBOId *egoIdFace){
	if(egoIdVert != NULL){engineGraphicObjectVBODispose(*egoIdVert); *egoIdVert = engineGraphicObjectVBOCreate(localGlobal.buffVertIndex, localGlobal.buffVert);}
	if(egoIdClor != NULL){engineGraphicObjectVBODispose(*egoIdClor); *egoIdClor = engineGraphicObjectVBOCreate(localGlobal.buffClorIndex, localGlobal.buffClor);}
	if(egoIdTexc != NULL){engineGraphicObjectVBODispose(*egoIdTexc); *egoIdTexc = engineGraphicObjectVBOCreate(localGlobal.buffTexcIndex, localGlobal.buffTexc);}
	if(egoIdFace != NULL){engineGraphicObjectIBODispose(*egoIdFace); *egoIdFace = engineGraphicObjectIBOCreate(localGlobal.buffFaceIndex, localGlobal.buffFace);}
}

// ----------------------------------------------------------------

// 頂点座標配列に要素追加
void engineGraphicBufferPushVert(double x, double y, double z){
	if(localGlobal.buffVertLength < localGlobal.buffVertIndex + 3){
		uint32_t length = localGlobal.buffVertLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.buffVertLength > 0){
			memcpy(array, localGlobal.buffVert, localGlobal.buffVertLength * sizeof(double));
			free(localGlobal.buffVert);
		}
		localGlobal.buffVertLength = length;
		localGlobal.buffVert = array;
	}
	*(localGlobal.buffVert + localGlobal.buffVertIndex++) = x;
	*(localGlobal.buffVert + localGlobal.buffVertIndex++) = y;
	*(localGlobal.buffVert + localGlobal.buffVertIndex++) = z;
}

// 色彩配列に要素追加
void engineGraphicBufferPushClor(double r, double g, double b){
	if(localGlobal.buffClorLength < localGlobal.buffClorIndex + 3){
		uint32_t length = localGlobal.buffClorLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.buffClorLength > 0){
			memcpy(array, localGlobal.buffClor, localGlobal.buffClorLength * sizeof(double));
			free(localGlobal.buffClor);
		}
		localGlobal.buffClorLength = length;
		localGlobal.buffClor = array;
	}
	*(localGlobal.buffClor + localGlobal.buffClorIndex++) = r;
	*(localGlobal.buffClor + localGlobal.buffClorIndex++) = g;
	*(localGlobal.buffClor + localGlobal.buffClorIndex++) = b;
}

// テクスチャ座標配列に要素追加
void engineGraphicBufferPushTexc(double u, double v){
	if(localGlobal.buffTexcLength < localGlobal.buffTexcIndex + 2){
		uint32_t length = localGlobal.buffTexcLength + 200;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.buffTexcLength > 0){
			memcpy(array, localGlobal.buffTexc, localGlobal.buffTexcLength * sizeof(double));
			free(localGlobal.buffTexc);
		}
		localGlobal.buffTexcLength = length;
		localGlobal.buffTexc = array;
	}
	*(localGlobal.buffTexc + localGlobal.buffTexcIndex++) = u;
	*(localGlobal.buffTexc + localGlobal.buffTexcIndex++) = v;
}

// 頂点番号配配列に要素追加
void engineGraphicBufferPushFace(uint16_t index, uint16_t t0, uint16_t t1, uint16_t t2){
	if(localGlobal.buffFaceLength < localGlobal.buffFaceIndex + 3){
		uint32_t length = localGlobal.buffFaceLength + 150;
		uint16_t *array = (uint16_t*)malloc(length * sizeof(uint16_t));
		if(localGlobal.buffFaceLength > 0){
			memcpy(array, localGlobal.buffFace, localGlobal.buffFaceLength * sizeof(uint16_t));
			free(localGlobal.buffFace);
		}
		localGlobal.buffFaceLength = length;
		localGlobal.buffFace = array;
	}
	*(localGlobal.buffFace + localGlobal.buffFaceIndex++) = index + t0;
	*(localGlobal.buffFace + localGlobal.buffFaceIndex++) = index + t1;
	*(localGlobal.buffFace + localGlobal.buffFaceIndex++) = index + t2;
}

// ----------------------------------------------------------------

// 頂点座標配列に四角形の要素追加
void engineGraphicBufferPushTetraVert(double x, double y, double w, double h){
	engineGraphicBufferPushVert(x + 0, y + 0, 0.0);
	engineGraphicBufferPushVert(x + 0, y + h, 0.0);
	engineGraphicBufferPushVert(x + w, y + h, 0.0);
	engineGraphicBufferPushVert(x + w, y + 0, 0.0);
}

// テクスチャ座標配列に要素追加
void engineGraphicBufferPushTetraTexc(uint16_t imgw, uint16_t imgh, double u, double v, double w, double h){
	engineGraphicBufferPushTexc((u + 0) / imgw, (v + 0) / imgh);
	engineGraphicBufferPushTexc((u + 0) / imgw, (v + h) / imgh);
	engineGraphicBufferPushTexc((u + w) / imgw, (v + h) / imgh);
	engineGraphicBufferPushTexc((u + w) / imgw, (v + 0) / imgh);
}

// 頂点番号配配列に要素追加
void engineGraphicBufferPushTetraFace(uint16_t index){
	engineGraphicBufferPushFace(index, 0, 1, 2);
	engineGraphicBufferPushFace(index, 2, 3, 0);
}

// ----------------------------------------------------------------

// VBOバッファ配列内の位置獲得
uint32_t engineGraphicBufferVretIndexGet(){
	return localGlobal.buffVertIndex / 3;
}

// IBOバッファ配列内の位置獲得
uint32_t engineGraphicBufferFaceIndexGet(){
	return localGlobal.buffFaceIndex / 3;
}

// ----------------------------------------------------------------

// バッファ片付け
void engineGraphicBufferClean(){
	free(localGlobal.buffVert);
	free(localGlobal.buffClor);
	free(localGlobal.buffTexc);
	free(localGlobal.buffFace);
	localGlobal.buffVertLength = 0;
	localGlobal.buffClorLength = 0;
	localGlobal.buffTexcLength = 0;
	localGlobal.buffFaceLength = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

