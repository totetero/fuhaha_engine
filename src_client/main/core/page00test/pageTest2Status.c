#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体 作成
struct pageTest2Status *pageTest2StatusCreate(){
	struct pageTest2Status *this = (struct pageTest2Status*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2Status));
	return this;
}

// ページカートリッジ構造体 破棄
void pageTest2StatusDispose(struct pageTest2Status *this){
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

