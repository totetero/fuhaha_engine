#import "fuhaha-Swift.h"
#include "library.h"
#include "native.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ローカルデータ読み込み
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, int glId, int texw, int texh, int imgw, int imgh)){
	pluginTextureCallbackId callbackId = gamePluginTextureCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginTexture platformPluginTextureLocal: callbackId src: nsstr1];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

