package com.totetero.fuhaha;

import android.app.Activity;
import android.view.MotionEvent;
import android.view.KeyEvent;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// openGL描画クラス
public class FuhahaCtrl implements SensorEventListener{
	// タッチ1情報
	private int t1id = -1;
	private int t1x = 0;
	private int t1y = 0;
	// タッチ2情報
	private int t2id = -1;
	private int t2x = 0;
	private int t2y = 0;
	// キー情報
	private boolean kbk = false;
	private boolean kup = false;
	private boolean kdn = false;
	private boolean krt = false;
	private boolean klt = false;
	private boolean kzb = false;
	private boolean kxb = false;
	private boolean kcb = false;
	private boolean kvb = false;
	// 加速度センサー
	private SensorManager sensorManager = null;
	private Sensor sensor = null;

	// ----------------------------------------------------------------

	// JNI連携
	static{System.loadLibrary("fuhaha_native");}
	public static native void gameEvenTouch(int index, int tx, int ty, boolean tdn);
	public static native void gameEventKeyBack(boolean kbk);
	public static native void gameEventKeyArrow(boolean kup, boolean kdn, boolean krt, boolean klt);
	public static native void gameEventKeyZxcv(boolean kzb, boolean kxb, boolean kcb, boolean kvb);
	public static native void gameEventAcceleration(double accx, double accy, double accz);

	// ----------------------------------------------------------------

	// コンストラクタ
	public FuhahaCtrl(){
		if(true){
			// 加速度センサー
			this.sensorManager = (SensorManager)FuhahaGLView.activity.getSystemService(FuhahaGLView.activity.SENSOR_SERVICE);
		}
	}

	public void onResume(){
		if(this.sensorManager != null){
			// 加速度センサー
			this.sensor = this.sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			if(this.sensor != null){this.sensorManager.registerListener(this, this.sensor, SensorManager.SENSOR_DELAY_NORMAL);}
		}
	}

	public void onPause(){
		if(this.sensorManager != null){
			// 加速度センサー
			this.sensorManager.unregisterListener(this);
			this.sensor = null;
		}
	}

	// ----------------------------------------------------------------

	// タッチイベント
	public boolean onTouchEvent(MotionEvent event){
		int eventAction = event.getActionMasked();
		int pointerIndex = event.getActionIndex();
		int pointerId = event.getPointerId(pointerIndex);
		boolean isChangeTouch1 = false;
		boolean isChangeTouch2 = false;

		// タッチ座標保存
		if(this.t1id >= 0){
			int ptrIndex = event.findPointerIndex(this.t1id);
			this.t1x = (int)event.getX(ptrIndex);
			this.t1y = (int)event.getY(ptrIndex);
			isChangeTouch1 = true;
		}
		if(this.t2id >= 0){
			int ptrIndex = event.findPointerIndex(this.t2id);
			this.t2x = (int)event.getX(ptrIndex);
			this.t2y = (int)event.getY(ptrIndex);
			isChangeTouch2 = true;
		}

		switch(eventAction){
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				// タッチ開始
				if(this.t1id < 0 && this.t1id != pointerId && this.t2id != pointerId){
					this.t1id = pointerId;
					this.t1x = (int)event.getX(pointerIndex);
					this.t1y = (int)event.getY(pointerIndex);
					isChangeTouch1 = true;
				}
				if(this.t2id < 0 && this.t1id != pointerId && this.t2id != pointerId){
					this.t2id = pointerId;
					this.t2x = (int)event.getX(pointerIndex);
					this.t2y = (int)event.getY(pointerIndex);
					isChangeTouch2 = true;
				}
				break;
			case MotionEvent.ACTION_MOVE:
				// タッチ中
				break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL:
				// タッチ完了
				if(this.t1id == pointerId){this.t1id = -1; isChangeTouch1 = true;}
				if(this.t2id == pointerId){this.t2id = -1; isChangeTouch2 = true;}
				break;
		}

		// タッチ情報をjniに送信
		if(isChangeTouch1){FuhahaCtrl.gameEvenTouch(0, this.t1x, this.t1y, (this.t1id >= 0));}
		if(isChangeTouch2){FuhahaCtrl.gameEvenTouch(1, this.t2x, this.t2y, (this.t2id >= 0));}

		return true;
	}

	// ----------------------------------------------------------------

	// キーイベント
	public boolean dispatchKeyEvent(KeyEvent event){
		boolean isChangeKeyBack = false;
		boolean isChangeKeyArrow = false;
		boolean isChangeKeyZxcv = false;
		if(event.getAction() == KeyEvent.ACTION_DOWN){
			switch(event.getKeyCode()){
				case KeyEvent.KEYCODE_BACK:       this.kbk = true; isChangeKeyBack = true; break;
				case KeyEvent.KEYCODE_DPAD_UP:    this.kup = true; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_DPAD_DOWN:  this.kdn = true; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_DPAD_RIGHT: this.krt = true; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_DPAD_LEFT:  this.klt = true; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_Z:          this.kzb = true; isChangeKeyZxcv = true; break;
				case KeyEvent.KEYCODE_X:          this.kxb = true; isChangeKeyZxcv = true; break;
				case KeyEvent.KEYCODE_C:          this.kcb = true; isChangeKeyZxcv = true; break;
				case KeyEvent.KEYCODE_V:          this.kvb = true; isChangeKeyZxcv = true; break;
				default: break;
			}
		}else if(event.getAction() == KeyEvent.ACTION_UP){
			switch(event.getKeyCode()){
				case KeyEvent.KEYCODE_BACK:       this.kbk = false; isChangeKeyBack = true; break;
				case KeyEvent.KEYCODE_DPAD_UP:    this.kup = false; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_DPAD_DOWN:  this.kdn = false; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_DPAD_RIGHT: this.krt = false; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_DPAD_LEFT:  this.klt = false; isChangeKeyArrow = true; break;
				case KeyEvent.KEYCODE_Z:          this.kzb = false; isChangeKeyZxcv = true; break;
				case KeyEvent.KEYCODE_X:          this.kxb = false; isChangeKeyZxcv = true; break;
				case KeyEvent.KEYCODE_C:          this.kcb = false; isChangeKeyZxcv = true; break;
				case KeyEvent.KEYCODE_V:          this.kvb = false; isChangeKeyZxcv = true; break;
				default: break;
			}
		}

		if(isChangeKeyBack || isChangeKeyArrow || isChangeKeyZxcv){
			if(isChangeKeyBack){FuhahaCtrl.gameEventKeyBack(this.kbk);}
			if(isChangeKeyArrow){FuhahaCtrl.gameEventKeyArrow(this.kup, this.kdn, this.krt, this.klt);}
			if(isChangeKeyZxcv){FuhahaCtrl.gameEventKeyZxcv(this.kzb, this.kxb, this.kcb, this.kvb);}
			return true;
		}
		return false;
	}

	// ----------------------------------------------------------------

	// センサー精度の更新
	public void onAccuracyChanged(Sensor sensor, int accuracy){}

	// センサーの取得値の更新
	public void onSensorChanged(SensorEvent event){
		if(event.sensor.getType() == Sensor.TYPE_ACCELEROMETER){
			// androidの軸方向を基準とする、最大値は知らん
			FuhahaCtrl.gameEventAcceleration(event.values[0], event.values[1], event.values[2]);
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

