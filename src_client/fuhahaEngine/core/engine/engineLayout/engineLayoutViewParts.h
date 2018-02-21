#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayoutViewPartsRect{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	struct engineMathVector4 color;
};

// 画像描画構造体 作成
struct engineLayoutViewPartsRect *engineLayoutViewPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayoutViewPartsRect *engineLayoutViewPartsRectCreateWhite();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 枠付描画構造体
struct engineLayoutViewPartsFrame{
	struct engineLayoutView super;
	struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	struct{int bt; int bb; int br; int bl;} border;
	double scale;
};

// 枠付描画構造体 作成
struct engineLayoutViewPartsFrame *engineLayoutViewPartsFrameCreate();

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

// フォント数字描画構造体
struct engineLayoutViewPartsFontNumber{
	struct engineLayoutView super;
	struct{
		double size;
		int xalign;
		int yalign;
		struct{double size; int quality; struct engineMathVector4 color;} outline;
	} fontStyle;
	struct engineMathVector4 color;
	int value;
};

// フォント数字描画構造体 作成
struct engineLayoutViewPartsFontNumber *engineLayoutViewPartsFontNumberCreate(enum pluginTextureFontSetId fontSetId);
struct engineLayoutViewPartsFontNumber *engineLayoutViewPartsFontNumberCreateDefault(int value);

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

// 画像数字描画構造体
struct engineLayoutViewPartsImageNumber{
	struct engineLayoutView super;
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
struct engineLayoutViewPartsImageNumber *engineLayoutViewPartsImageNumberCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th);
struct engineLayoutViewPartsImageNumber *engineLayoutViewPartsImageNumberCreateDefault(int value);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 標準ボタン構造体
struct engineLayoutViewPartsButtonBox{
	struct engineLayoutView super;
	bool isSelect;
	bool isInactiveDraw;
	struct engineLayoutViewPartsFrame *frameNormal;
	struct engineLayoutViewPartsFrame *frameSelect;
	struct engineLayoutViewPartsFrame *frameActibve;
	struct engineLayoutViewPartsFrame *frameInactive;
	struct engineLayoutView *viewInner;
};

// 標準ボタン構造体 作成
struct engineLayoutViewPartsButtonBox *engineLayoutViewPartsButtonBoxCreate();
struct engineLayoutViewPartsButtonBox *engineLayoutViewPartsButtonBoxCreateText(char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スクローラ構造体
struct engineLayoutViewPartsScroller{
	struct engineLayoutView super;
	struct{double x; double y;} position;
	struct{double x; double y;} velocity;
	struct{double w; double h;} inner;
	struct{struct engineLayoutViewPartsScroller *parentScroller; bool isChildrenMove;} bubbling;
	struct engineLayoutView *viewInner;
	struct engineLayoutView *viewBarX;
	struct engineLayoutView *viewBarY;
};

// スクローラ構造体 作成
struct engineLayoutViewPartsScroller *engineLayoutViewPartsScrollerCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 長方形マスク構造体
struct engineLayoutViewPartsMaskRect{
	struct engineLayoutView super;
};

// 長方形マスク構造体 作成
struct engineLayoutViewPartsMaskRect *engineLayoutViewPartsMaskRectCreate();

// ----------------------------------------------------------------

// カラーマトリクスフィルタ構造体
struct engineLayoutViewPartsFilterColor{
	struct engineLayoutView super;
	struct engineMathMatrix44 matrix;
	struct engineMathVector4 vector;
	double alpha;
};

// カラーマトリクスフィルタ構造体 作成
struct engineLayoutViewPartsFilterColor *engineLayoutViewPartsFilterColorCreate();
// カラーマトリクスフィルタ構造体 色行列設定
void engineLayoutViewPartsFilterColorCreateSetNegative1(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorCreateSetNegative2(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorCreateSetGrayscale(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorCreateSetSepia(struct engineLayoutViewPartsFilterColor *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

