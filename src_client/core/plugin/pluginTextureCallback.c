#include "../library.h"
#include "platform.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginTextureCallbackId gamePluginTextureCallbackSet(void *param, void(*callback)(void *param, uint32_t glId, uint16_t texw, uint16_t texh, uint16_t imgw, uint16_t imgh)){
	return (pluginTextureCallbackId)corePluginUtilCallbackSet(param, callback);
}

// コールバック関数の実行と解放
bool gamePluginTextureCallbackCall(pluginTextureCallbackId callbackId, uint32_t glId, uint16_t texw, uint16_t texh, uint16_t imgw, uint16_t imgh){
	void *param;
	void(*callback)(void *param, uint32_t glId, uint16_t texw, uint16_t texh, uint16_t imgw, uint16_t imgh) = corePluginUtilCallbackGet(callbackId, &param);
	if(callback == NULL){return false;}
	callback(param, glId, texw, texh, imgw, imgh);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

