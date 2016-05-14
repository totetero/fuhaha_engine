#include "../core/library.h"
#include "platform.h"
#include "native.h"
#include "../core/plugin/pluginUtil.h"
#include <time.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// url取得
JNIEXPORT jstring JNICALL Java_com_totetero_fuhaha_AndroidPluginUtil_gamePluginUtilUrlGet(JNIEnv *env, jobject obj){return (*env)->NewStringUTF(env, gamePluginUtilUrlGet());}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラットフォーム名取得 返値文字列は揮発性バッファで解放禁止
char *platformPluginUtilPlatformGet(void){
	char *value = "and";
	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);
	return buff;
}

// ユーザーID取得 返値文字列は揮発性バッファで解放禁止
char *platformPluginUtilUidGet(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginUtilUidGet", "()Ljava/lang/String;");
	jstring str = (*env)->CallStaticObjectMethod(env, cls, mid);
	char* value = (char*)((*env)->GetStringUTFChars(env, str, NULL));
	if(value == NULL){return NULL;}

	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);
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
bool platformPluginUtilIsLoading(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginUtilIsLoading", "()Z");
	return (*env)->CallStaticBooleanMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 読み込み中カウンタ加算
void nativePluginUtilLoadingIncrement(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "nativePluginUtilLoadingIncrement", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// 読み込み中カウンタ減算
void nativePluginUtilLoadingDecrement(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "nativePluginUtilLoadingDecrement", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

