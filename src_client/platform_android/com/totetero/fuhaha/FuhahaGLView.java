package com.totetero.fuhaha;

import android.app.Activity;
import android.view.View;
import android.view.KeyEvent;
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

	public FuhahaEvent fuhahaEvent;

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

		// イベントコントローラ
		this.fuhahaEvent = new FuhahaEvent();
		// キーイベント
		this.setFocusableInTouchMode(true);
		this.setOnKeyListener(new View.OnKeyListener(){
			@Override
			public boolean onKey(View view, int keyCode, KeyEvent event){
				return fuhahaEvent.dispatchKeyEvent(event);
			}
		});

		FuhahaGLView.nativeOnCreate();
	}

	@Override
	public void onResume(){
		super.onResume();
		FuhahaGLView.nativeOnResume();
		this.fuhahaEvent.onResume();
		AndroidPluginSound.onResume();
	}

	@Override
	public void onPause(){
		AndroidPluginSound.onPause();
		this.fuhahaEvent.onPause();
		FuhahaGLView.nativeOnPause();
		super.onPause();
	}

	public void onDestroy(){
		FuhahaGLView.nativeOnDestroy();
	}

	// ----------------------------------------------------------------

	// GL初期化
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config){
		FuhahaGLView.nativeGlSetup();
	}

	// GL画面サイズ変更
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height){
		FuhahaGLView.nativeGlResize(width, height);
	}

	// GL描画
	@Override
	public void onDrawFrame(GL10 gl){
		FuhahaGLView.nativeGlStep();
	}

	// ----------------------------------------------------------------

	// タッチイベント
	@Override
	public boolean onTouchEvent(MotionEvent event){
		return this.fuhahaEvent.onTouchEvent(event) || super.onTouchEvent(event);
	}

	// ----------------------------------------------------------------

	// アプリ終了命令
	public static void platformMainSurfaceExit(){
		FuhahaGLView.activity.finish();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

