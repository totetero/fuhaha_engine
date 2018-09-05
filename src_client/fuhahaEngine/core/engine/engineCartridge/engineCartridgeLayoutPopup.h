#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct engineCartridgeLayoutPopup{
	struct engineCartridgePopup super;
	struct engineCartridgePopupManager popupManager;
	int result;
};

// ポップアップカートリッジ構造体 作成
struct engineCartridgeLayoutPopup *engineCartridgeLayoutPopupCreate(void);
// ポップアップカートリッジ構造体 設定
void engineCartridgeLayoutPopupSetView(struct engineCartridgeLayoutPopup *this, struct engineLayoutView *view);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

