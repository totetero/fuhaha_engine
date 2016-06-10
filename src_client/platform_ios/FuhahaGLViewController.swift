import UIKit
import GLKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaGLViewController: GLKViewController{
	var context: EAGLContext? = nil;
	var fuhahaCtrl: FuhahaCtrl? = nil;

	// ----------------------------------------------------------------

	// viewの始まり
	override func viewDidLoad(){
		super.viewDidLoad();

		// openGL
		self.context = EAGLContext(API: .OpenGLES2);
		let view: GLKView = self.view as! GLKView;
		view.context = self.context!;
		view.drawableDepthFormat = .Format24;
		view.drawableStencilFormat = .Format8;
		EAGLContext.setCurrentContext(self.context);

		// controller
		self.fuhahaCtrl = FuhahaCtrl();

		gameSurfaceCreated();
	}

	// 描画時
	override func glkView(view: GLKView, drawInRect rect: CGRect){
		gameSurfaceDrawFrame();
	}

	// 画面サイズ変更時
	override func viewDidLayoutSubviews(){
		super.viewDidLayoutSubviews();
		gameSurfaceChanged(Int32(self.view.frame.size.width), Int32(self.view.frame.size.height), 1.0);
	}

	// ----------------------------------------------------------------

	// ライフタイムイベント アプリ閉じそう
	static internal func applicationWillResignActive(){
	}

	// ライフタイムイベント アプリ閉じた
	static internal func applicationDidEnterBackground(){
		gameSurfacePause();
	}

	// ライフタイムイベント アプリ開きそう
	static internal func applicationWillEnterForeground(){
	}

	// ライフタイムイベント アプリ開いた
	static internal func applicationDidBecomeActive(){
	}

	// ライフタイムイベント フリックしてアプリを終了させた
	static internal func applicationWillTerminate(){
		gameSurfaceDestroy();
	}

	// ----------------------------------------------------------------

	// タッチ開始
	override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?){
		self.fuhahaCtrl!.touchesBegan(self.view, touches: touches, withEvent: event);
	}

	// タッチ移動
	override func touchesMoved(touches: Set<UITouch>, withEvent event: UIEvent?){
		self.fuhahaCtrl!.touchesMoved(self.view, touches: touches, withEvent: event);
	}

	// タッチ完了
	override func touchesEnded(touches: Set<UITouch>, withEvent event: UIEvent?){
		self.fuhahaCtrl!.touchesEnded(self.view, touches: touches, withEvent: event);
	}

	// タッチ中止
	override func touchesCancelled(touches: Set<UITouch>!, withEvent event: UIEvent?){
		self.fuhahaCtrl!.touchesCancelled(self.view, touches: touches, withEvent: event);
	}

	// ----------------------------------------------------------------

	// メモリがやばい
	override func didReceiveMemoryWarning(){
		super.didReceiveMemoryWarning();
		gameSurfaceDestroy();
		if(self.isViewLoaded() && (self.view.window != nil)){
			self.view = nil;
			if(EAGLContext.currentContext() === self.context){
				EAGLContext.setCurrentContext(nil);
			}
			self.context = nil;
		}
	}

	// 解放時
	deinit{
		gameSurfaceDestroy();
		if(EAGLContext.currentContext() === self.context){
			EAGLContext.setCurrentContext(nil);
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

