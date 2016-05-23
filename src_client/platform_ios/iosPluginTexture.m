#import "fuhaha-Swift.h"
#include "library.h"
#include "platform.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ローカルデータ読み込み
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, uint32_t glId, uint16_t texw, uint16_t texh, uint16_t imgw, uint16_t imgh)){
	pluginTextureCallbackId callbackId = gamePluginTextureCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginTexture platformPluginTextureLocal: callbackId src: nsstr1];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

