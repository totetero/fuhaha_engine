#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- グローバル変数

// 活性状態構造体
struct globalActivatable{
	bool isActive;
	bool triggerActive;
	bool triggerInactive;
};

// タッチ状態構造体
struct globalTouch{
	struct{
		int x;
		int y;
	} window;
	struct{
		int x;
		int y;
	} screen;
	struct globalActivatable dn;
};

// グローバル変数
extern struct globalVariable{
	// ウインドウ 画面全体
	struct{
		int w;
		int h;
		double pixelRatio;
	} window;
	// スクリーン 主要描画領域
	struct{
		int w;
		int h;
		// スクリーン比率
		struct{
			double x;
			double y;
		} aspect;
		// スクリーン位置
		struct{
			double x;
			double y;
		} offset;
	} screen;
	// タッチ状態
	struct globalTouch touch[2];
	// キー状態
	struct{
		struct globalActivatable bk;
		struct globalActivatable up;
		struct globalActivatable dn;
		struct globalActivatable rt;
		struct globalActivatable lt;
		struct globalActivatable zb;
		struct globalActivatable xb;
		struct globalActivatable cb;
		struct globalActivatable vb;
	} key;
	// 加速度状態
	struct{
		double x;
		double y;
		double z;
	} acc;
} global;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

