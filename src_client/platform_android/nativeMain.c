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
JNIEnv *getJNIEnv(){
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
	gameSurfacePause();
}

// ライフサイクル ゲーム終了時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnDestroy(JNIEnv *env, jobject obj){
	gameSurfaceDestroy();
}

// openGL設定
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlSetup(JNIEnv *env, jobject obj){
	gameSurfaceCreated();
}

// openGL画面サイズ変更時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlResize(JNIEnv *env, jobject obj, jint width, jint height){
	gameSurfaceChanged(width, height, 1.0);
}

// openGL描画
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlStep(JNIEnv *env, jobject obj){
	gameSurfaceDrawFrame();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
