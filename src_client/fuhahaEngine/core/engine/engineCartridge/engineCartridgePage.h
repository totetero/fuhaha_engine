#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体 継承して使う
struct engineCartridgePage{
	struct engineCartridgePage *next;
	bool isExist;

	void (*init)(struct engineCartridgePage *this);
	void (*calc)(struct engineCartridgePage *this);
	void (*draw)(struct engineCartridgePage *this);
	void (*pause)(struct engineCartridgePage *this);
	void (*dispose)(struct engineCartridgePage *this);
};

// ゲームのページ管理 初期化
void engineCartridgePageManagerInit(void(*createFirstCartridge)());
// ゲームのページ管理 計算
void engineCartridgePageManagerCalc(void);
// ゲームのページ管理 描画
void engineCartridgePageManagerDraw(void);
// ゲームのページ管理 一時停止
void engineCartridgePageManagerPause(void);
// ゲームのページ管理 破棄
void engineCartridgePageManagerDispose(void);

// ゲームにページカートリッジを装填
void engineCartridgePageManagerPush(struct engineCartridgePage *cartridge);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

