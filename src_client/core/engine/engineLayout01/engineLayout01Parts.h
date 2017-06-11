#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン構造体
struct engineLayout01Button{
	double x;
	double y;
	double w;
	double h;
	bool active;
	bool inactive;
	bool trigger;
	struct engineLayout01Button *child;
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
void engineLayout01ButtonInit(struct engineLayout01Button *this);
// ボタン構造体 静的位置設定
void engineLayout01ButtonSetPosition(struct engineLayout01Button *this, double x, double y, double w, double h);
// ボタン構造体 領域内確認行列読み込み
void engineLayout01ButtonSetMatrix(struct engineLayout01Button *this, struct engineMathMatrix44 *mat);
// ボタン構造体 計算
void engineLayout01ButtonCalc(struct engineLayout01Button *this);
// ボタン構造体 タッチ情報を使い回す計算
void engineLayout01ButtonSubCalc(struct engineLayout01Button *this, struct engineCtrlTouch *t, bool clickable);
// ボタン構造体 タッチ領域内確認
bool engineLayout01ButtonIsInner(struct engineLayout01Button *this, struct engineCtrlTouch *t);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示箱付きボタン構造体
struct engineLayout01ButtonBox{
	struct engineLayout01Button super;
	struct engineLayout01Trans trans;
	struct engineLayout01ImageFrame imgNormal;
	struct engineLayout01ImageFrame imgSelect;
	struct engineLayout01ImageFrame imgActibve;
	struct engineLayout01ImageFrame imgInactive;
	struct engineLayout01ImageText imgText;
	bool select;
};

// 表示箱付きボタン構造体 初期化
void engineLayout01ButtonBoxInit(struct engineLayout01ButtonBox *this);
// 表示箱付きボタン構造体 静的位置設定
void engineLayout01ButtonBoxSetPosition(struct engineLayout01ButtonBox *this, double x, double y, double w, double h);
// 表示箱付きボタン構造体 計算
void engineLayout01ButtonBoxCalc(struct engineLayout01ButtonBox *this);
// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void engineLayout01ButtonBoxSubCalc(struct engineLayout01ButtonBox *this, struct engineCtrlTouch *t, bool clickable);
// 配列に表示箱付きボタンの描画情報を追加
void engineLayout01ButtonBoxCreateArray(struct engineLayout01ButtonBox *this, char *text);

// 表示箱付きボタン構造体 描画
void engineLayout01ButtonBoxDraw(struct engineLayout01ButtonBox *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示箱付きボタン構造体 破棄
void engineLayout01ButtonBoxDispose(struct engineLayout01ButtonBox *this);

// ----------------------------------------------------------------

// ピッカーボタン構造体
struct engineLayout01ButtonPicker{
	struct engineLayout01Button super;
	struct engineLayout01Trans trans;
	struct engineLayout01ImageRect imgRectNormal;
	struct engineLayout01ImageRect imgRectSelect;
	struct engineLayout01ImageRect imgRectActibve;
	struct engineLayout01ImageRect imgRectInactive;
	struct engineLayout01ImageFrame imgFrameNormal;
	struct engineLayout01ImageFrame imgFrameSelect;
	struct engineLayout01ImageFrame imgFrameActibve;
	struct engineLayout01ImageFrame imgFrameInactive;
	struct engineLayout01ImageText imgText[8];
	int picked;
	bool select;
};

// ピッカーボタン構造体 初期化
void engineLayout01ButtonPickerInit(struct engineLayout01ButtonPicker *this, int picked);
// ピッカーボタン構造体 静的位置設定
void engineLayout01ButtonPickerSetPosition(struct engineLayout01ButtonPicker *this, double x, double y, double w);
// ピッカーボタン構造体 計算
void engineLayout01ButtonPickerCalc(struct engineLayout01ButtonPicker *this);
// ピッカーボタン構造体 タッチ情報を使い回す計算
void engineLayout01ButtonPickerSubCalc(struct engineLayout01ButtonPicker *this, struct engineCtrlTouch *t, bool clickable);
// 配列にピッカーボタンの描画情報を追加
void engineLayout01ButtonPickerCreateArray(struct engineLayout01ButtonPicker *this, char **textList, int textListLength);

// ピッカーボタン構造体 描画
void engineLayout01ButtonPickerDraw(struct engineLayout01ButtonPicker *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// ピッカーボタン構造体 破棄
void engineLayout01ButtonPickerDispose(struct engineLayout01ButtonPicker *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

