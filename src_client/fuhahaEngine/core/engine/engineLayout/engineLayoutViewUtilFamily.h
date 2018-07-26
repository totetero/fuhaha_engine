#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素
struct engineLayoutViewUtilFamily{
	struct engineLayoutView *parent;
	struct engineLayoutView *prev;
	struct engineLayoutView *next;
	struct engineLayoutView *childrenHead;
	struct engineLayoutView *childrenTail;
	double zIndex;
	bool isInactive;
	bool isInvisible;
};

// 表示要素構造体子要素 初期化
void engineLayoutViewUtilFamilyInit(struct engineLayoutView *this);
// 表示要素構造体子要素 子要素追加
void engineLayoutViewUtilFamilyAdd(struct engineLayoutView *this, struct engineLayoutView *child);
// 表示要素構造体子要素 子要素除外
void engineLayoutViewUtilFamilyRemove(struct engineLayoutView *this, struct engineLayoutView *child, bool isDispose);
// 表示要素構造体子要素 破棄
void engineLayoutViewUtilFamilyDispose(struct engineLayoutView *this);

// 表示要素構造体子要素 タッチ処理
bool engineLayoutViewUtilChildrenTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
// 表示要素構造体子要素 計算
void engineLayoutViewUtilChildrenCalc(struct engineLayoutView *this, bool isCancel);
// 表示要素構造体子要素 描画
void engineLayoutViewUtilChildrenDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示要素構造体子要素 一時停止
void engineLayoutViewUtilChildrenPause(struct engineLayoutView *this);
// 表示要素構造体子要素 破棄
void engineLayoutViewUtilChildrenDispose(struct engineLayoutView *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

