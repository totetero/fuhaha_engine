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
		int x;
		int y;
	} window;
	struct{
		int x;
		int y;
	} screen;
	struct activatable dn;
};

// グローバル変数
extern struct global{
	// ウインドウ 画面全体
	struct{
		int w;
		int h;
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
	struct touch touch[2];
	// キー状態
	struct{
		struct activatable bk;
		struct activatable up;
		struct activatable dn;
		struct activatable rt;
		struct activatable lt;
		struct activatable zb;
		struct activatable xb;
		struct activatable cb;
		struct activatable vb;
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
void coreMainEventActivatableUpdate(struct activatable *this, bool isActive);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// opengl初期化
void gameMainSurfaceCreated(void);
// opengl描画
void gameMainSurfaceDrawFrame(void);
// opengl中断
void gameMainSurfacePause(void);
// opengl終了
void gameMainSurfaceDestroy(void);

// ユーザーインターフェイスイベント初期化
void gameMainEventInit(void);
// 画面サイズの状態を報告する
void gameMainEventScreen(int width, int height, double pixelRatio);
// タッチの状態を報告する
void gameMainEventTouch(int index, int tx, int ty, bool tdn);
// キーの押下状態変化を報告する
void gameMainEventKeyBack(bool kbk);
void gameMainEventKeyArrow(bool kup, bool kdn, bool krt, bool klt);
void gameMainEventKeyZxcv(bool kzb, bool kxb, bool kcb, bool kvb);
// 戻るボタンの制御を行うか
bool gameMainEventKeyIsBack(void);
// 傾きを使うかどうか
bool gameMainEventIsAcceleration(void);
// 傾きの状態を報告する
void gameMainEventAcceleration(double accx, double accy, double accz);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// アプリ終了命令
void platformMainSurfaceExit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

