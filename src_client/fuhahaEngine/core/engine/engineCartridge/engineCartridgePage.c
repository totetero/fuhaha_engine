#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	void (*createFirstCartridge)(void);
	struct engineCartridgePage *childPageList;
	bool isLoadedSystem;
} localGlobal = {0};

// ----------------------------------------------------------------

// ゲームのページ管理 初期化
void engineCartridgePageManagerInit(void(*createFirstCartridge)(void)){
	localGlobal.createFirstCartridge = createFirstCartridge;
}

// ゲームのページ管理 計算
void engineCartridgePageManagerCalc(void){
	if(!localGlobal.isLoadedSystem){
		// 最初のシステム画像ロード完了確認
		if(platformPluginUtilIsLoading()){return;}
		localGlobal.isLoadedSystem = true;
		// 最初のページカートリッジ装填
		if(localGlobal.childPageList == NULL){localGlobal.createFirstCartridge();}
	}

	// ゲームカートリッジ計算
	struct engineCartridgePage *cartridge = localGlobal.childPageList;
	while(cartridge != NULL){
		cartridge->calc(cartridge);
		if(cartridge->isExist){break;}

		// 中断処理
		cartridge->pause(cartridge);
		// 終了処理
		localGlobal.childPageList = cartridge->next;
		cartridge->dispose(cartridge);
		// 次のカートリッジを初期化
		cartridge = localGlobal.childPageList;
		if(cartridge != NULL){cartridge->init(cartridge);}
	};
}

// ゲームのページ管理 描画
void engineCartridgePageManagerDraw(void){
	if(!localGlobal.isLoadedSystem){
		// 最初のシステム画像ロード中
		engineGraphicEngineClearAll();
		engineGraphicEngineFlush();
	}else{
		// ゲームカートリッジ描画
		struct engineCartridgePage *cartridge = localGlobal.childPageList;
		if(cartridge != NULL){cartridge->draw(cartridge);}
	}
}

// ゲームのページ管理 一時停止
// initを通ったら必ずpauseを通る
void engineCartridgePageManagerPause(void){
	struct engineCartridgePage *cartridge = localGlobal.childPageList;
	if(cartridge != NULL){cartridge->pause(cartridge);}
}

// ゲームのページ管理 破棄
// pushをしたら必ずdisposeを通る
void engineCartridgePageManagerDispose(void){
	while(localGlobal.childPageList != NULL){
		struct engineCartridgePage *cartridge = localGlobal.childPageList;
		localGlobal.childPageList = cartridge->next;
		cartridge->dispose(cartridge);
	}
	localGlobal.isLoadedSystem = false;
}

// ----------------------------------------------------------------

// ゲームにページカートリッジを装填
void engineCartridgePageManagerPush(struct engineCartridgePage *cartridge){
	if(localGlobal.childPageList == NULL){
		localGlobal.childPageList = cartridge;
		cartridge->init(cartridge);
	}else{
		struct engineCartridgePage *temp = localGlobal.childPageList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = cartridge;
	}
	cartridge->next = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

