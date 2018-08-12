#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct pageTest2CartridgePopup{
	struct engineCartridgePopup super;
	struct engineCartridgePopupManager popupManager;
};

// ポップアップカートリッジ構造体 作成
struct pageTest2CartridgePopup *pageTest2CartridgePopupCreate(void *param, struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePopup *popup, void *param), void(*paramDispose)(void *param));

// ポップアップカートリッジ構造体 結果設定
void pageTest2CartridgePopupResultSet(struct pageTest2CartridgePopup *this, int value);

// ポップアップカートリッジ構造体 結果取得
int pageTest2CartridgePopupResultGet(struct engineCartridgePopup *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

