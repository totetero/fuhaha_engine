#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayoutViewPartsRect{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
};

// 画像描画構造体 作成
struct engineLayoutViewPartsRect *engineLayoutViewPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayoutViewPartsRect *engineLayoutViewPartsRectCreateWhite();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

