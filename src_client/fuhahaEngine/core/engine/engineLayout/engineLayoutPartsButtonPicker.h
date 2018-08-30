#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ピッカーボタン構造体
struct engineLayoutPartsButtonPicker{
	struct engineLayoutView super;
	bool isSelect;
	bool isInactiveDraw;
	struct engineLayoutPartsRect *rectNormal;
	struct engineLayoutPartsRect *rectSelect;
	struct engineLayoutPartsRect *rectActibve;
	struct engineLayoutPartsRect *rectInactive;
	struct engineLayoutPartsFrame *frameNormal;
	struct engineLayoutPartsFrame *frameSelect;
	struct engineLayoutPartsFrame *frameActibve;
	struct engineLayoutPartsFrame *frameInactive;
	struct engineLayoutView *viewInner;
};

// ピッカーボタン構造体 作成
struct engineLayoutPartsButtonPicker *engineLayoutPartsButtonPickerCreate(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

