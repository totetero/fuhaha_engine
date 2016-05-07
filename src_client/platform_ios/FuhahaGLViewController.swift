import UIKit
import GLKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaGLViewController: GLKViewController{
	var context: EAGLContext? = nil;

	// ----------------------------------------------------------------

	// viewの始まり
	override func viewDidLoad(){
		super.viewDidLoad();

		self.context = EAGLContext(API: .OpenGLES2);
		let view: GLKView = self.view as! GLKView;
		view.context = self.context!;
		view.drawableDepthFormat = .Format24;
		view.drawableStencilFormat = .Format8;
		EAGLContext.setCurrentContext(self.context);

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

