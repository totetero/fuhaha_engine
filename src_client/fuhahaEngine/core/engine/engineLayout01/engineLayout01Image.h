#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineLayout01ImageRect{
	struct engineLayout01Trans trans;
	int faceIndex;
	int faceNum;
};

// 画像描画構造体 初期化
void engineLayout01ImageRectInit(struct engineLayout01ImageRect *this);
// 配列に画像の描画情報を追加
void engineLayout01ImageRectCreateArray(struct engineLayout01ImageRect *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th);
void engineLayout01ImageRectCreateArrayWhite(struct engineLayout01ImageRect *this, double x, double y, double w, double h);

// 画像描画構造体 描画
void engineLayout01ImageRectDraw(struct engineLayout01ImageRect *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 画像描画構造体 破棄
void engineLayout01ImageRectDispose(struct engineLayout01ImageRect *this);

// ----------------------------------------------------------------

// 枠付描画構造体
struct engineLayout01ImageFrame{
	struct engineLayout01Trans trans;
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
void engineLayout01ImageFrameInit(struct engineLayout01ImageFrame *this);
// 配列に枠付の描画情報を追加
void engineLayout01ImageFrameCreateArray(struct engineLayout01ImageFrame *this, double x, double y, double w, double h);

// 枠付描画構造体 描画
void engineLayout01ImageFrameDraw(struct engineLayout01ImageFrame *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 枠付描画構造体 破棄
void engineLayout01ImageFrameDispose(struct engineLayout01ImageFrame *this);

// ----------------------------------------------------------------

// 文字列描画構造体
struct engineLayout01ImageText{
	struct engineLayout01Trans trans;
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
void engineLayout01ImageTextInit(struct engineLayout01ImageText *this);
// 配列に文字列(utf8)の描画情報を追加
void engineLayout01ImageTextCreateArray(struct engineLayout01ImageText *this, double x, double y, char *text);

// 文字列描画構造体 描画
void engineLayout01ImageTextDraw(struct engineLayout01ImageText *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 文字列描画構造体 破棄
void engineLayout01ImageTextDispose(struct engineLayout01ImageText *this);

// ----------------------------------------------------------------

// 数値列描画構造体
struct engineLayout01ImageNumber{
	struct engineLayout01Trans trans;
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
void engineLayout01ImageNumberInit(struct engineLayout01ImageNumber *this);
// 配列に数値列(utf8)の描画情報を追加
void engineLayout01ImageNumberCreateArray(struct engineLayout01ImageNumber *this, double x, double y, int number);

// 数値列描画構造体 描画
void engineLayout01ImageNumberDraw(struct engineLayout01ImageNumber *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 数値列描画構造体 破棄
void engineLayout01ImageNumberDispose(struct engineLayout01ImageNumber *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

