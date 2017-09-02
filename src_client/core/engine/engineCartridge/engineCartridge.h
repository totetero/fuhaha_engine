#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
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

// ポップアップカートリッジ構造体
struct engineCartridgePopup{
	struct engineCartridgePopup *next;
	bool isExist;

	void (*init)(struct engineCartridgePopup *this);
	void (*calc)(struct engineCartridgePopup *this);
	void (*draw)(struct engineCartridgePopup *this);
	void (*pause)(struct engineCartridgePopup *this);
	void (*dispose)(struct engineCartridgePopup *this);

	int openStep;
	int openStepMax;
	double openParam;
	double openSize;
	double openAlpha;
	double backParam;
	double backAlpha;
	bool isOpening;
	bool isClosing;
	bool isContinuousOpen;
	bool isContinuousClose;

	void *param;
	void (*onClose)(struct engineCartridgePopup *cartridge);
};

// ----------------------------------------------------------------

// ポップアップ管理構造体
struct engineCartridgePopupManager{
	struct engineCartridgePopup *popupCartridgeList;
};

// ポップアップ管理構造体 初期化
void engineCartridgePopupManagerInit(struct engineCartridgePopupManager *this);
// ポップアップ管理構造体 計算
bool engineCartridgePopupManagerCalc(struct engineCartridgePopupManager *this);
// ポップアップ管理構造体 描画
void engineCartridgePopupManagerDraw(struct engineCartridgePopupManager *this);
// ポップアップ管理構造体 一時停止
void engineCartridgePopupManagerPause(struct engineCartridgePopupManager *this);
// ポップアップ管理構造体 破棄
void engineCartridgePopupManagerDispose(struct engineCartridgePopupManager *this);

// ポップアップ管理構造体にポップアップカートリッジを装填
void engineCartridgePopupManagerPush(struct engineCartridgePopupManager *this, struct engineCartridgePopup *cartridge);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

