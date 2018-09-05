#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態構造体
struct pageTest2Status{
	struct engineCartridgeLayoutPage *cartridge;
	struct engineLayoutPartsSwipePager *viewPager;
};

// ページカートリッジ構造体 作成
struct pageTest2Status *pageTest2StatusCreate(void);

// ページカートリッジ構造体 破棄
void pageTest2StatusDispose(struct pageTest2Status *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

