#include "../../library.h"
#include "engineCartridge.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップ初期化
static void popupInit(struct engineCartridgePopup *this){
	this->openStep = this->openStepMax;
	this->openParam = 1.0;
	this->openSize = 1.5;
	this->openAlpha = 0.0;
	this->backParam = this->isContinuousOpen ? 0.0 : 1.0;
	this->backAlpha = this->isContinuousOpen ? 1.0 : 0.0;
	this->isOpening = true;
	this->isClosing = false;
	this->init(this);
}

// ポップアップ展開計算 返値で生存確認
static bool popupCalc(struct engineCartridgePopup *this){
	bool isTransition = (this->openStepMax > 0);

	if(isTransition){
		// 開遷移開始確認
		bool isOpenStart = (this->openStep >= this->openStepMax);
		if(isOpenStart){this->openStep = -this->openStepMax;}
		// 遷移処理
		if(this->openStep != 0){this->openStep++;}
	}

	// 通常処理
	this->calc(this);
	bool isExist = this->isExist;
	bool isCloseStart = !isExist;
	bool isCloseEnd = !isExist;

	if(isTransition){
		// 閉遷移開始確認
		isCloseStart = (this->openStep <= 0 && !isExist);
		if(isCloseStart && this->openStep == 0){this->openStep = 1;}
		if(isCloseStart && this->openStep < 0){this->openStep *= -1;}
		// 閉遷移完了確認
		isCloseEnd = (this->openStep >= this->openStepMax);

		// 遷移描画状態計算
		double param = (double)this->openStep / this->openStepMax;
		this->isOpening = (this->openStep < 0 || this->openStepMax <= this->openStep);
		this->isClosing = (0 < this->openStep && this->openStep < this->openStepMax);
		this->openParam = param;
		this->openSize = 1 + 0.5 * param * param;
		this->openAlpha = 1 - param * param;
		bool isContinuous = ((this->isOpening && this->isContinuousOpen) || (this->isClosing && this->isContinuousClose));
		this->backParam = isContinuous ? 0.0 : param;
		this->backAlpha = (isContinuous ? 1.0 : this->openAlpha) * 0.5;
	}

	// 閉遷移開始時の処理
	if(isCloseStart && this->onClose != NULL){this->onClose(this);}

	isExist = !isCloseEnd;
	return isExist;
}

// ----------------------------------------------------------------

// ポップアップ管理構造体 初期化
void engineCartridgePopupManagerInit(struct engineCartridgePopupManager *this){
}

// ポップアップ管理構造体 計算
bool engineCartridgePopupManagerCalc(struct engineCartridgePopupManager *this){
	// ポップアップカートリッジ計算
	struct engineCartridgePopup *cartridge = this->popupCartridgeList;
	while(cartridge != NULL){
		bool isExist = popupCalc(cartridge);
		if(isExist){break;}

		// 中断処理
		cartridge->pause(cartridge);
		// 終了処理
		this->popupCartridgeList = cartridge->next;
		cartridge->dispose(cartridge);
		// 次のカートリッジを初期化
		cartridge = this->popupCartridgeList;
		if(cartridge != NULL){popupInit(cartridge);}
	}

	return (cartridge != NULL);
}

// ポップアップ管理構造体 描画
void engineCartridgePopupManagerDraw(struct engineCartridgePopupManager *this){
	struct engineCartridgePopup *cartridge = this->popupCartridgeList;
	if(cartridge != NULL){cartridge->draw(cartridge);}
}

// ポップアップ管理構造体 一時停止
// initを通ったら必ずpauseを通る
void engineCartridgePopupManagerPause(struct engineCartridgePopupManager *this){
	struct engineCartridgePopup *cartridge = this->popupCartridgeList;
	if(cartridge != NULL){cartridge->pause(cartridge);}
}

// ポップアップ管理構造体 破棄
// pushをしたら必ずdisposeを通る
void engineCartridgePopupManagerDispose(struct engineCartridgePopupManager *this){
	while(this->popupCartridgeList != NULL){
		struct engineCartridgePopup *cartridge = this->popupCartridgeList;
		this->popupCartridgeList = cartridge->next;
		cartridge->dispose(cartridge);
	}
}

// ----------------------------------------------------------------

// ポップアップ管理構造体にポップアップカートリッジを装填
void engineCartridgePopupManagerPush(struct engineCartridgePopupManager *this, struct engineCartridgePopup *cartridge){
	cartridge->openStepMax = 10;

	if(this->popupCartridgeList == NULL){
		this->popupCartridgeList = cartridge;
		popupInit(cartridge);
	}else{
		struct engineCartridgePopup *temp = this->popupCartridgeList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = cartridge;
		// 連続フラグを立てる
		temp->isContinuousClose = true;
		temp->next->isContinuousOpen = true;
	}
	cartridge->next = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

