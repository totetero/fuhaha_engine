#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- タッチ処理

// タッチ状態構造体
struct engineCtrlTouch{
	struct{
		// ウインドウ内位置
		int16_t x;
		int16_t y;
	} window;
	struct{
		// スクリーン内位置
		int16_t x;
		int16_t y;
	} screen;
	// 押下中フラグ
	bool dn;
	// 移動中フラグ
	bool mv;
	// 占有中フラグ
	bool active;
};

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

// コントローラ状態メインループ計算
void engineCtrlTouchCalc(void);
// タッチ情報取得
struct engineCtrlTouch *engineCtrlTouchGet(enum engineCtrlTouchId touchId);
// 取得したタッチ情報占有
void engineCtrlTouchOwn(void);
// 取得して占有したタッチ情報の解放
void engineCtrlTouchFree(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

