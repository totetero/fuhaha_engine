#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/enginePrimitive/enginePrimitive.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 基本図形球構造体 初期化
void enginePrimitiveTorusInit(struct enginePrimitiveTorus *this, double r0, double r1){
	this->quality0 = 8;
	this->quality1 = 8;
	this->torus.r0 = r0;
	this->torus.r1 = r1;
	this->texture.imgw = TEXSIZ_SYSTEM_W;
	this->texture.imgh = TEXSIZ_SYSTEM_H;
	this->texture.tu = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25);
	this->texture.tv = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25);
	this->texture.tw = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_W * 0.5);
	this->texture.th = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_H * 0.5);
}

// 配列に基本図形球構造体の描画情報を追加
void enginePrimitiveTorusCreateArray(struct enginePrimitiveTorus *this){
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int faceNum = 0;

	// 頂点配列作成
	for(int i = 0; i < (this->quality0 + 1); i++){
		double u = i / (double)this->quality0;
		double c0 = engineMathCos(2 * ENGINEMATH_PI * u);
		double s0 = engineMathSin(2 * ENGINEMATH_PI * u);
		double x0 = this->torus.r0 * c0;
		double y0 = 0;
		double z0 = this->torus.r0 * s0;
		for(int j = 0; j < (this->quality1 + 1); j++){
			double v = j / (double)this->quality1;
			double c1 = engineMathCos(2 * ENGINEMATH_PI * v);
			double s1 = engineMathSin(2 * ENGINEMATH_PI * v);
			double x1 = this->torus.r1 * c1 * c0;
			double y1 = this->torus.r1 * s1;
			double z1 = this->torus.r1 * c1 * s0;
			double tu = (this->texture.tu + this->texture.tw * u) / this->texture.imgw;
			double tv = (this->texture.tv + this->texture.th * v) / this->texture.imgh;
			engineGraphicBufferPushVert(x0 + x1, y0 + y1, z0 + z1);
			engineGraphicBufferPushNorm(x1, y1, z1);
			engineGraphicBufferPushTexc(tu, tv);
		}
	}

	// 頂点インデックス配列作成
	for(int i = 0; i < this->quality0; i++){
		for(int j = 0; j < this->quality1; j++){
			int index00 = (i + 0) * (this->quality1 + 1) + (j + 0);
			int index01 = (i + 0) * (this->quality1 + 1) + (j + 1);
			int index02 = (i + 1) * (this->quality1 + 1) + (j + 0);
			int index10 = (i + 1) * (this->quality1 + 1) + (j + 0);
			int index11 = (i + 0) * (this->quality1 + 1) + (j + 1);
			int index12 = (i + 1) * (this->quality1 + 1) + (j + 1);
			engineGraphicBufferPushFace(vertIndex, index00, index01, index02); faceNum++;
			engineGraphicBufferPushFace(vertIndex, index10, index11, index12); faceNum++;
		}
	}

	this->faceIndex = faceIndex;
	this->faceNum = faceNum;
}

// 基本図形球構造体 描画
void enginePrimitiveTorusDraw(struct enginePrimitiveTorus *this){
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

