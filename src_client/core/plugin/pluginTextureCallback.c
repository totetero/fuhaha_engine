#include "../library.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginTextureCallbackId gamePluginTextureCallbackSet(void *param, void(*callback)(void *param, int glId, int texw, int texh, int imgw, int imgh)){
	return (pluginTextureCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// コールバック関数の実行と解放
bool gamePluginTextureCallbackCall(pluginTextureCallbackId callbackId, int glId, int texw, int texh, int imgw, int imgh){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param);
	if(callback == NULL){return false;}
	((void(*)(void*, int, int, int, int, int))callback)(param, glId, texw, texh, imgw, imgh);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

