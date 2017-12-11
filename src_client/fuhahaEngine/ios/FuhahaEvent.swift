import UIKit
import CoreMotion

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaEvent{
	// タッチ1情報
	internal var t1id: Int = -1;
	internal var t1x: Int32 = 0;
	internal var t1y: Int32 = 0;
	internal var t1trigger: Bool = false;
	// タッチ2情報
	internal var t2id: Int = -1;
	internal var t2x: Int32 = 0;
	internal var t2y: Int32 = 0;
	internal var t2trigger: Bool = false;
	// 加速度センサー
	internal let motionManager: CMMotionManager?;

	// ----------------------------------------------------------------

	// コンストラクタ
	init(){
		if(gameMainEventIsAcceleration()){
			// 加速度センサー
			self.motionManager = CMMotionManager();
			self.motionManager!.accelerometerUpdateInterval = 0.05;
			self.motionManager!.startAccelerometerUpdates(to: OperationQueue.current!, withHandler: {(data: CMAccelerometerData?, error: Error?) -> Void in
				if(data == nil){return;}
				// androidの軸方向に合わせる、最大値は知らん
				gameMainEventAcceleration(-data!.acceleration.x, -data!.acceleration.y, -data!.acceleration.z);
			});
		}else{
			self.motionManager = nil;
		}
	}

	// ----------------------------------------------------------------

	func onTouch(){
		if(self.t1trigger){gameMainEventTouch(0, self.t1x, self.t1y, (self.t1id >= 0))};
		if(self.t2trigger){gameMainEventTouch(1, self.t2x, self.t2y, (self.t2id >= 0))};
		self.t1trigger = false;
		self.t2trigger = false;
	}

	// タッチ開始
	func touchesBegan(_ view: UIView, touches: Set<UITouch>, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.location(in: view);
			if(self.t1id < 0){
				self.t1x = Int32(point.x);
				self.t1y = Int32(point.y);
				self.t1id = tid;
				self.t1trigger = true;
			}else if(self.t2id < 0){
				self.t2x = Int32(point.x);
				self.t2y = Int32(point.y);
				self.t2id = tid;
				self.t2trigger = true;
			}
		}
		self.onTouch();
	}

	// タッチ移動
	func touchesMoved(_ view: UIView, touches: Set<UITouch>, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.location(in: view);
			if(self.t1id == tid){
				self.t1x = Int32(point.x);
				self.t1y = Int32(point.y);
				self.t1trigger = true;
			}else if(self.t2id == tid){
				self.t2x = Int32(point.x);
				self.t2y = Int32(point.y);
				self.t2trigger = true;
			}
		}
		self.onTouch();
	}

	// タッチ完了
	func touchesEnded(_ view: UIView, touches: Set<UITouch>, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.location(in: view);
			if(self.t1id == tid){
				self.t1x = Int32(point.x);
				self.t1y = Int32(point.y);
				self.t1id = -1;
				self.t1trigger = true;
			}else if(self.t2id == tid){
				self.t2x = Int32(point.x);
				self.t2y = Int32(point.y);
				self.t2id = -1;
				self.t2trigger = true;
			}
		}
		self.onTouch();
	}

	// タッチ中止
	func touchesCancelled(_ view: UIView, touches: Set<UITouch>!, withEvent event: UIEvent?){
		for touch: UITouch in touches{
			let tid: Int = touch.hash;
			let point: CGPoint = touch.location(in: view);
			if(self.t1id == tid){
				self.t1x = Int32(point.x);
				self.t1y = Int32(point.y);
				self.t1id = -1;
				self.t1trigger = true;
			}else if(self.t2id == tid){
				self.t2x = Int32(point.x);
				self.t2y = Int32(point.y);
				self.t2id = -1;
				self.t2trigger = true;
			}
		}
		self.onTouch();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

