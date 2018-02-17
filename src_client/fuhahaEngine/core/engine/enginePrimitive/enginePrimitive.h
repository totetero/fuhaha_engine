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

