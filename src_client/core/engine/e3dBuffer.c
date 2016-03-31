#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	double *e3dBufferVert;
	double *e3dBufferClor;
	double *e3dBufferTexc;
	uint16_t *e3dBufferFace;
	uint32_t e3dBufferVertIndex;
	uint32_t e3dBufferClorIndex;
	uint32_t e3dBufferTexcIndex;
	uint32_t e3dBufferFaceIndex;
	uint32_t e3dBufferVertLength;
	uint32_t e3dBufferClorLength;
	uint32_t e3dBufferTexcLength;
	uint32_t e3dBufferFaceLength;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バッファ作成開始
void e3dBufferBegin(){
	localGlobal.e3dBufferVertIndex = 0;
	localGlobal.e3dBufferClorIndex = 0;
	localGlobal.e3dBufferTexcIndex = 0;
	localGlobal.e3dBufferFaceIndex = 0;
}

// バッファ作成完了
void e3dBufferEnd(e3dObjectVBOId *e3dIdVert, e3dObjectVBOId *e3dIdClor, e3dObjectVBOId *e3dIdTexc, e3dObjectIBOId *e3dIdFace){
	if(e3dIdVert != NULL){e3dObjectVBODispose(*e3dIdVert); *e3dIdVert = e3dObjectVBOCreate(localGlobal.e3dBufferVertIndex, localGlobal.e3dBufferVert);}
	if(e3dIdClor != NULL){e3dObjectVBODispose(*e3dIdClor); *e3dIdClor = e3dObjectVBOCreate(localGlobal.e3dBufferClorIndex, localGlobal.e3dBufferClor);}
	if(e3dIdTexc != NULL){e3dObjectVBODispose(*e3dIdTexc); *e3dIdTexc = e3dObjectVBOCreate(localGlobal.e3dBufferTexcIndex, localGlobal.e3dBufferTexc);}
	if(e3dIdFace != NULL){e3dObjectIBODispose(*e3dIdFace); *e3dIdFace = e3dObjectIBOCreate(localGlobal.e3dBufferFaceIndex, localGlobal.e3dBufferFace);}
}

// ----------------------------------------------------------------

// 頂点座標配列に要素追加
void e3dBufferPushVert(double x, double y, double z){
	if(localGlobal.e3dBufferVertLength < localGlobal.e3dBufferVertIndex + 3){
		uint32_t length = localGlobal.e3dBufferVertLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.e3dBufferVertLength > 0){
			memcpy(array, localGlobal.e3dBufferVert, localGlobal.e3dBufferVertLength * sizeof(double));
			free(localGlobal.e3dBufferVert);
		}
		localGlobal.e3dBufferVertLength = length;
		localGlobal.e3dBufferVert = array;
	}
	*(localGlobal.e3dBufferVert + localGlobal.e3dBufferVertIndex++) = x;
	*(localGlobal.e3dBufferVert + localGlobal.e3dBufferVertIndex++) = y;
	*(localGlobal.e3dBufferVert + localGlobal.e3dBufferVertIndex++) = z;
}

// 色彩配列に要素追加
void e3dBufferPushClor(double r, double g, double b){
	if(localGlobal.e3dBufferClorLength < localGlobal.e3dBufferClorIndex + 3){
		uint32_t length = localGlobal.e3dBufferClorLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.e3dBufferClorLength > 0){
			memcpy(array, localGlobal.e3dBufferClor, localGlobal.e3dBufferClorLength * sizeof(double));
			free(localGlobal.e3dBufferClor);
		}
		localGlobal.e3dBufferClorLength = length;
		localGlobal.e3dBufferClor = array;
	}
	*(localGlobal.e3dBufferClor + localGlobal.e3dBufferClorIndex++) = r;
	*(localGlobal.e3dBufferClor + localGlobal.e3dBufferClorIndex++) = g;
	*(localGlobal.e3dBufferClor + localGlobal.e3dBufferClorIndex++) = b;
}

// テクスチャ座標配列に要素追加
void e3dBufferPushTexc(double u, double v){
	if(localGlobal.e3dBufferTexcLength < localGlobal.e3dBufferTexcIndex + 2){
		uint32_t length = localGlobal.e3dBufferTexcLength + 200;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.e3dBufferTexcLength > 0){
			memcpy(array, localGlobal.e3dBufferTexc, localGlobal.e3dBufferTexcLength * sizeof(double));
			free(localGlobal.e3dBufferTexc);
		}
		localGlobal.e3dBufferTexcLength = length;
		localGlobal.e3dBufferTexc = array;
	}
	*(localGlobal.e3dBufferTexc + localGlobal.e3dBufferTexcIndex++) = u;
	*(localGlobal.e3dBufferTexc + localGlobal.e3dBufferTexcIndex++) = v;
}

// 頂点番号配配列に要素追加
void e3dBufferPushFace(uint16_t index, uint16_t t0, uint16_t t1, uint16_t t2){
	if(localGlobal.e3dBufferFaceLength < localGlobal.e3dBufferFaceIndex + 3){
		uint32_t length = localGlobal.e3dBufferFaceLength + 150;
		uint16_t *array = (uint16_t*)malloc(length * sizeof(uint16_t));
		if(localGlobal.e3dBufferFaceLength > 0){
			memcpy(array, localGlobal.e3dBufferFace, localGlobal.e3dBufferFaceLength * sizeof(uint16_t));
			free(localGlobal.e3dBufferFace);
		}
		localGlobal.e3dBufferFaceLength = length;
		localGlobal.e3dBufferFace = array;
	}
	*(localGlobal.e3dBufferFace + localGlobal.e3dBufferFaceIndex++) = index + t0;
	*(localGlobal.e3dBufferFace + localGlobal.e3dBufferFaceIndex++) = index + t1;
	*(localGlobal.e3dBufferFace + localGlobal.e3dBufferFaceIndex++) = index + t2;
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
	return localGlobal.e3dBufferVertIndex / 3;
}

// IBOバッファ配列内の位置獲得
uint32_t e3dBufferFaceIndexGet(){
	return localGlobal.e3dBufferFaceIndex / 3;
}

// ----------------------------------------------------------------

// バッファ片付け
void e3dBufferClean(){
	free(localGlobal.e3dBufferVert);
	free(localGlobal.e3dBufferClor);
	free(localGlobal.e3dBufferTexc);
	free(localGlobal.e3dBufferFace);
	localGlobal.e3dBufferVertLength = 0;
	localGlobal.e3dBufferClorLength = 0;
	localGlobal.e3dBufferTexcLength = 0;
	localGlobal.e3dBufferFaceLength = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

