#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 標準ボタン構造体
struct engineLayoutPartsButtonBox{
	struct engineLayoutView super;
	bool isSelect;
	bool isInactiveDraw;
	struct engineLayoutPartsFrame *frameNormal;
	struct engineLayoutPartsFrame *frameSelect;
	struct engineLayoutPartsFrame *frameActibve;
	struct engineLayoutPartsFrame *frameInactive;
	struct engineLayoutView *viewInner;
};

// 標準ボタン構造体 作成
struct engineLayoutPartsButtonBox *engineLayoutPartsButtonBoxCreate(void);
struct engineLayoutPartsButtonBox *engineLayoutPartsButtonBoxCreateText(char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

