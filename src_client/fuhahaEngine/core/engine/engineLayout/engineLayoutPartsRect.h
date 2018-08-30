#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayoutPartsRect{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
};

// 画像描画構造体 作成
struct engineLayoutPartsRect *engineLayoutPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayoutPartsRect *engineLayoutPartsRectCreateWhite(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

