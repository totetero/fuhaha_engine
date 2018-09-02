#import "./fuhaha-Swift.h"
#include "fuhahaEngine.h"
#include "./native.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 設定読み込み 返値文字列は揮発性バッファで解放禁止
char *platformPluginPreferenceGet(char *key){
	NSString *nsstr1 = [NSString stringWithCString: key encoding: NSUTF8StringEncoding];
	NSString *nsstr2 = [IosPluginPreference platformPluginPreferenceGet: nsstr1];
	if(nsstr2 == NULL){return NULL;}

	char *value = (char*)[nsstr2 UTF8String];
	char *buff = (char*)corePluginUtilMemoryTemporary(strlen(value) + 1);
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

