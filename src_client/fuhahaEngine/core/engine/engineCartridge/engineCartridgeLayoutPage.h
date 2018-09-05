#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct engineCartridgeLayoutPage{
	struct engineCartridgePage super;
	struct engineCartridgePopupManager popupManager;
};

// ページカートリッジ構造体 作成
struct engineCartridgeLayoutPage *engineCartridgeLayoutPageCreate(void);
// ページカートリッジ構造体 設定
void engineCartridgeLayoutPageSetView(struct engineCartridgeLayoutPage *this, struct engineLayoutView *view);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

