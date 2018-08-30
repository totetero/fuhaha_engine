#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フォント文字列描画構造体
struct engineLayoutPartsFontText{
	struct engineLayoutView super;
	struct{
		double size;
		int lineNum;
		int wordNum;
		int maxWidth;
		int maxHeight;
		int xalign;
		int yalign;
		struct engineMathVector4 color;
		struct{double size; int quality; struct engineMathVector4 color;} outline;
	} fontStyle;
};

// フォント文字列描画構造体 作成
struct engineLayoutPartsFontText *engineLayoutPartsFontTextCreate(enum pluginTextureFontSetId fontSetId);
struct engineLayoutPartsFontText *engineLayoutPartsFontTextCreateDefault(char *text);
// フォント文字列描画構造体 文字列設定
void engineLayoutPartsFontTextSet(struct engineLayoutPartsFontText *this, char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

