#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態初期化
void pageTest2PushPage(){
	// ページカートリッジ作成
	struct engineCartridgeLayoutPage *cartridgePage = engineCartridgeLayoutPageCreate();
	engineCartridgeLayoutPageSetView(cartridgePage, (struct engineLayoutView*)pageTest2PartsRootCreate(cartridgePage));
	// ページカートリッジ装填
	engineCartridgePageManagerPush((struct engineCartridgePage*)cartridgePage);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

