#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static double *e3dBufferVert;
static double *e3dBufferClor;
static double *e3dBufferTexc;
static uint16_t *e3dBufferFace;
static uint32_t e3dBufferVertIndex;
static uint32_t e3dBufferClorIndex;
static uint32_t e3dBufferTexcIndex;
static uint32_t e3dBufferFaceIndex;
static uint32_t e3dBufferVertLength = 0;
static uint32_t e3dBufferClorLength = 0;
static uint32_t e3dBufferTexcLength = 0;
static uint32_t e3dBufferFaceLength = 0;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バッファ作成開始
void e3dBufferBegin(){
	e3dBufferVertIndex = 0;
	e3dBufferClorIndex = 0;
	e3dBufferTexcIndex = 0;
	e3dBufferFaceIndex = 0;
}

// バッファ作成完了
void e3dBufferEnd(e3dObjectVBOId *e3dIdVert, e3dObjectVBOId *e3dIdClor, e3dObjectVBOId *e3dIdTexc, e3dObjectIBOId *e3dIdFace){
	if(e3dIdVert != NULL){e3dObjectVBODispose(*e3dIdVert); *e3dIdVert = e3dObjectVBOCreate(e3dBufferVertIndex, e3dBufferVert);}
	if(e3dIdClor != NULL){e3dObjectVBODispose(*e3dIdClor); *e3dIdClor = e3dObjectVBOCreate(e3dBufferClorIndex, e3dBufferClor);}
	if(e3dIdTexc != NULL){e3dObjectVBODispose(*e3dIdTexc); *e3dIdTexc = e3dObjectVBOCreate(e3dBufferTexcIndex, e3dBufferTexc);}
	if(e3dIdFace != NULL){e3dObjectIBODispose(*e3dIdFace); *e3dIdFace = e3dObjectIBOCreate(e3dBufferFaceIndex, e3dBufferFace);}
}

// ----------------------------------------------------------------

// 頂点座標配列に要素追加
void e3dBufferPushVert(double x, double y, double z){
	if(e3dBufferVertLength < e3dBufferVertIndex + 3){
		uint32_t length = e3dBufferVertLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(e3dBufferVertLength > 0){
			memcpy(array, e3dBufferVert, e3dBufferVertLength * sizeof(double));
			free(e3dBufferVert);
		}
		e3dBufferVertLength = length;
		e3dBufferVert = array;
	}
	*(e3dBufferVert + e3dBufferVertIndex++) = x;
	*(e3dBufferVert + e3dBufferVertIndex++) = y;
	*(e3dBufferVert + e3dBufferVertIndex++) = z;
}

// 色彩配列に要素追加
void e3dBufferPushClor(double r, double g, double b){
	if(e3dBufferClorLength < e3dBufferClorIndex + 3){
		uint32_t length = e3dBufferClorLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(e3dBufferClorLength > 0){
			memcpy(array, e3dBufferClor, e3dBufferClorLength * sizeof(double));
			free(e3dBufferClor);
		}
		e3dBufferClorLength = length;
		e3dBufferClor = array;
	}
	*(e3dBufferClor + e3dBufferClorIndex++) = r;
	*(e3dBufferClor + e3dBufferClorIndex++) = g;
	*(e3dBufferClor + e3dBufferClorIndex++) = b;
}

// テクスチャ座標配列に要素追加
void e3dBufferPushTexc(double u, double v){
	if(e3dBufferTexcLength < e3dBufferTexcIndex + 2){
		uint32_t length = e3dBufferTexcLength + 200;
		double *array = (double*)malloc(length * sizeof(double));
		if(e3dBufferTexcLength > 0){
			memcpy(array, e3dBufferTexc, e3dBufferTexcLength * sizeof(double));
			free(e3dBufferTexc);
		}
		e3dBufferTexcLength = length;
		e3dBufferTexc = array;
	}
	*(e3dBufferTexc + e3dBufferTexcIndex++) = u;
	*(e3dBufferTexc + e3dBufferTexcIndex++) = v;
}

// 頂点番号配配列に要素追加
void e3dBufferPushFace(uint16_t index, uint16_t t0, uint16_t t1, uint16_t t2){
	if(e3dBufferFaceLength < e3dBufferFaceIndex + 3){
		uint32_t length = e3dBufferFaceLength + 150;
		uint16_t *array = (uint16_t*)malloc(length * sizeof(uint16_t));
		if(e3dBufferFaceLength > 0){
			memcpy(array, e3dBufferFace, e3dBufferFaceLength * sizeof(uint16_t));
			free(e3dBufferFace);
		}
		e3dBufferFaceLength = length;
		e3dBufferFace = array;
	}
	*(e3dBufferFace + e3dBufferFaceIndex++) = index + t0;
	*(e3dBufferFace + e3dBufferFaceIndex++) = index + t1;
	*(e3dBufferFace + e3dBufferFaceIndex++) = index + t2;
}

// ----------------------------------------------------------------

// 頂点座標配列に四角形の要素追加
void e3dBufferPushTetraVert(double x, double y, double w, double h){
	e3dBufferPushVert(x + 0, y + 0, 0.0);
	e3dBufferPushVert(x + 0, y + h, 0.0);
	e3dBufferPushVert(x + w, y + h, 0.0);
	e3dBufferPushVert(x + w, y + 0, 0.0);
}

// テクスチャ座標配列に要素追加
void e3dBufferPushTetraTexc(uint16_t imgw, uint16_t imgh, double u, double v, double w, double h){
	e3dBufferPushTexc((u + 0) / imgw, (v + 0) / imgh);
	e3dBufferPushTexc((u + 0) / imgw, (v + h) / imgh);
	e3dBufferPushTexc((u + w) / imgw, (v + h) / imgh);
	e3dBufferPushTexc((u + w) / imgw, (v + 0) / imgh);
}

// 頂点番号配配列に要素追加
void e3dBufferPushTetraFace(uint16_t index){
	e3dBufferPushFace(index, 0, 1, 2);
	e3dBufferPushFace(index, 2, 3, 0);
}

// ----------------------------------------------------------------

// VBOバッファ配列内の位置獲得
uint32_t e3dBufferVretIndexGet(){
	return e3dBufferVertIndex / 3;
}

// IBOバッファ配列内の位置獲得
uint32_t e3dBufferFaceIndexGet(){
	return e3dBufferFaceIndex / 3;
}

// ----------------------------------------------------------------

// バッファ片付け
void e3dBufferClean(){
	free(e3dBufferVert);
	free(e3dBufferClor);
	free(e3dBufferTexc);
	free(e3dBufferFace);
	e3dBufferVertLength = 0;
	e3dBufferClorLength = 0;
	e3dBufferTexcLength = 0;
	e3dBufferFaceLength = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

