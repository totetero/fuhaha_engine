#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体
struct engineLayoutView{
	struct engineLayoutViewUtilFamily family;
	struct engineLayoutViewUtilPosition position;
	struct engineLayoutViewUtilInteract interact;

	bool (*touch)(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
	void (*calc)(struct engineLayoutView *this);
	void (*draw)(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*pause)(struct engineLayoutView *this);
	void (*dispose)(struct engineLayoutView *this);
};

// 表示要素構造体 作成
struct engineLayoutView *engineLayoutViewCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ユーティリティマクロ

#define engineLayoutViewTouch(this, touchId, x, y, dn, mv, isCancel) (this)->touch(this, touchId, x, y, dn, mv, isCancel)
#define engineLayoutViewCalc(this) (this)->calc(this)
#define engineLayoutViewDraw(this, mat, color) (this)->draw(this, mat, color)
#define engineLayoutViewPause(this) (this)->pause(this)
#define engineLayoutViewDispose(this) (this)->dispose(this)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

