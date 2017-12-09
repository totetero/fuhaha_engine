#include "../core/library.h"
#include "native.h"
#include "../core/plugin/pluginUtil.h"
#include "../core/plugin/pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バイナリのコールバック
void callbackCallBinary(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0, bool(*callbackCall)(int callbackId, void *buff, size_t size)){
	if(buff0 == NULL){callbackCall(callbackId, NULL, 0); return;}
	jsize size = (*env)->GetArrayLength(env, buff0);
	jbyte *buff1 = (*env)->GetByteArrayElements(env, buff0, NULL);
	void *buff2 = gamePluginUtilMemoryMalloc("pluginData", size); // バッファの解放はゲーム側で行う
	memcpy(buff2, buff1, size);
	(*env)->ReleaseByteArrayElements(env, buff0, buff1, 0);
	callbackCall(callbackId, buff2, size);
}

// 文字列のコールバック
void callbackCallString(JNIEnv *env, jobject obj, jint callbackId, jstring buff0, bool(*callbackCall)(int callbackId, void *buff, size_t size)){
	if(buff0 == NULL){callbackCall(callbackId, NULL, 0); return;}
	jbyte *buff1 = (*env)->GetStringUTFChars(env, buff0, NULL);
	size_t size = strlen(buff1) + 1;
	char *buff2 = (char*)gamePluginUtilMemoryMalloc("pluginData", size); // バッファの解放はゲーム側で行う
	strcpy(buff2, buff1);
	(*env)->ReleaseStringUTFChars(env, buff0, buff1);
	callbackCall(callbackId, buff2, size);
}

JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataHttpCallbackCallBinary(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){callbackCallBinary(env, obj, callbackId, buff0, gamePluginDataHttpCallbackCall);}
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataHttpCallbackCallString(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){callbackCallBinary(env, obj, callbackId, buff0, gamePluginDataHttpCallbackCall);}
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataLocalCallbackCallBinary(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){callbackCallBinary(env, obj, callbackId, buff0, gamePluginDataLocalCallbackCall);}
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginData_gamePluginDataLocalCallbackCallString(JNIEnv *env, jobject obj, jint callbackId, jbyteArray buff0){callbackCallBinary(env, obj, callbackId, buff0, gamePluginDataLocalCallbackCall);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, PLUGINDATA_HTTP_CALLBACKPARAMS)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataHttp", "(ILjava/lang/String;Ljava/lang/String;)V");
	jstring str1 = (*env)->NewStringUTF(env, url);
	jstring str2 = (*env)->NewStringUTF(env, request);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginDataHttpCallbackSet(param, callback), str1, str2);
}

// ----------------------------------------------------------------

// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, PLUGINDATA_LOCAL_CALLBACKPARAMS)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginData");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginDataLocal", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginDataLocalCallbackSet(param, callback), str);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

