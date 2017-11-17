#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- タッチ処理

// タッチ識別ID列挙型
enum engineCtrlTouchId{
	ENGINECTRLTOUCHID_NONE,
	ENGINECTRLTOUCHID_TEST1,
	ENGINECTRLTOUCHID_TEST2,
	ENGINECTRLTOUCHID_CONTROLLER,
	ENGINECTRLTOUCHID_BUTTON,
	ENGINECTRLTOUCHID_SCREEN,
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ボタン処理

// ボタン状態構造体
struct engineCtrlButton{
	struct{
		enum engineCtrlTouchId touchId;
		bool isInactiveMove;
		bool isInactiveCalc;
		bool isZKey;
		bool isXKey;
		bool isCKey;
		bool isVKey;
		bool isOutside;
	} setting;
	struct{
		bool isActive;
		bool isTrigger;
	} status;
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

