#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayoutOld01ImageRect{
	struct engineLayoutOld01Trans trans;
	int faceIndex;
	int faceNum;
};

// 画像描画構造体 初期化
void engineLayoutOld01ImageRectInit(struct engineLayoutOld01ImageRect *this);
// 配列に画像の描画情報を追加
void engineLayoutOld01ImageRectCreateArray(struct engineLayoutOld01ImageRect *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th);
void engineLayoutOld01ImageRectCreateArrayWhite(struct engineLayoutOld01ImageRect *this, double x, double y, double w, double h);

// 画像描画構造体 描画
void engineLayoutOld01ImageRectDraw(struct engineLayoutOld01ImageRect *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 画像描画構造体 破棄
void engineLayoutOld01ImageRectDispose(struct engineLayoutOld01ImageRect *this);

// ----------------------------------------------------------------

// 枠付描画構造体
struct engineLayoutOld01ImageFrame{
	struct engineLayoutOld01Trans trans;
	int faceIndex;
	int faceNum;
	struct{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} texture;
		struct{int bt; int bb; int br; int bl;} border;
		double scale;
	} createArrayInfo;
	double x;
	double y;
	double w;
	double h;
};

// 枠付描画構造体 初期化
void engineLayoutOld01ImageFrameInit(struct engineLayoutOld01ImageFrame *this);
// 配列に枠付の描画情報を追加
void engineLayoutOld01ImageFrameCreateArray(struct engineLayoutOld01ImageFrame *this, double x, double y, double w, double h);

// 枠付描画構造体 描画
void engineLayoutOld01ImageFrameDraw(struct engineLayoutOld01ImageFrame *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 枠付描画構造体 破棄
void engineLayoutOld01ImageFrameDispose(struct engineLayoutOld01ImageFrame *this);

// ----------------------------------------------------------------

// 文字列描画構造体
struct engineLayoutOld01ImageText{
	struct engineLayoutOld01Trans trans;
	int faceIndex;
	int faceNum;
	struct{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} alphabet;
		struct{int tu; int tv; int tw; int th;} hiragana;
		struct{int tu; int tv; int tw; int th;} katakana;
		double scale;
		int xalign;
		int yalign;
	} createArrayInfo;
};

// 文字列描画構造体 初期化
void engineLayoutOld01ImageTextInit(struct engineLayoutOld01ImageText *this);
// 配列に文字列(utf8)の描画情報を追加
void engineLayoutOld01ImageTextCreateArray(struct engineLayoutOld01ImageText *this, double x, double y, char *text);

// 文字列描画構造体 描画
void engineLayoutOld01ImageTextDraw(struct engineLayoutOld01ImageText *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 文字列描画構造体 破棄
void engineLayoutOld01ImageTextDispose(struct engineLayoutOld01ImageText *this);

// ----------------------------------------------------------------

// 数値列描画構造体
struct engineLayoutOld01ImageNumber{
	struct engineLayoutOld01Trans trans;
	int faceIndex;
	int faceNum;
	struct{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} number;
		double scale;
		int xalign;
		int yalign;
	} createArrayInfo;
	double width;
	int value;
};

// 数値列描画構造体 初期化
void engineLayoutOld01ImageNumberInit(struct engineLayoutOld01ImageNumber *this);
// 配列に数値列(utf8)の描画情報を追加
void engineLayoutOld01ImageNumberCreateArray(struct engineLayoutOld01ImageNumber *this, double x, double y, int number);

// 数値列描画構造体 描画
void engineLayoutOld01ImageNumberDraw(struct engineLayoutOld01ImageNumber *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 数値列描画構造体 破棄
void engineLayoutOld01ImageNumberDispose(struct engineLayoutOld01ImageNumber *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

