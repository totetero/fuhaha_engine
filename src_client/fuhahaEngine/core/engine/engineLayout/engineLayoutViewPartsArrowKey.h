#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 十字キー構造体
struct engineLayoutViewPartsArrowKey{
	struct engineLayoutView super;
	bool isUp;
	bool isDn;
	bool isRt;
	bool isLt;
	struct engineLayoutViewPartsRect *buttonUpNormal;
	struct engineLayoutViewPartsRect *buttonDnNormal;
	struct engineLayoutViewPartsRect *buttonRtNormal;
	struct engineLayoutViewPartsRect *buttonLtNormal;
	struct engineLayoutViewPartsRect *buttonUpActibve;
	struct engineLayoutViewPartsRect *buttonDnActibve;
	struct engineLayoutViewPartsRect *buttonRtActibve;
	struct engineLayoutViewPartsRect *buttonLtActibve;
};

// 十字キー構造体 作成
struct engineLayoutViewPartsArrowKey *engineLayoutViewPartsArrowKeyCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

