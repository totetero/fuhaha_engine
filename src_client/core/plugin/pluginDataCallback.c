#include "../library.h"
#include "platform.h"
#include "pluginUtil.h"
#include "pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
int gamePluginDataCallbackSet(void *param, void(*callback)(void *param, void *buff, size_t size)){
	return corePluginUtilCallbackSet(param, callback);
}

// コールバック関数の実行と解放
bool gamePluginDataCallbackCall(int callbackId, void *buff, size_t size){
	void *param;
	void(*callback)(void *param, void *buff, size_t size) = corePluginUtilCallbackGet(callbackId, &param);
	if(callback == NULL){return false;}
	callback(param, buff, size);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

