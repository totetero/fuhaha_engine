#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像文字列描画構造体
struct engineLayoutViewPartsImageText{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} alphabet;
	struct{int tu; int tv;} hiragana;
	struct{int tu; int tv;} katakana;
	struct{
		double scale;
		int xalign;
		int yalign;
	} fontStyle;
	struct engineMathVector4 color;
};

// 画像文字列描画構造体 作成
struct engineLayoutViewPartsImageText *engineLayoutViewPartsImageTextCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayoutViewPartsImageText *engineLayoutViewPartsImageTextCreateDefault(char *text);
// 画像文字列描画構造体 文字列設定(utf8)
void engineLayoutViewPartsImageTextSet(struct engineLayoutViewPartsImageText *this, char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

