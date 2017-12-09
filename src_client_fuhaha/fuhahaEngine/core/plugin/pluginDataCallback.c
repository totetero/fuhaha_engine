#include "../library.h"
#include "../plugin/pluginUtil.h"
#include "../plugin/pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginDataHttpCallbackId gamePluginDataHttpCallbackSet(void *param, void(*callback)(void *param, PLUGINDATA_HTTP_CALLBACKPARAMS)){
	return (pluginDataHttpCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// コールバック関数の登録
pluginDataLocalCallbackId gamePluginDataLocalCallbackSet(void *param, void(*callback)(void *param, PLUGINDATA_LOCAL_CALLBACKPARAMS)){
	return (pluginDataLocalCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// ----------------------------------------------------------------

// コールバック関数の実行と解放
bool gamePluginDataHttpCallbackCall(pluginDataHttpCallbackId callbackId, PLUGINDATA_HTTP_CALLBACKPARAMS){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param, true);
	if(callback == NULL){return false;}
	((void(*)(void*, void*, size_t))callback)(param, buff, size);
	return true;
}

// コールバック関数の実行と解放
bool gamePluginDataLocalCallbackCall(pluginDataLocalCallbackId callbackId, PLUGINDATA_LOCAL_CALLBACKPARAMS){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param, true);
	if(callback == NULL){return false;}
	((void(*)(void*, void*, size_t))callback)(param, buff, size);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

