#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページャー構造体
struct engineLayoutPartsSwipePager{
	struct engineLayoutView super;
	struct engineLayoutUtilSwipe swipe;
	struct{double x;} position;
	struct{double x;} velocity;
	struct engineLayoutView *viewInner;
	bool isLoop;
	int count;
	int index;
};

// ページャー構造体 作成
struct engineLayoutPartsSwipePager *engineLayoutPartsSwipePagerCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

