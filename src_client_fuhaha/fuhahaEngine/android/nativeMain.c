#include "../core/library.h"
#include "native.h"
#include "../core/game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// バーチャルマシン
	JavaVM *javaVM;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// JNIロード時に呼び出されるコールバック
jint JNI_OnLoad(JavaVM *vm, void* reserved){
	localGlobal.javaVM = vm;
	return JNI_VERSION_1_6;
}

// JNIEnvを取得する
JNIEnv *getJNIEnv(void){
	JNIEnv *env;
	jint ret = (*localGlobal.javaVM)->GetEnv(localGlobal.javaVM, (void**)&env, JNI_VERSION_1_6);
	return (ret == JNI_OK) ? env : NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ライフサイクル ゲーム初期化時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnCreate(JNIEnv *env, jobject obj){
}

// ライフサイクル ゲーム開始時と一時停止解除時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnResume(JNIEnv *env, jobject obj){
}

// ライフサイクル ゲーム一時停止時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnPause(JNIEnv *env, jobject obj){
	gameMainSurfacePause();
}

// ライフサイクル ゲーム終了時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnDestroy(JNIEnv *env, jobject obj){
	gameMainSurfaceDestroy();
}

// openGL設定
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlSetup(JNIEnv *env, jobject obj){
	gameMainSurfaceCreated();
	gameMainEventInit();
}

// openGL画面サイズ変更時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlResize(JNIEnv *env, jobject obj, jint width, jint height){
	gameMainEventScreen(width, height, 1.0);
}

// openGL描画
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlStep(JNIEnv *env, jobject obj){
	gameMainSurfaceDrawFrame();
}

// ----------------------------------------------------------------

// タッチの状態を報告する
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventTouch(JNIEnv *env, jobject obj, jint index, jint tx, jint ty, jboolean tdn){
	gameMainEventTouch(index, tx, ty, tdn);
}

// キーの押下状態変化を報告する
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventKeyBack(JNIEnv *env, jobject obj, jboolean kbk){
	gameMainEventKeyBack(kbk);
}
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventKeyArrow(JNIEnv *env, jobject obj, jboolean kup, jboolean kdn, jboolean krt, jboolean klt){
	gameMainEventKeyArrow(kup, kdn, krt, klt);
}
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventKeyZxcv(JNIEnv *env, jobject obj, jboolean kzb, jboolean kxb, jboolean kcb, jboolean kvb){
	gameMainEventKeyZxcv(kzb, kxb, kcb, kvb);
}

// 戻るボタンの制御を行うか
JNIEXPORT jboolean JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventKeyIsBack(JNIEnv *env, jobject obj){
	return gameMainEventKeyIsBack();
}

// 傾きを使うかどうか
JNIEXPORT jboolean JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventIsAcceleration(JNIEnv *env, jobject obj){
	return gameMainEventIsAcceleration();
}

// 傾きの状態を報告する
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaEvent_gameMainEventAcceleration(JNIEnv *env, jobject obj, jdouble accx, jdouble accy, jdouble accz){
	gameMainEventAcceleration(accx, accy, accz);
}

// ----------------------------------------------------------------

// アプリ終了命令
void platformMainSurfaceExit(void){
	JNIEnv *env = getJNIEnv();
	jclass cls = (*env)->FindClass(env, "com/totetero/fuhaha/FuhahaGLView");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "platformMainSurfaceExit", "()V");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

