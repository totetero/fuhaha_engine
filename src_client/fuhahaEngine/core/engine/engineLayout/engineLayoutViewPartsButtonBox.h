#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 標準ボタン構造体
struct engineLayoutViewPartsButtonBox{
	struct engineLayoutView super;
	bool isSelect;
	bool isInactiveDraw;
	struct engineLayoutViewPartsFrame *frameNormal;
	struct engineLayoutViewPartsFrame *frameSelect;
	struct engineLayoutViewPartsFrame *frameActibve;
	struct engineLayoutViewPartsFrame *frameInactive;
	struct engineLayoutView *viewInner;
};

// 標準ボタン構造体 作成
struct engineLayoutViewPartsButtonBox *engineLayoutViewPartsButtonBoxCreate(void);
struct engineLayoutViewPartsButtonBox *engineLayoutViewPartsButtonBoxCreateText(char *text);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

