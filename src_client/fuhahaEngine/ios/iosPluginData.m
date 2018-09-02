#import "./fuhaha-Swift.h"
#include "fuhahaEngine.h"
#include "./native.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, PLUGINDATA_HTTP_CALLBACKPARAMS)){
	pluginDataHttpCallbackId callbackId = gamePluginDataHttpCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: url encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [NSString stringWithCString: request encoding: NSUTF8StringEncoding];
	[IosPluginData platformPluginDataHttp: callbackId url: nsstr1 request: nsstr2];
}

// ----------------------------------------------------------------

// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, PLUGINDATA_LOCAL_CALLBACKPARAMS)){
	pluginDataLocalCallbackId callbackId = gamePluginDataLocalCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginData platformPluginDataLocal: callbackId src: nsstr1];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

