#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageTest2CartridgePage{
	struct engineCartridgePage super;
	struct engineCartridgePopupManager popupManager;
};

// ページカートリッジ構造体 作成
struct pageTest2CartridgePage *pageTest2CartridgePageCreate(void *param, struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePage *page, void *param), void(*paramDispose)(void *param));

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

