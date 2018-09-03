#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	void *buff;
	size_t size;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 揮発性一時バッファ 返値領域は解放禁止
void *corePluginUtilMemoryTemporary(size_t size){
	if(localGlobal.size < size){
		if(localGlobal.size > 0){engineUtilMemoryInfoFree("(permanent) volatility buffer", localGlobal.buff);}
		localGlobal.size = size;
		localGlobal.buff = engineUtilMemoryInfoMalloc("(permanent) volatility buffer", localGlobal.size);
	}
	return localGlobal.buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 領域確保mallocラッパ
void *gamePluginUtilMemoryMalloc(char *info, size_t size){
	return engineUtilMemoryInfoMalloc(info, size);
}

// 領域確保callocラッパ
void *gamePluginUtilMemoryCalloc(char *info, size_t n, size_t size){
	return engineUtilMemoryInfoCalloc(info, n, size);
}

// 領域確保確認
void gamePluginUtilMemoryTrace(void){
	engineUtilMemoryTrace("plugin trace");
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

