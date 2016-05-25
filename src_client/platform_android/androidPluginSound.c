#include "../core/library.h"
#include "platform.h"
#include "native.h"
#include "../core/plugin/pluginSound.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// BGM読込
void platformPluginSoundBgmLoad(pluginSoundBgmId bgmId, char *src){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundBgmLoad", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, bgmId, str);
}

// BGM再生
void platformPluginSoundBgmPlay(pluginSoundBgmId bgmId){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundBgmPlay", "(I)V");
	(*env)->CallStaticVoidMethod(env, cls, mid, bgmId);
}

// BGMトーンダウン
void platformPluginSoundBgmToneDown(double volume){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundBgmToneDown", "(D)V");
	(*env)->CallStaticVoidMethod(env, cls, mid, volume);
}

// BGM設定音量
void platformPluginSoundBgmVolume(double volume){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundBgmVolume", "(D)V");
	(*env)->CallStaticVoidMethod(env, cls, mid, volume);
}

// ----------------------------------------------------------------

// SE読込
void platformPluginSoundSeLoad(pluginSoundSeId seId, char *src){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundSeLoad", "(ILjava/lang/String;)V");
	jstring str = (*env)->NewStringUTF(env, src);
	(*env)->CallStaticVoidMethod(env, cls, mid, seId, str);
}

// SE再生
void platformPluginSoundSePlay(pluginSoundSeId seId){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundSePlay", "(I)V");
	(*env)->CallStaticVoidMethod(env, cls, mid, seId);
}

// SE設定音量
void platformPluginSoundSeVolume(double volume){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginSound");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginSoundSeVolume", "(D)V");
	(*env)->CallStaticVoidMethod(env, cls, mid, volume);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

