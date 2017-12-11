#include "../library.h"
#include "../plugin/pluginUtil.h"
#include "../plugin/pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録
pluginTextureLocalCallbackId gamePluginTextureLocalCallbackSet(void *param, void(*callback)(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS)){
	return (pluginTextureLocalCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// コールバック関数の登録
pluginTextureFontCallbackId gamePluginTextureFontCallbackSet(void *param, void(*callback)(void *param, PLUGINTEXTURE_FONT_CALLBACKPARAMS)){
	return (pluginTextureFontCallbackId)corePluginUtilCallbackSet(param, (void*)callback);
}

// ----------------------------------------------------------------

// コールバック関数の実行と解放
bool gamePluginTextureLocalCallbackCall(pluginTextureLocalCallbackId callbackId, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param, true);
	if(callback == NULL){return false;}
	((void(*)(void*, int, int, int))callback)(param, glId, imgw, imgh);
	return true;
}

// コールバック関数の実行と解放
bool gamePluginTextureFontCallbackCall(pluginTextureFontCallbackId callbackId, PLUGINTEXTURE_FONT_CALLBACKPARAMS){
	void *param;
	void *callback = corePluginUtilCallbackGet(callbackId, &param, true);
	if(callback == NULL){return false;}
	((void(*)(void*, int, int))callback)(param, codeListIndex, codeListLength);
	return true;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

