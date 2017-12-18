#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayout02ViewPartsRect{
	struct engineLayout02View super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	struct engineMathVector4 color;
};

// 画像描画構造体 作成
struct engineLayout02ViewPartsRect *engineLayout02ViewPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayout02ViewPartsRect *engineLayout02ViewPartsRectCreateWhite();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 枠付描画構造体
struct engineLayout02ViewPartsFrame{
	struct engineLayout02View super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	struct{int bt; int bb; int br; int bl;} border;
	double scale;
};

// 枠付描画構造体 作成
struct engineLayout02ViewPartsFrame *engineLayout02ViewPartsFrameCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 文字列描画構造体
struct engineLayout02ViewPartsFontText{
	struct engineLayout02View super;
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

// 文字列描画構造体 作成
struct engineLayout02ViewPartsFontText *engineLayout02ViewPartsFontTextCreate();
struct engineLayout02ViewPartsFontText *engineLayout02ViewPartsFontTextCreateText(char *text);
// 文字列描画構造体 文字列設定
void engineLayout02ViewPartsFontTextSet(struct engineLayout02ViewPartsFontText *this, enum pluginTextureFontSetId fontSetId, char *text);

// ----------------------------------------------------------------

// 画像文字列描画構造体
struct engineLayout02ViewPartsImageText{
	struct engineLayout02View super;
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
struct engineLayout02ViewPartsImageText *engineLayout02ViewPartsImageTextCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayout02ViewPartsImageText *engineLayout02ViewPartsImageTextCreateDefault(char *text);
// 画像文字列描画構造体 文字列設定(utf8)
void engineLayout02ViewPartsImageTextSet(struct engineLayout02ViewPartsImageText *this, char *text);

// ----------------------------------------------------------------

// 画像数字描画構造体
struct engineLayout02ViewPartsImageNumber{
	struct engineLayout02View super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} number;
	struct{
		double scale;
		int xalign;
		int yalign;
	} fontStyle;
	struct engineMathVector4 color;
	int value;
};

// 画像数字描画構造体 作成
struct engineLayout02ViewPartsImageNumber *engineLayout02ViewPartsImageNumberCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayout02ViewPartsImageNumber *engineLayout02ViewPartsImageNumberCreateDefault(int value);
// 画像文字列描画構造体 数値設定
void engineLayout02ViewPartsImageNumberSet(struct engineLayout02ViewPartsImageNumber *this, int value);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 標準ボタン構造体
struct engineLayout02ViewPartsButtonBox{
	struct engineLayout02View super;
	bool isSelect;
	bool isInactiveDraw;
	struct engineLayout02ViewPartsFrame *frameNormal;
	struct engineLayout02ViewPartsFrame *frameSelect;
	struct engineLayout02ViewPartsFrame *frameActibve;
	struct engineLayout02ViewPartsFrame *frameInactive;
	struct engineLayout02View *viewInner;
};

// 標準ボタン構造体 作成
struct engineLayout02ViewPartsButtonBox *engineLayout02ViewPartsButtonBoxCreate();
struct engineLayout02ViewPartsButtonBox *engineLayout02ViewPartsButtonBoxCreateText(char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スクローラ構造体
struct engineLayout02ViewPartsScroller{
	struct engineLayout02View super;
	struct{double x; double y;} position;
	struct{double x; double y;} velocity;
	struct{double w; double h;} inner;
	struct{struct engineLayout02ViewPartsScroller *parentScroller; bool isChildrenMove;} bubbling;
	struct engineLayout02View *viewInner;
	struct engineLayout02View *viewBarX;
	struct engineLayout02View *viewBarY;
};

// スクローラ構造体 作成
struct engineLayout02ViewPartsScroller *engineLayout02ViewPartsScrollerCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 長方形マスク構造体
struct engineLayout02ViewPartsMaskRect{
	struct engineLayout02View super;
};

// 長方形マスク構造体 作成
struct engineLayout02ViewPartsMaskRect *engineLayout02ViewPartsMaskRectCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

