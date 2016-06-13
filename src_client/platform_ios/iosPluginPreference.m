#import "fuhaha-Swift.h"
#include "library.h"
#include "platform.h"
#include "native.h"
#include "pluginUtil.h"
#include "pluginPreference.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 設定読み込み 返値文字列は揮発性バッファで解放禁止
char *platformPluginPreferenceGet(char *key){
	NSString *nsstr1 = [NSString stringWithCString: key encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [IosPluginPreference platformPluginPreferenceGet: nsstr1];

	char *value = (char*)[nsstr2 UTF8String];
	if(value == NULL){return NULL;}
	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);

	return buff;
}

// 設定書き込み
void platformPluginPreferenceSet(char *key, char *value){
	NSString *nsstr1 = [NSString stringWithCString: key encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [NSString stringWithCString: value encoding: NSUTF8StringEncoding];
	[IosPluginPreference platformPluginPreferenceSet: nsstr1 value: nsstr2];
}

// 設定保存
void platformPluginPreferenceCommit(void){
	[IosPluginPreference platformPluginPreferenceCommit];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

