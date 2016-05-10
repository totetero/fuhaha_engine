#include "native.h"
#include "../core/gamePluginUtil.h"
#include "../core/gamePluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, void *buff, size_t size)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataHttp", "(ILjava/lang/String;Ljava/lang/String;)V");
	jstring str1 = (*env)->NewStringUTF(env, url);
	jstring str2 = (*env)->NewStringUTF(env, request);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginUtilCallbackSet(param, callback), str1, str2);
}

// ----------------------------------------------------------------

// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, void *buff, size_t size)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataLocal", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginUtilCallbackSet(param, callback), str);
}

// ----------------------------------------------------------------

// 設定読み込み 返値文字列は揮発性バッファで解放禁止
char *platformPluginDataPreferenceGet(char *key){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataPreferenceGet", "(Ljava/lang/String;)Ljava/lang/String;");
	jstring str1 = (*env)->NewStringUTF(env, key);
	jstring str2 = (*env)->CallStaticObjectMethod(env, cls, mid, str1);

	char* value = (char*)((*env)->GetStringUTFChars(env, str2, NULL));
	if(value == NULL){return NULL;}
	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);
	(*env)->ReleaseStringUTFChars(env, str2, value);

	return buff;
}

// 設定書き込み
void platformPluginDataPreferenceSet(char *key, char *value){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataPreferenceSet", "(Ljava/lang/String;Ljava/lang/String;)V");
	jstring str1 = (*env)->NewStringUTF(env, key);
	jstring str2 = (*env)->NewStringUTF(env, value);
	(*env)->CallStaticVoidMethod(env, cls, mid, str1, str2);
}

// 設定保存
void platformPluginDataPreferenceCommit(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataPreferenceCommit", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

