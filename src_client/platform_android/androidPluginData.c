#include "../core/library.h"
#include "native.h"
#include "../core/plugin/pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バイナリのコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataCallbackBinary(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){
	if(buff0 == NULL){gamePluginDataCallbackCall(callbackId, NULL, 0); return;}
	jsize size = (*env)->GetArrayLength(env, buff0);
	jbyte *buff1 = (*env)->GetByteArrayElements(env, buff0, NULL);
	void *buff2 = malloc(size); // バッファの解放はゲーム側で行う
	memcpy(buff2, buff1, size);
	(*env)->ReleaseByteArrayElements(env, buff0, buff1, 0);
	gamePluginDataCallbackCall(callbackId, buff2, size);
}

// 文字列のコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataCallbackString(JNIEnv *env, jobject obj, jint callbackId, jstring buff0){
	if(buff0 == NULL){gamePluginDataCallbackCall(callbackId, NULL, 0); return;}
	jbyte *buff1 = (*env)->GetStringUTFChars(env, buff0, NULL);
	size_t size = strlen(buff1) + 1;
	char *buff2 = (char*)malloc(size); // バッファの解放はゲーム側で行う
	strcpy(buff2, buff1);
	(*env)->ReleaseStringUTFChars(env, buff0, buff1);
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

