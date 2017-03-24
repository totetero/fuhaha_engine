#include "../library.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginTextureLocalCallbackId gamePluginTextureLocalCallbackSet(void *param, void(*callback)(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS)){
	return (pluginTextureLocalCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// ----------------------------------------------------------------

// コールバック関数の実行と解放
bool gamePluginTextureLocalCallbackCall(pluginTextureLocalCallbackId callbackId, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param, true);
	if(callback == NULL){return false;}
	((void(*)(void*, int, int, int))callback)(param, glId, w, h);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

