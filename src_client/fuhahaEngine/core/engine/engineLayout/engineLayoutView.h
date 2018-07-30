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

	struct engineMathVector4 color;

	bool (*touch)(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
	void (*calc)(struct engineLayoutView *this, bool isCancel);
	void (*draw)(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*pause)(struct engineLayoutView *this);
	void (*dispose)(struct engineLayoutView *this);
};

// 表示要素構造体 ユーティリティの初期化
void engineLayoutViewUtilInit(struct engineLayoutView *this);
// 表示要素構造体 ユーティリティの破棄
void engineLayoutViewUtilDispose(struct engineLayoutView *this);

// 表示要素構造体 作成
struct engineLayoutView *engineLayoutViewCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

