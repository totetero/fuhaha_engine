#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 基本図形水滴構造体
struct enginePrimitiveWater{
	int faceIndex;
	int faceNum;
	int quality;
	struct{double y; double r;} sphere0;
	struct{double y; double r;} sphere1;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
};

// 基本図形水滴構造体 初期化
void enginePrimitiveWaterInit(struct enginePrimitiveWater *this, double y0, double y1, double r0, double r1);
// 配列に基本図形水滴構造体の描画情報を追加
void enginePrimitiveWaterCreateArray(struct enginePrimitiveWater *this);
// 基本図形球水滴構造体 描画
void enginePrimitiveWaterDraw(struct enginePrimitiveWater *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

