package com.totetero.fuhaha;

import android.app.Activity;
import android.view.MotionEvent;

import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// openGL描画クラス
public class FuhahaGLView extends GLSurfaceView implements Renderer{
	public static Activity activity;
	public static FuhahaGLView surfaceView;

	// ----------------------------------------------------------------

	// JNI連携
	static{System.loadLibrary("fuhaha_native");}
	public static native void nativeOnCreate();
	public static native void nativeOnResume();
	public static native void nativeOnPause();
	public static native void nativeOnDestroy();
	public static native void nativeGlSetup();
	public static native void nativeGlResize(int width, int height);
	public static native void nativeGlStep();

	// ----------------------------------------------------------------

	// コンストラクタ
	public FuhahaGLView(Activity activity){
		super(activity);
		FuhahaGLView.activity = activity;
		FuhahaGLView.surfaceView = this;

		// openGL
		this.setEGLContextClientVersion(2);
		this.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		this.setRenderer(this);

		FuhahaGLView.nativeOnCreate();
	}

	public void onResume(){
		FuhahaGLView.nativeOnResume();
		AndroidPluginSound.onResume();
		super.onResume();
	}

	public void onPause(){
		FuhahaGLView.nativeOnPause();
		AndroidPluginSound.onPause();
		super.onPause();
	}

	public void onDestroy(){
		FuhahaGLView.nativeOnDestroy();
	}

	// ----------------------------------------------------------------

	// GL初期化
	public void onSurfaceCreated(GL10 gl, EGLConfig config){
		FuhahaGLView.nativeGlSetup();
	}

	// GL画面サイズ変更
	public void onSurfaceChanged(GL10 gl, int width, int height){
		FuhahaGLView.nativeGlResize(width, height);
	}

	// GL描画
	public void onDrawFrame(GL10 gl){
		FuhahaGLView.nativeGlStep();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

