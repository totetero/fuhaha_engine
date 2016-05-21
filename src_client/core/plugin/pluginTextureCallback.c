#include "../library.h"
#include "platform.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
int gamePluginTextureCallbackSet(void *param, void(*callback)(void *param, int glId, int texw, int texh, int imgw, int imgh)){
	return corePluginUtilCallbackSet(param, callback);
}

// コールバック関数の実行と解放
bool gamePluginTextureCallbackCall(int callbackId, int glId, int texw, int texh, int imgw, int imgh){
	void *param;
	void(*callback)(void *param, int glId, int texw, int texh, int imgw, int imgh) = corePluginUtilCallbackGet(callbackId, &param);
	if(callback == NULL){return false;}
	callback(param, glId, texw, texh, imgw, imgh);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

