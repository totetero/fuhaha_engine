#import "fuhaha-Swift.h"

#include "native.h"
#include "gamePluginUtil.h"
#include "gamePluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, void *buff, size_t size)){
	int callbackId = gamePluginUtilCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: url encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [NSString stringWithCString: request encoding: NSUTF8StringEncoding];
	[IosPluginData platformPluginDataHttp: callbackId url: nsstr1 request: nsstr2];
}

// ----------------------------------------------------------------

// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, void *buff, size_t size)){
	int callbackId = gamePluginUtilCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginData platformPluginDataLocal: callbackId src: nsstr1];
}

// ----------------------------------------------------------------

// 設定読み込み 返値文字列は揮発性バッファで解放禁止
char *platformPluginDataPreferenceGet(char *key){
	NSString *nsstr1 = [NSString stringWithCString: key encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [IosPluginData platformPluginDataPreferenceGet: nsstr1];

	char* value = (char*)[nsstr2 UTF8String];
	if(value == NULL){return NULL;}
	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);

	return buff;
}

// 設定書き込み
void platformPluginDataPreferenceSet(char *key, char *value){
	NSString *nsstr1 = [NSString stringWithCString: key encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [NSString stringWithCString: value encoding: NSUTF8StringEncoding];
	[IosPluginData platformPluginDataPreferenceSet: nsstr1 value: nsstr2];
}

// 設定保存
void platformPluginDataPreferenceCommit(void){
	[IosPluginData platformPluginDataPreferenceCommit];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

