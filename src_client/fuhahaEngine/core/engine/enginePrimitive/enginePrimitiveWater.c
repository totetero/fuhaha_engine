#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/enginePrimitive/enginePrimitive.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 基本図形水滴構造体 初期化
void enginePrimitiveWaterInit(struct enginePrimitiveWater *this, double y0, double y1, double r0, double r1){
	this->quality = 8;
	this->sphere0.y = y0;
	this->sphere0.r = r0;
	this->sphere1.y = y1;
	this->sphere1.r = r1;
	this->texture.imgw = TEXSIZ_SYSTEM_W;
	this->texture.imgh = TEXSIZ_SYSTEM_H;
	this->texture.tu = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25);
	this->texture.tv = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25);
	this->texture.tw = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_W * 0.5);
	this->texture.th = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_H * 0.5);
}

// 配列に基本図形水滴構造体の描画情報を追加
void enginePrimitiveWaterCreateArray(struct enginePrimitiveWater *this){
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int faceNum = 0;

	// 頂点配列作成 回転体点
	double ymax = this->sphere0.y + this->sphere0.r;
	double ymin = this->sphere1.y - this->sphere1.r;
	double theta = ENGINEMATH_PI - engineMathAcos((this->sphere1.r - this->sphere0.r) / (this->sphere1.y - this->sphere0.y));
	int num0 = engineMathRound(this->quality * theta / ENGINEMATH_PI);
	if(num0 < 2){num0 = 2;}else if(num0 > this->quality - 2){num0 = this->quality - 2;}
	for(int i = 0; i < num0; i++){
		double phi = theta * (i / (double)(num0 - 1));
		double phicos = engineMathCos(phi);
		double phisin = engineMathSin(phi);
		for(int j = 0; j < (this->quality * 2 + 1); j++){
			double y = this->sphere0.r * phicos + this->sphere0.y;
			double u = j / (double)(this->quality * 2);
			double v = (y - ymin) / (ymax - ymin);
			double psi = 2 * ENGINEMATH_PI * u;
			double psicos = engineMathCos(psi);
			double psisin = engineMathSin(psi);
			double x = this->sphere0.r * phisin * psicos;
			double z = this->sphere0.r * phisin * psisin;
			double tu = (this->texture.tu + this->texture.tw * (1 - u)) / this->texture.imgw;
			double tv = (this->texture.tv + this->texture.th * (1 - v)) / this->texture.imgh;
			engineGraphicBufferPushVert(x, y, z);
			engineGraphicBufferPushNorm(x, y - this->sphere0.y, z);
			engineGraphicBufferPushTexc(tu, tv);
		}
	}
	int num1 = this->quality - num0;
	for(int i = 0; i < num1; i++){
		double phi = ENGINEMATH_PI - (ENGINEMATH_PI - theta) * (1 - (i / (double)(num1 - 1)));
		double phicos = engineMathCos(phi);
		double phisin = engineMathSin(phi);
		for(int j = 0; j < (this->quality * 2 + 1); j++){
			double y = this->sphere1.r * phicos + this->sphere1.y;
			double u = j / (double)(this->quality * 2);
			double v = (y - ymin) / (ymax - ymin);
			double psi = 2 * ENGINEMATH_PI * u;
			double psicos = engineMathCos(psi);
			double psisin = engineMathSin(psi);
			double x = this->sphere1.r * phisin * psicos;
			double z = this->sphere1.r * phisin * psisin;
			double tu = (this->texture.tu + this->texture.tw * (1 - u)) / this->texture.imgw;
			double tv = (this->texture.tv + this->texture.th * (1 - v)) / this->texture.imgh;
			engineGraphicBufferPushVert(x, y, z);
			engineGraphicBufferPushNorm(x, y - this->sphere1.y, z);
			engineGraphicBufferPushTexc(tu, tv);
		}
	}

	// 頂点インデックス配列作成
	for(int i = 0; i < (this->quality - 1); i++){
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

// 基本図形水滴構造体 描画
void enginePrimitiveWaterDraw(struct enginePrimitiveWater *this){
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

