#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 十字キー構造体
struct engineLayoutPartsArrowKey{
	struct engineLayoutView super;
	bool isUp;
	bool isDn;
	bool isRt;
	bool isLt;
	struct engineLayoutPartsRect *buttonUpNormal;
	struct engineLayoutPartsRect *buttonDnNormal;
	struct engineLayoutPartsRect *buttonRtNormal;
	struct engineLayoutPartsRect *buttonLtNormal;
	struct engineLayoutPartsRect *buttonUpActibve;
	struct engineLayoutPartsRect *buttonDnActibve;
	struct engineLayoutPartsRect *buttonRtActibve;
	struct engineLayoutPartsRect *buttonLtActibve;
};

// 十字キー構造体 作成
struct engineLayoutPartsArrowKey *engineLayoutPartsArrowKeyCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

