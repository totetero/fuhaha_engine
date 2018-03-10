#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スクローラ構造体
struct engineLayoutViewPartsScroller{
	struct engineLayoutView super;
	struct{double x; double y;} position;
	struct{double x; double y;} velocity;
	struct{double w; double h;} inner;
	struct{struct engineLayoutViewPartsScroller *parentScroller; bool isChildrenMove;} bubbling;
	struct engineLayoutView *viewInner;
	struct engineLayoutView *viewBarX;
	struct engineLayoutView *viewBarY;
};

// スクローラ構造体 作成
struct engineLayoutViewPartsScroller *engineLayoutViewPartsScrollerCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

