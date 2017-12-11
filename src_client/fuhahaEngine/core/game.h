#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にゲーム側から呼び出す

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

// 戻るボタンの制御を行うか
bool gameMainEventKeyIsBack(void);
// 傾きを使うかどうか
bool gameMainEventIsAcceleration(void);

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

