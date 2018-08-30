#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スワイプ構造体
struct engineLayoutUtilSwipe{
	struct engineLayoutUtilSwipe *parentSwipe;
	bool isChildrenMove;
	int x;
	int y;
};

// スワイプ構造体 タッチ処理
bool engineLayoutUtilSwipeTouch(struct engineLayoutUtilSwipe *this, struct engineLayoutView *view, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

