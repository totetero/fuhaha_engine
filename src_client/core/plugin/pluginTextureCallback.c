#include "../library.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginTextureCallbackId gamePluginTextureCallbackSet(void *param, void(*callback)(void *param, int glId, int w, int h)){
	return (pluginTextureCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// コールバック関数の実行と解放
bool gamePluginTextureCallbackCall(pluginTextureCallbackId callbackId, int glId, int w, int h){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param);
	if(callback == NULL){return false;}
	((void(*)(void*, int, int, int))callback)(param, glId, w, h);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

