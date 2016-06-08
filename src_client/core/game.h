#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 活性状態構造体
struct activatable{
	bool isActive;
	bool triggerActive;
	bool triggerInactive;
};

// タッチ状態構造体
struct touch{
	struct{
		int16_t x;
		int16_t y;
	} window;
	struct{
		int16_t x;
		int16_t y;
	} screen;
	struct activatable dn;
};

// グローバル変数
extern struct global{
	// ウインドウ 画面全体
	struct{
		uint16_t w;
		uint16_t h;
	} window;
	// スクリーン 主要描画領域
	struct{
		uint16_t w;
		uint16_t h;
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
	struct touch touch[2];
	// キーボード状態
	struct{
		struct activatable up;
		struct activatable dn;
		struct activatable rt;
		struct activatable lt;
		struct activatable zb;
		struct activatable xb;
		struct activatable cb;
		struct activatable sb;
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
// -------- ゲーム側で実装 主にゲーム側から呼び出す

// 活性状態更新
void coreEventActivatableUpdate(struct activatable *this, bool isActive);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// opengl初期化
void gameSurfaceCreated(void);
// opengl画面サイズ変更
void gameSurfaceChanged(uint16_t width, uint16_t height, double pixelRatio);
// opengl描画
void gameSurfaceDrawFrame(void);
// opengl中断
void gameSurfacePause(void);
// opengl終了
void gameSurfaceDestroy(void);

// タッチの状態を報告する
void gameEvenTouch(uint8_t index, int16_t tx, int16_t ty, bool tdn);
// キーボードの押下状態変化を報告する
void gameEventKey(bool kup, bool kdn, bool krt, bool klt, bool kzb, bool kxb, bool kcb, bool ksb);
// 傾きの状態を報告する
void gameEventAcceleration(double accx, double accy, double accz);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

