#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体 継承して使う
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
	double backParam;
	double backAlpha;
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

