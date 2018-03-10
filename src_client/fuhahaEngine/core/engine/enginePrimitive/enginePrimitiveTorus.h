#pragma once

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

