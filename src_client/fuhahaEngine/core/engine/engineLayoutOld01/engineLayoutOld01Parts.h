#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン構造体
struct engineLayoutOld01Button{
	double x;
	double y;
	double w;
	double h;
	bool active;
	bool inactive;
	bool trigger;
	struct engineLayoutOld01Button *child;
	bool zKey;
	bool xKey;
	bool cKey;
	bool vKey;
	bool inner;

	bool isMatrix;
	struct engineMathVector3 hitVertex0[6];
	struct engineMathVector3 hitVertex1[6];
	int hitVertexLength;
};

// ボタン構造体 初期化
void engineLayoutOld01ButtonInit(struct engineLayoutOld01Button *this);
// ボタン構造体 静的位置設定
void engineLayoutOld01ButtonSetPosition(struct engineLayoutOld01Button *this, double x, double y, double w, double h);
// ボタン構造体 領域内確認行列読み込み
void engineLayoutOld01ButtonSetMatrix(struct engineLayoutOld01Button *this, struct engineMathMatrix44 *mat);
// ボタン構造体 計算
void engineLayoutOld01ButtonCalc(struct engineLayoutOld01Button *this);
// ボタン構造体 タッチ情報を使い回す計算
void engineLayoutOld01ButtonSubCalc(struct engineLayoutOld01Button *this, struct engineCtrlTouch *t, bool clickable);
// ボタン構造体 タッチ領域内確認
bool engineLayoutOld01ButtonIsInner(struct engineLayoutOld01Button *this, struct engineCtrlTouch *t);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示箱付きボタン構造体
struct engineLayoutOld01ButtonBox{
	struct engineLayoutOld01Button super;
	struct engineLayoutOld01Trans trans;
	struct engineLayoutOld01ImageFrame imgNormal;
	struct engineLayoutOld01ImageFrame imgSelect;
	struct engineLayoutOld01ImageFrame imgActibve;
	struct engineLayoutOld01ImageFrame imgInactive;
	struct engineLayoutOld01ImageText imgText;
	bool select;
};

// 表示箱付きボタン構造体 初期化
void engineLayoutOld01ButtonBoxInit(struct engineLayoutOld01ButtonBox *this);
// 表示箱付きボタン構造体 静的位置設定
void engineLayoutOld01ButtonBoxSetPosition(struct engineLayoutOld01ButtonBox *this, double x, double y, double w, double h);
// 表示箱付きボタン構造体 計算
void engineLayoutOld01ButtonBoxCalc(struct engineLayoutOld01ButtonBox *this);
// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void engineLayoutOld01ButtonBoxSubCalc(struct engineLayoutOld01ButtonBox *this, struct engineCtrlTouch *t, bool clickable);
// 配列に表示箱付きボタンの描画情報を追加
void engineLayoutOld01ButtonBoxCreateArray(struct engineLayoutOld01ButtonBox *this, char *text);

// 表示箱付きボタン構造体 描画
void engineLayoutOld01ButtonBoxDraw(struct engineLayoutOld01ButtonBox *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示箱付きボタン構造体 破棄
void engineLayoutOld01ButtonBoxDispose(struct engineLayoutOld01ButtonBox *this);

// ----------------------------------------------------------------

// ピッカーボタン構造体
struct engineLayoutOld01ButtonPicker{
	struct engineLayoutOld01Button super;
	struct engineLayoutOld01Trans trans;
	struct engineLayoutOld01ImageRect imgRectNormal;
	struct engineLayoutOld01ImageRect imgRectSelect;
	struct engineLayoutOld01ImageRect imgRectActibve;
	struct engineLayoutOld01ImageRect imgRectInactive;
	struct engineLayoutOld01ImageFrame imgFrameNormal;
	struct engineLayoutOld01ImageFrame imgFrameSelect;
	struct engineLayoutOld01ImageFrame imgFrameActibve;
	struct engineLayoutOld01ImageFrame imgFrameInactive;
	struct engineLayoutOld01ImageText imgText[8];
	int picked;
	bool select;
};

// ピッカーボタン構造体 初期化
void engineLayoutOld01ButtonPickerInit(struct engineLayoutOld01ButtonPicker *this, int picked);
// ピッカーボタン構造体 静的位置設定
void engineLayoutOld01ButtonPickerSetPosition(struct engineLayoutOld01ButtonPicker *this, double x, double y, double w);
// ピッカーボタン構造体 計算
void engineLayoutOld01ButtonPickerCalc(struct engineLayoutOld01ButtonPicker *this);
// ピッカーボタン構造体 タッチ情報を使い回す計算
void engineLayoutOld01ButtonPickerSubCalc(struct engineLayoutOld01ButtonPicker *this, struct engineCtrlTouch *t, bool clickable);
// 配列にピッカーボタンの描画情報を追加
void engineLayoutOld01ButtonPickerCreateArray(struct engineLayoutOld01ButtonPicker *this, char **textList, int textListLength);

// ピッカーボタン構造体 描画
void engineLayoutOld01ButtonPickerDraw(struct engineLayoutOld01ButtonPicker *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// ピッカーボタン構造体 破棄
void engineLayoutOld01ButtonPickerDispose(struct engineLayoutOld01ButtonPicker *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

