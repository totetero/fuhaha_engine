#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スワイプ構造体
struct engineLayoutSwipe{
	struct engineLayoutSwipe *parentSwipe;
	bool isChildrenMove;
	int x;
	int y;
};

// スワイプ構造体 タッチ処理
bool engineLayoutSwipeTouch(struct engineLayoutSwipe *this, struct engineLayoutView *view, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

