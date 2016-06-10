package com.totetero.fuhaha;

import android.app.Activity;
import android.view.MotionEvent;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// openGL描画クラス
public class FuhahaController implements SensorEventListener{
	// タッチ1情報
	public int t1id = -1;
	public int t1x = 0;
	public int t1y = 0;
	// タッチ2情報
	public int t2id = -1;
	public int t2x = 0;
	public int t2y = 0;

	// 加速度センサー
	private SensorManager sensorManager;
	private Sensor sensor;

	// ----------------------------------------------------------------

	// JNI連携
	static{System.loadLibrary("fuhaha_native");}
	public static native void gameEvenTouch(int index, int tx, int ty, boolean tdn);
	public static native void gameEventAcceleration(double accx, double accy, double accz);

	// ----------------------------------------------------------------

	// コンストラクタ
	public FuhahaController(){
		// 加速度センサー
		this.sensorManager = (SensorManager)FuhahaGLView.activity.getSystemService(FuhahaGLView.activity.SENSOR_SERVICE);
	}

	public void onResume(){
		// 加速度センサー
		this.sensor = this.sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		if(this.sensor != null){this.sensorManager.registerListener(this, this.sensor, SensorManager.SENSOR_DELAY_NORMAL);}
	}

	public void onPause(){
		// 加速度センサー
		this.sensorManager.unregisterListener(this);
		this.sensor = null;
	}

	// ----------------------------------------------------------------

	// タッチイベント
	public boolean onTouchEvent(MotionEvent event){
		int eventAction = event.getActionMasked();
		int pointerIndex = event.getActionIndex();
		int pointerId = event.getPointerId(pointerIndex);

		// タッチ座標保存
		if(this.t1id >= 0){
			int ptrIndex = event.findPointerIndex(this.t1id);
			this.t1x = (int)event.getX(ptrIndex);
			this.t1y = (int)event.getY(ptrIndex);
		}
		if(this.t2id >= 0){
			int ptrIndex = event.findPointerIndex(this.t2id);
			this.t2x = (int)event.getX(ptrIndex);
			this.t2y = (int)event.getY(ptrIndex);
		}

		switch(eventAction){
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				// タッチ開始
				if(this.t1id < 0 && this.t1id != pointerId && this.t2id != pointerId){
					this.t1id = pointerId;
					this.t1x = (int)event.getX(pointerIndex);
					this.t1y = (int)event.getY(pointerIndex);
				}
				if(this.t2id < 0 && this.t1id != pointerId && this.t2id != pointerId){
					this.t2id = pointerId;
					this.t2x = (int)event.getX(pointerIndex);
					this.t2y = (int)event.getY(pointerIndex);
				}
				break;
			case MotionEvent.ACTION_MOVE:
				// タッチ中
				break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL:
				// タッチ完了
				if(this.t1id == pointerId){this.t1id = -1;}
				if(this.t2id == pointerId){this.t2id = -1;}
				break;
		}

		// タッチ情報をjniに送信
		FuhahaController.gameEvenTouch(0, this.t1x, this.t1y, (this.t1id >= 0));
		FuhahaController.gameEvenTouch(1, this.t2x, this.t2y, (this.t2id >= 0));

		return true;
	}

	// ----------------------------------------------------------------

	// センサー精度の更新
	public void onAccuracyChanged(Sensor sensor, int accuracy){}

	// センサーの取得値の更新
	public void onSensorChanged(SensorEvent event){
		if(event.sensor.getType() == Sensor.TYPE_ACCELEROMETER){
			// androidの軸方向を基準とする、最大値は知らん
			FuhahaController.gameEventAcceleration(event.values[0], event.values[1], event.values[2]);
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

