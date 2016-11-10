#include "../core/library.h"
#include "native.h"
#include "../core/plugin/pluginTexture.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャバインド状態の記録と重複確認
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginTexture_corePluginTextureIsBind(JNIEnv *env, jobject obj, jint glId){
	return corePluginTextureIsBind(glId);
}

// テクスチャのコールバック
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginTexture_gamePluginTextureCallback(JNIEnv *env, jobject obj, jint callbackId, jint glId, jint w, jint h){
	gamePluginTextureCallbackCall(callbackId, glId, w, h);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ローカルデータ読み込み
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, int glId, int w, int h)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginTexture");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginTextureLocal", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginTextureCallbackSet(param, callback), str);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

