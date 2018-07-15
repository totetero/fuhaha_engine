#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フォント数字描画構造体
struct engineLayoutViewPartsFontNumber{
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
struct engineLayoutViewPartsFontNumber *engineLayoutViewPartsFontNumberCreate(enum pluginTextureFontSetId fontSetId);
struct engineLayoutViewPartsFontNumber *engineLayoutViewPartsFontNumberCreateDefault(int value);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

