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
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_AndroidPluginTexture_gamePluginTextureCallback(JNIEnv *env, jobject obj, jint callbackId, jint glId, jint texw, jint texh, jint imgw, jint imgh){
	gamePluginTextureCallbackCall(callbackId, glId, texw, texh, imgw, imgh);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ローカルデータ読み込み
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, int glId, int texw, int texh, int imgw, int imgh)){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginTexture");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginTextureLocal", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, gamePluginTextureCallbackSet(param, callback), str);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

