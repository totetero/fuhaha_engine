#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体
struct engineLayoutView{
	struct engineLayoutViewGearFamily family;
	struct engineLayoutViewGearPosition position;
	struct engineLayoutViewGearInteract interact;
	struct engineLayoutViewGearGraphicObject graphicObject;

	struct engineMathVector4 color;

	bool (*touch)(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
	void (*calc)(struct engineLayoutView *this, bool isCancel);
	void (*draw)(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*pause)(struct engineLayoutView *this);
	void (*dispose)(struct engineLayoutView *this);
};

// 表示要素構造体 ユーティリティの初期化
void engineLayoutViewGearInit(struct engineLayoutView *this);
// 表示要素構造体 ユーティリティの破棄
void engineLayoutViewGearDispose(struct engineLayoutView *this);

// 表示要素構造体 デフォルト関数
bool engineLayoutViewDefaultTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
void engineLayoutViewDefaultCalc(struct engineLayoutView *this, bool isCancel);
void engineLayoutViewDefaultDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
void engineLayoutViewDefaultPause(struct engineLayoutView *this);
void engineLayoutViewDefaultDispose(struct engineLayoutView *this);

// 表示要素構造体 作成
struct engineLayoutView *engineLayoutViewCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

