#import "./fuhaha-Swift.h"
#include "fuhahaEngine.h"
#include "./native.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラットフォーム名取得 返値文字列は揮発性バッファで解放禁止
char *platformPluginUtilPlatformGet(void){
	NSString *value = @"ios";
	char *buff = (char*)corePluginUtilMemoryTemporary([value length] + 1);
	strcpy(buff, (char*)[value UTF8String]);
	return buff;
}

// ユーザーID取得 返値文字列は揮発性バッファで解放禁止
char *platformPluginUtilUidGet(void){
	NSString *value = [IosPluginUtil platformPluginUtilUidGet];
	char *buff = (char*)corePluginUtilMemoryTemporary([value length] + 1);
	strcpy(buff, (char*)[value UTF8String]);
	return buff;
}

// ----------------------------------------------------------------

// 読み込み中確認
bool platformPluginUtilIsLoading(void){
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

