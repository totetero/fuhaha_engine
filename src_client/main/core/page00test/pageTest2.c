#include "fuhahaEngine.h"
#include "pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態初期化
void pageTest2PushPage(){
	// ステータス初期化
	struct pageTest2Status *stat = pageTest2StatusCreate();
	// ページカートリッジ作成
	struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePage*, void*) = (struct engineLayoutView*(*)(struct pageTest2CartridgePage*, void*))pageTest2PartsPagerCreate;
	void(*paramDispose)(void *param) = (void(*)(void *param))pageTest2StatusDispose;
	stat->cartridge = pageTest2CartridgePageCreate(stat, viewCreate, paramDispose);
	// ページカートリッジ装填
	engineCartridgePageManagerPush((struct engineCartridgePage*)stat->cartridge);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

