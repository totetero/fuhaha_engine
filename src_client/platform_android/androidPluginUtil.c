#include "native.h"
#include "../core/gamePluginUtil.h"
#include <time.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// url取得
JNIEXPORT jstring JNICALL Java_com_totetero_fuhaha_AndroidPluginUtil_gamePluginUtilUrlGet(JNIEnv *env, jobject obj){return (*env)->NewStringUTF(env, gamePluginUtilUrlGet());}

// バイナリのコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginUtil_gamePluginUtilCallbackBinary(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){
	jsize size = (*env)->GetArrayLength(env, buff0);
	jbyte *buff1 = (*env)->GetByteArrayElements(env, buff0, NULL);
	void *buff2 = malloc(size); // バッファの開放はゲーム側で行う
	memcpy(buff2, buff1, size);
	(*env)->ReleaseByteArrayElements(env, buff0, buff1, 0);
	gamePluginUtilCallbackCall(callbackId, buff2, size);
}

// 文字列のコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginUtil_gamePluginUtilCallbackString(JNIEnv *env, jobject obj, jint callbackId, jstring buff0){
	jbyte *buff1 = (*env)->GetStringUTFChars(env, buff0, NULL);
	size_t size = strlen(buff1) + 1;
	char *buff2 = (char*)malloc(size); // バッファの開放はゲーム側で行う
	strcpy(buff2, buff1);
	(*env)->ReleaseStringUTFChars(env, buff0, buff1);
	gamePluginUtilCallbackCall(callbackId, buff2, size);
}

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

