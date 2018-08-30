#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ルート構造体
struct engineLayoutPartsRoot{
	struct engineLayoutView super;
};

// ルート構造体 作成
struct engineLayoutPartsRoot *engineLayoutPartsRootCreate(void);

// ルート構造体 運用
void engineLayoutPartsRootTouch(struct engineLayoutPartsRoot *this, bool isCancel);
void engineLayoutPartsRootCalc(struct engineLayoutPartsRoot *this, bool isCancel);
void engineLayoutPartsRootDraw(struct engineLayoutPartsRoot *this);
void engineLayoutPartsRootPause(struct engineLayoutPartsRoot *this);
void engineLayoutPartsRootDispose(struct engineLayoutPartsRoot *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

