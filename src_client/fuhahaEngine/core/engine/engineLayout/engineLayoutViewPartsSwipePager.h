#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページャー構造体
struct engineLayoutViewPartsSwipePager{
	struct engineLayoutView super;
	struct engineLayoutSwipe swipe;
	struct{double x;} position;
	struct{double x;} velocity;
	struct engineLayoutView *viewInner;
	bool isLoop;
	int count;
	int index;
};

// ページャー構造体 作成
struct engineLayoutViewPartsSwipePager *engineLayoutViewPartsSwipePagerCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

