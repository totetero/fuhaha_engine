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

// 文字列描画構造体
struct engineLayout02ViewPartsText{
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
struct engineLayout02ViewPartsText *engineLayout02ViewPartsTextCreate();
// 文字列描画構造体 文字列設定
void engineLayout02ViewPartsTextSet(struct engineLayout02ViewPartsText *this, enum pluginTextureFontSetId fontSetId, char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 標準ボタン構造体
struct engineLayout02ViewPartsButtonBox{
	struct engineLayout02View super;
	struct engineCtrlButton button;
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
// 標準ボタン構造体 ボタン処理
void engineLayout02ViewPartsButtonBoxCalcButton(struct engineLayout02ViewPartsButtonBox *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

