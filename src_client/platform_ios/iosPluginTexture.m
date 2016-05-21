#import "fuhaha-Swift.h"
#include "library.h"
#include "platform.h"
#include "pluginUtil.h"
#include "pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ローカルデータ読み込み
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, int glId, int texw, int texh, int imgw, int imgh)){
	int callbackId = gamePluginTextureCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginTexture platformPluginTextureLocal: callbackId src: nsstr1];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

