#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素
struct engineLayoutViewGearFamily{
	struct engineLayoutView *parent;
	struct engineLayoutView *prev;
	struct engineLayoutView *next;
	struct engineLayoutView *childrenHead;
	struct engineLayoutView *childrenTail;
	struct{int frameCount; bool isFirstCalc;} addInfo;
	double zIndex;
	bool isInactive;
	bool isInvisible;
};

// 表示要素構造体子要素 初期化
void engineLayoutViewGearFamilyInit(struct engineLayoutView *this);
// 表示要素構造体子要素 子要素追加
void engineLayoutViewGearFamilyAdd(struct engineLayoutView *this, struct engineLayoutView *child);
// 表示要素構造体子要素 子要素除外
void engineLayoutViewGearFamilyRemove(struct engineLayoutView *this, struct engineLayoutView *child, bool isDispose);
// 表示要素構造体子要素 破棄
void engineLayoutViewGearFamilyDispose(struct engineLayoutView *this);

// 表示要素構造体子要素 タッチ処理
bool engineLayoutViewGearChildrenTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
// 表示要素構造体子要素 計算
void engineLayoutViewGearChildrenCalc(struct engineLayoutView *this, bool isCancel);
// 表示要素構造体子要素 描画
void engineLayoutViewGearChildrenDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示要素構造体子要素 一時停止
void engineLayoutViewGearChildrenPause(struct engineLayoutView *this);
// 表示要素構造体子要素 破棄
void engineLayoutViewGearChildrenDispose(struct engineLayoutView *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

