#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態初期化
void pageTest2PushPage(){
	// ページカートリッジ作成
	struct engineCartridgeLayoutPage *cartridge = engineCartridgeLayoutPageCreate();
	engineCartridgeLayoutPageSetView(cartridge, (struct engineLayoutView*)pageTest2PartsRootCreate(cartridge));
	// ページカートリッジ装填
	engineCartridgePageManagerPush((struct engineCartridgePage*)cartridge);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

