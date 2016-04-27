#import "fuhaha-Swift.h"

#include "native.h"
#include "gamePluginUtil.h"
#include <time.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラットフォーム名取得 返値文字列は解放禁止
char *platformPluginUtilPlatformGet(void){
	static const char* value = "ios";
	return (char*)value;
}

// ユーザーID取得 返値文字列は解放禁止
char *platformPluginUtilUidGet(void){
	NSString *value = [IosPluginUtil platformPluginUtilUidGet];

	static char buff[8];
	strncpy(buff, (char*)[value UTF8String], sizeof(buff));
	return buff;
}

// ----------------------------------------------------------------

// unix時間取得
long long int platformPluginUtilTimeGet(void){
	time_t timer;
	time(&timer);
	return (long long int)timer;
}

// ----------------------------------------------------------------

// 読み込み中確認
int platformPluginUtilIsLoading(void){
	return [IosPluginUtil platformPluginUtilIsLoading];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 読み込み中カウンタ加算
void nativePluginUtilLoadingIncrement(void){
	[IosPluginUtil nativePluginUtilLoadingIncrement];
}

// 読み込み中カウンタ減算
void nativePluginUtilLoadingDecrement(void){
	[IosPluginUtil nativePluginUtilLoadingDecrement];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

