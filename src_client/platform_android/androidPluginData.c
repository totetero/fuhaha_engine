#include "../core/library.h"
#include "native.h"
#include "../core/plugin/pluginUtil.h"
#include "../core/plugin/pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バイナリのコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataCallbackBinary(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){
	if(buff0 == NULL){gamePluginDataCallbackCall(callbackId, NULL, 0); return;}
	jsize size = (*env)->GetArrayLength(env, buff0);
	void *buff2 = gamePluginUtilMemoryMalloc("pluginData", size); // バッファの解放はゲーム側で行う
	jbyte *buff1 = (*env)->GetPrimitiveArrayCritical(env, buff0, NULL);
	memcpy(buff2, buff1, size);
	(*env)->ReleasePrimitiveArrayCritical(env, buff0, buff1, 0);
	gamePluginDataCallbackCall(callbackId, buff2, size);
}

// 文字列のコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataCallbackString(JNIEnv *env, jobject obj, jint callbackId, jstring buff0){
	if(buff0 == NULL){gamePluginDataCallbackCall(callbackId, NULL, 0); return;}
	jbyte *buff1 = (*env)->GetStringCritical(env, buff0, NULL);
	size_t size = strlen(buff1) + 1;
	char *buff2 = (char*)gamePluginUtilMemoryMalloc("pluginData", size); // バッファの解放はゲーム側で行う
	strcpy(buff2, buff1);
	(*env)->ReleaseStringCritical(env, buff0, buff1);
	gamePluginDataCallbackCall(callbackId, buff2, size);
}

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
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginDataCallbackSet(param, callback), str1, str2);
}

// ----------------------------------------------------------------

// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, void *buff, size_t size)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataLocal", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginDataCallbackSet(param, callback), str);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

