#include "../core/library.h"
#include "./native.h"
#include "../core/plugin/pluginUtil.h"
#include "../core/plugin/pluginPreference.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 設定読み込み 返値文字列は揮発性バッファで解放禁止
char *platformPluginPreferenceGet(char *key){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginPreference");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginPreferenceGet", "(Ljava/lang/String;)Ljava/lang/String;");
	jstring str1 = (*env)->NewStringUTF(env, key);
	jstring str2 = (*env)->CallStaticObjectMethod(env, cls, mid, str1);
	if(str2 == NULL){return NULL;}

	char *value = (char*)((*env)->GetStringUTFChars(env, str2, NULL));
	char *buff = (char*)corePluginUtilMemoryTemporary(strlen(value) + 1);
	strcpy(buff, value);
	(*env)->ReleaseStringUTFChars(env, str2, value);

	return buff;
}

// 設定書き込み
void platformPluginPreferenceSet(char *key, char *value){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginPreference");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginPreferenceSet", "(Ljava/lang/String;Ljava/lang/String;)V");
	jstring str1 = (*env)->NewStringUTF(env, key);
	jstring str2 = (*env)->NewStringUTF(env, value);
	(*env)->CallStaticVoidMethod(env, cls, mid, str1, str2);
}

// 設定保存
void platformPluginPreferenceCommit(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/AndroidPluginPreference");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformPluginPreferenceCommit", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

