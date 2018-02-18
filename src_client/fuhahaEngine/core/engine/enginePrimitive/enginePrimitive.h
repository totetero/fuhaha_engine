#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 基本図形球構造体
struct enginePrimitiveSphere{
	int faceIndex;
	int faceNum;
	int quality;
	struct{double r;} sphere;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
};

// 基本図形球構造体 初期化
void enginePrimitiveSphereInit(struct enginePrimitiveSphere *this, double r);
// 配列に基本図形球構造体の描画情報を追加
void enginePrimitiveSphereCreateArray(struct enginePrimitiveSphere *this);
// 基本図形球構造体 描画
void enginePrimitiveSphereDraw(struct enginePrimitiveSphere *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 基本図形トーラス構造体
struct enginePrimitiveTorus{
	int faceIndex;
	int faceNum;
	int quality0;
	int quality1;
	struct{double r0; double r1;} torus;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
};

// 基本図形トーラス構造体 初期化
void enginePrimitiveTorusInit(struct enginePrimitiveTorus *this, double r0, double r1);
// 配列に基本図形トーラス構造体の描画情報を追加
void enginePrimitiveTorusCreateArray(struct enginePrimitiveTorus *this);
// 基本図形球トーラス構造体 描画
void enginePrimitiveTorusDraw(struct enginePrimitiveTorus *this);

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

