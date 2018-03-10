#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フォント文字列描画構造体
struct engineLayoutViewPartsFontText{
	struct engineLayoutView super;
	struct{
		double size;
		int lineNum;
		int wordNum;
		int maxWidth;
		int maxHeight;
		int xalign;
		int yalign;
		struct{double size; int quality; struct engineMathVector4 color;} outline;
	} fontStyle;
	struct engineMathVector4 color;
};

// フォント文字列描画構造体 作成
struct engineLayoutViewPartsFontText *engineLayoutViewPartsFontTextCreate(enum pluginTextureFontSetId fontSetId);
struct engineLayoutViewPartsFontText *engineLayoutViewPartsFontTextCreateDefault(char *text);
// フォント文字列描画構造体 文字列設定
void engineLayoutViewPartsFontTextSet(struct engineLayoutViewPartsFontText *this, char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

