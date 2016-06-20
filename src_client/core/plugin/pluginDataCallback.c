#include "../library.h"
#include "pluginUtil.h"
#include "pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginDataCallbackId gamePluginDataCallbackSet(void *param, void(*callback)(void *param, void *buff, size_t size)){
	return (pluginDataCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// コールバック関数の実行と解放
bool gamePluginDataCallbackCall(pluginDataCallbackId callbackId, void *buff, size_t size){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param);
	if(callback == NULL){return false;}
	((void(*)(void*, void*, size_t))callback)(param, buff, size);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

