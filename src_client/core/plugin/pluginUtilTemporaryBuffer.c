#include "../library.h"
#include "pluginUtil.h"
#include "../engine/engineUtil/engineUtil.h"

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
void *corePluginUtilTemporaryBuffer(size_t size){
	if(localGlobal.size < size){
		if(localGlobal.size > 0){engineUtilMemoryFree(localGlobal.buff);}
		localGlobal.size = size;
		localGlobal.buff = engineUtilMemoryMalloc(localGlobal.size);
	}
	return localGlobal.buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

