#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フォント数字描画構造体
struct engineLayoutPartsFontNumber{
	struct engineLayoutView super;
	struct{
		double size;
		int xalign;
		int yalign;
		struct engineMathVector4 color;
		struct{double size; int quality; struct engineMathVector4 color;} outline;
	} fontStyle;
	int value;
};

// フォント数字描画構造体 作成
struct engineLayoutPartsFontNumber *engineLayoutPartsFontNumberCreate(enum pluginTextureFontSetId fontSetId);
struct engineLayoutPartsFontNumber *engineLayoutPartsFontNumberCreateDefault(int value);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

