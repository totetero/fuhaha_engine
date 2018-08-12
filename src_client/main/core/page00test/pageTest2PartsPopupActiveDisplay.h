#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// アクティブポップアップディスプレイ構造体
struct pageTest2PartsPopupActiveDisplay{
	struct engineLayoutView super;
	struct engineLayoutViewPartsArrowKey *viewArrowKey;
};

// アクティブポップアップディスプレイ構造体 作成
struct pageTest2PartsPopupActiveDisplay *pageTest2PartsPopupActiveDisplayCreate(struct pageTest2Status *stat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

