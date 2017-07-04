#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayout02ViewPartsRect{
	struct engineLayout02View super;
	int imgw;
	int imgh;
	struct{int tu; int tv; int tw; int th;} texture;
	struct engineMathVector4 color;
};

// 画像描画構造体 作成
struct engineLayout02ViewPartsRect *engineLayout02ViewPartsRectCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 枠付描画構造体
struct engineLayout02ViewPartsFrame{
	struct engineLayout02View super;
	int imgw;
	int imgh;
	struct{int tu; int tv; int tw; int th;} texture;
	struct{int bt; int bb; int br; int bl;} border;
	double scale;
};

// 枠付描画構造体 作成
struct engineLayout02ViewPartsFrame *engineLayout02ViewPartsFrameCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

