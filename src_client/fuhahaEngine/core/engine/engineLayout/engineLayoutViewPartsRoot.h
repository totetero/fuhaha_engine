#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ルート構造体
struct engineLayoutViewPartsRoot{
	struct engineLayoutView super;
};

// ルート構造体 作成
struct engineLayoutViewPartsRoot *engineLayoutViewPartsRootCreate(void);
// ルート構造体 運用
void engineLayoutViewPartsRootTouch(struct engineLayoutViewPartsRoot *this, bool isCancel);
void engineLayoutViewPartsRootCalc(struct engineLayoutViewPartsRoot *this, bool isCancel);
void engineLayoutViewPartsRootDraw(struct engineLayoutViewPartsRoot *this);
void engineLayoutViewPartsRootPause(struct engineLayoutViewPartsRoot *this);
void engineLayoutViewPartsRootDispose(struct engineLayoutViewPartsRoot *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

