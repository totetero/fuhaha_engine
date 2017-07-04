#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 長方形表示構造体
struct engineLayout02ViewPartsRect{
	struct engineLayout02View super;
	int tu;
	int tv;
	int tw;
	int th;
	int imgw;
	int imgh;
	struct engineMathVector4 color;
};

// 長方形表示構造体 作成
struct engineLayout02ViewPartsRect *engineLayout02ViewPartsRectCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

