#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/enginePrimitive/enginePrimitive.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 基本図形球構造体 初期化
void enginePrimitiveSphereInit(struct enginePrimitiveSphere *this, double r){
	this->quality = 8;
	this->sphere.r = r;
	this->texture.imgw = TEXSIZ_SYSTEM_W;
	this->texture.imgh = TEXSIZ_SYSTEM_H;
	this->texture.tu = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25);
	this->texture.tv = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25);
	this->texture.tw = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_W * 0.5);
	this->texture.th = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_H * 0.5);
}

// 配列に基本図形球構造体の描画情報を追加
void enginePrimitiveSphereCreateArray(struct enginePrimitiveSphere *this){
	if(this == NULL){return;}

	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int faceNum = 0;

	// 頂点配列作成
	for(int i = 0; i < (this->quality + 1); i++){
		double v = i / (double)this->quality;
		double y = this->sphere.r * engineMathCos(ENGINEMATH_PI * v);
		double r = this->sphere.r * engineMathSin(ENGINEMATH_PI * v);
		for(int j = 0; j < (this->quality * 2 + 1); j++){
			double u = j / (double)(this->quality * 2);
			double x = r * engineMathCos(2 * ENGINEMATH_PI * u);
			double z = r * engineMathSin(2 * ENGINEMATH_PI * u);
			double tu = (this->texture.tu + this->texture.tw * (1 - u)) / this->texture.imgw;
			double tv = (this->texture.tv + this->texture.th * v) / this->texture.imgh;
			engineGraphicBufferPushVert(x, y, z);
			engineGraphicBufferPushNorm(x, y, z);
			engineGraphicBufferPushTexc(tu, tv);
		}
	}

	// 頂点インデックス配列作成
	for(int i = 0; i < this->quality; i++){
		for(int j = 0; j < (this->quality * 2); j++){
			int index00 = (i + 0) * (this->quality * 2 + 1) + (j + 0);
			int index01 = (i + 0) * (this->quality * 2 + 1) + (j + 1);
			int index02 = (i + 1) * (this->quality * 2 + 1) + (j + 0);
			int index10 = (i + 1) * (this->quality * 2 + 1) + (j + 0);
			int index11 = (i + 0) * (this->quality * 2 + 1) + (j + 1);
			int index12 = (i + 1) * (this->quality * 2 + 1) + (j + 1);
			engineGraphicBufferPushFace(vertIndex, index00, index01, index02); faceNum++;
			engineGraphicBufferPushFace(vertIndex, index10, index11, index12); faceNum++;
		}
	}

	this->faceIndex = faceIndex;
	this->faceNum = faceNum;
}

// 基本図形球構造体 描画
void enginePrimitiveSphereDraw(struct enginePrimitiveSphere *this){
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

