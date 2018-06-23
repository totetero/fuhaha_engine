#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体
struct engineLayoutView{
	struct engineLayoutViewUtilFamily family;
	struct engineLayoutViewUtilPosition position;
	struct engineLayoutViewUtilInteract interact;
	struct engineLayoutViewUtilGraphicObject graphicObject;

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

