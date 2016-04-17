#include "native.h"
#include "../core/gamePluginUtil.h"
#include <time.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// url取得
JNIEXPORT jstring JNICALL Java_com_totetero_fuhaha_AndroidPluginUtil_gamePluginUtilUrlGet(JNIEnv *env, jobject obj){return (*env)->NewStringUTF(env, gamePluginUtilUrlGet());}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラットフォーム名取得 返値文字列は解放禁止
char *platformPluginUtilPlatformGet(void){
	static const char* value = "and";
	return (char*)value;
}

// ユーザーID取得 返値文字列は解放禁止
char *platformPluginUtilUidGet(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginUtilUidGet", "()Ljava/lang/String;");
	jstring str = (*env)->CallStaticObjectMethod(env, cls, mid);
	char* value = (char*)((*env)->GetStringUTFChars(env, str, NULL));
	if(value == NULL){return NULL;}

	static char buff[8];
	strncpy(buff, value, sizeof(buff));
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
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginUtilIsLoading", "()I");
	return (*env)->CallStaticIntMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 読み込み中カウンタ加算
void nativePluginUtilIncrementLoading(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "nativePluginUtilIncrementLoading", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// 読み込み中カウンタ減算
void nativePluginUtilDecrementLoading(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginUtil");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "nativePluginUtilDecrementLoading", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

