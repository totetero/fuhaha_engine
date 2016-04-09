#include "native.h"
#include "../core/game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// バーチャルマシン
	JavaVM *javaVM;
	// ピクセルレシオの逆数
	double iPixelRatio;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

jint JNI_OnLoad(JavaVM *vm, void* reserved){
	localGlobal.javaVM = vm;
	return JNI_VERSION_1_6;
}

static JNIEnv *getJNIEnv(){
	JNIEnv *env;
	jint ret = (*localGlobal.javaVM)->GetEnv(localGlobal.javaVM, (void**)&env, JNI_VERSION_1_6);
	return (ret == JNI_OK) ? env : NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// javaから呼び出すc言語

// ゲーム初期化時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnCreate(JNIEnv *env, jobject obj){
}

// ゲーム開始時と一時停止解除時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnResume(JNIEnv *env, jobject obj){
}

// ゲーム一時停止時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnPause(JNIEnv *env, jobject obj){
	gameSurfacePause();
}

// ゲーム終了時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeOnDestroy(JNIEnv *env, jobject obj){
	gameSurfaceDestroy();
}

// openGL設定
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlSetup(JNIEnv *env, jobject obj){
	gameSurfaceCreated();
}

// openGL画面サイズ変更時
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlResize(JNIEnv *env, jobject obj, jint width, jint height){
	localGlobal.iPixelRatio = 320.0 / (width < height ? width : height);
	width *= localGlobal.iPixelRatio;
	height *= localGlobal.iPixelRatio;
	gameSurfaceChanged(width, height, 1 / localGlobal.iPixelRatio);
}

// openGL描画
JNIEXPORT void JNICALL Java_com_totetero_fuhaha_FuhahaGLView_nativeGlStep(JNIEnv *env, jobject obj){
	gameSurfaceDrawFrame();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// c言語からよびだすjava

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

