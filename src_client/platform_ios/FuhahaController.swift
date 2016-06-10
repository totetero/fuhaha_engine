import UIKit
import CoreMotion

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaController{
	// タッチ1情報
	internal var t1id: Int = -1;
	internal var t1x: Int32 = 0;
	internal var t1y: Int32 = 0;
	// タッチ2情報
	internal var t2id: Int = -1;
	internal var t2x: Int32 = 0;
	internal var t2y: Int32 = 0;
	// 加速度センサー
	internal let motionManager: CMMotionManager;

	// ----------------------------------------------------------------

	// コンストラクタ
	init(){
		// 加速度センサー
		self.motionManager = CMMotionManager();
		self.motionManager.accelerometerUpdateInterval = 0.05;
		self.motionManager.startAccelerometerUpdatesToQueue(NSOperationQueue.currentQueue()!, withHandler: {(data: CMAccelerometerData?, error: NSError?) -> Void in
			if(data == nil){return;}
			// androidの軸方向に合わせる、最大値は知らん
			gameEventAcceleration(-data!.acceleration.x, -data!.acceleration.y, -data!.acceleration.z);
		});
	}

	// ----------------------------------------------------------------

	func onTouch(){
		gameEvenTouch(0, t1x, t1y, (t1id >= 0));
		gameEvenTouch(1, t2x, t2y, (t2id >= 0));
	}

	// タッチ開始
	func touchesBegan(view: UIView, touches: Set<UITouch>, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.locationInView(view);
			if(t1id < 0){
				t1x = Int32(point.x);
				t1y = Int32(point.y);
				t1id = tid;
			}else if(t2id < 0){
				t2x = Int32(point.x);
				t2y = Int32(point.y);
				t2id = tid;
			}
		}
		self.onTouch();
	}

	// タッチ移動
	func touchesMoved(view: UIView, touches: Set<UITouch>, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.locationInView(view);
			if(t1id == tid){
				t1x = Int32(point.x);
				t1y = Int32(point.y);
			}else if(t2id == tid){
				t2x = Int32(point.x);
				t2y = Int32(point.y);
			}
		}
		self.onTouch();
	}

	// タッチ完了
	func touchesEnded(view: UIView, touches: Set<UITouch>, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.locationInView(view);
			if(t1id == tid){
				t1x = Int32(point.x);
				t1y = Int32(point.y);
				t1id = -1;
			}else if(t2id == tid){
				t2x = Int32(point.x);
				t2y = Int32(point.y);
				t2id = -1;
			}
		}
		self.onTouch();
	}

	// タッチ中止
	func touchesCancelled(view: UIView, touches: Set<UITouch>!, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.locationInView(view);
			if(t1id == tid){
				t1x = Int32(point.x);
				t1y = Int32(point.y);
				t1id = -1;
			}else if(t2id == tid){
				t2x = Int32(point.x);
				t2y = Int32(point.y);
				t2id = -1;
			}
		}
		self.onTouch();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

