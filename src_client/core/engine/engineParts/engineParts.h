#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン構造体
struct enginePartsButton{
	double x;
	double y;
	double w;
	double h;
	bool active;
	bool inactive;
	bool trigger;
	struct enginePartsButton *child;
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
void enginePartsButtonInit(struct enginePartsButton *this);
// ボタン構造体 静的位置設定
void enginePartsButtonSetPosition(struct enginePartsButton *this, double x, double y, double w, double h);
// ボタン構造体 領域内確認行列読み込み
void enginePartsButtonSetMatrix(struct enginePartsButton *this, struct engineMathMatrix44 *mat);
// ボタン構造体 計算
void enginePartsButtonCalc(struct enginePartsButton *this);
// ボタン構造体 タッチ情報を使い回す計算
void enginePartsButtonSubCalc(struct enginePartsButton *this, struct engineCtrlTouch *t, bool clickable);
// ボタン構造体 タッチ領域内確認
bool enginePartsButtonIsInner(struct enginePartsButton *this, struct engineCtrlTouch *t);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示箱付きボタン構造体
struct enginePartsButtonBox{
	struct enginePartsButton super;
	struct engineGraphicTrans trans;
	struct engineGraphicImageFrame imgNormal;
	struct engineGraphicImageFrame imgSelect;
	struct engineGraphicImageFrame imgActibve;
	struct engineGraphicImageFrame imgInactive;
	struct engineGraphicImageText imgText;
	bool select;
};

// 表示箱付きボタン構造体 初期化
void enginePartsButtonBoxInit(struct enginePartsButtonBox *this);
// 表示箱付きボタン構造体 静的位置設定
void enginePartsButtonBoxSetPosition(struct enginePartsButtonBox *this, double x, double y, double w, double h);
// 表示箱付きボタン構造体 計算
void enginePartsButtonBoxCalc(struct enginePartsButtonBox *this);
// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void enginePartsButtonBoxSubCalc(struct enginePartsButtonBox *this, struct engineCtrlTouch *t, bool clickable);
// 配列に表示箱付きボタンの描画情報を追加
void enginePartsButtonBoxCreateArray(struct enginePartsButtonBox *this, char *text);

// 表示箱付きボタン構造体 描画
void enginePartsButtonBoxDraw(struct enginePartsButtonBox *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示箱付きボタン構造体 破棄
void enginePartsButtonBoxDispose(struct enginePartsButtonBox *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

