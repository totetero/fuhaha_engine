#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スクローラ構造体
struct engineLayoutPartsSwipeScroller{
	struct engineLayoutView super;
	struct engineLayoutUtilSwipe swipe;
	struct{double x; double y;} position;
	struct{double x; double y;} velocity;
	struct{double w; double h;} inner;
	struct engineLayoutView *viewInner;
	struct engineLayoutView *viewBarX;
	struct engineLayoutView *viewBarY;
};

// スクローラ構造体 作成
struct engineLayoutPartsSwipeScroller *engineLayoutPartsSwipeScrollerCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

