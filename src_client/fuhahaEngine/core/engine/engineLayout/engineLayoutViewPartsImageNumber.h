#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像数字描画構造体
struct engineLayoutViewPartsImageNumber{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} number;
	struct{
		double scale;
		int xalign;
		int yalign;
	} fontStyle;
	int value;
};

// 画像数字描画構造体 作成
struct engineLayoutViewPartsImageNumber *engineLayoutViewPartsImageNumberCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayoutViewPartsImageNumber *engineLayoutViewPartsImageNumberCreateDefault(int value);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

