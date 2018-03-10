#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体対話型
struct engineLayoutViewUtilInteract{
	struct{
		bool isTouchable;
		bool isMoveableX;
		bool isMoveableY;
	} setting;
	struct{
		int touchIndex;
		bool isActive;
		bool isHover;
		bool isMove;
		bool isTriggerDn;
		bool isTriggerUp;
		int startX;
		int startY;
		int currX;
		int currY;
	} status;
};

// 表示要素構造体対話型 タッチ処理
bool engineLayoutViewUtilInteractTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);

// 表示要素構造体対話型 基盤タッチ処理
void engineLayoutViewUtilInteractTouchRoot(struct engineLayoutView *this, bool isCancel);

// 表示要素構造体対話型 ボタンタッチ確認
bool engineLayoutViewUtilInteractIsTriggerUp(struct engineLayoutView *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

