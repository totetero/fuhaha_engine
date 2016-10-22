import UIKit
import GLKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaGLViewController: GLKViewController{
	var context: EAGLContext? = nil;
	var fuhahaEvent: FuhahaEvent? = nil;

	// ----------------------------------------------------------------

	// viewの始まり
	override func viewDidLoad(){
		super.viewDidLoad();

		// openGL
		self.context = EAGLContext(api: .openGLES2);
		self.preferredFramesPerSecond = 60;
		let view: GLKView = self.view as! GLKView;
		view.context = self.context!;
		view.drawableDepthFormat = .format24;
		view.drawableStencilFormat = .format8;
		EAGLContext.setCurrent(self.context);

		// controller
		self.fuhahaEvent = FuhahaEvent();

		gameMainSurfaceCreated();
		gameMainEventInit();
	}

	// 描画時
	override func glkView(_ view: GLKView, drawIn rect: CGRect){
		gameMainSurfaceDrawFrame();
	}

	// 画面サイズ変更時
	override func viewDidLayoutSubviews(){
		super.viewDidLayoutSubviews();
		gameMainEventScreen(Int32(self.view.frame.size.width), Int32(self.view.frame.size.height), 1.0);
	}

	// ----------------------------------------------------------------

	// ライフタイムイベント アプリ閉じそう
	static internal func applicationWillResignActive(){
	}

	// ライフタイムイベント アプリ閉じた
	static internal func applicationDidEnterBackground(){
		gameMainSurfacePause();
	}

	// ライフタイムイベント アプリ開きそう
	static internal func applicationWillEnterForeground(){
	}

	// ライフタイムイベント アプリ開いた
	static internal func applicationDidBecomeActive(){
	}

	// ライフタイムイベント フリックしてアプリを終了させた
	static internal func applicationWillTerminate(){
		gameMainSurfaceDestroy();
	}

	// ----------------------------------------------------------------

	// タッチ開始
	override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?){
		self.fuhahaEvent!.touchesBegan(self.view, touches: touches, withEvent: event);
	}

	// タッチ移動
	override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?){
		self.fuhahaEvent!.touchesMoved(self.view, touches: touches, withEvent: event);
	}

	// タッチ完了
	override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?){
		self.fuhahaEvent!.touchesEnded(self.view, touches: touches, withEvent: event);
	}

	// タッチ中止
	override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?){
		self.fuhahaEvent!.touchesCancelled(self.view, touches: touches, withEvent: event);
	}

	// ----------------------------------------------------------------

	// アプリ終了命令
	static internal func platformMainSurfaceExit(){
		exit(1);
	}

	// ----------------------------------------------------------------

	// メモリがやばい
	override func didReceiveMemoryWarning(){
		super.didReceiveMemoryWarning();
		gameMainSurfaceDestroy();
		if(self.isViewLoaded && (self.view.window != nil)){
			self.view = nil;
			if(EAGLContext.current() === self.context){
				EAGLContext.setCurrent(nil);
			}
			self.context = nil;
		}
	}

	// 解放時
	deinit{
		gameMainSurfaceDestroy();
		if(EAGLContext.current() === self.context){
			EAGLContext.setCurrent(nil);
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

