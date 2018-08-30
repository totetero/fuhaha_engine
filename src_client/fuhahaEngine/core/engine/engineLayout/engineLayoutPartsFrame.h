#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 枠付描画構造体
struct engineLayoutPartsFrame{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	struct{int bt; int bb; int br; int bl;} border;
	double scale;
};

// 枠付描画構造体 作成
struct engineLayoutPartsFrame *engineLayoutPartsFrameCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

