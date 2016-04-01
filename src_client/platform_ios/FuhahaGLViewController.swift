import UIKit
import GLKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaGLViewController: GLKViewController{
	var context: EAGLContext? = nil;

	override func viewDidLoad(){
		super.viewDidLoad();

		self.context = EAGLContext(API: .OpenGLES2);
		let view = self.view as! GLKView;
		view.context = self.context!;
		view.drawableDepthFormat = .Format24;
		view.drawableStencilFormat = .Format8;
		EAGLContext.setCurrentContext(self.context);

		gameSurfaceCreated();
	}

	override func glkView(view: GLKView, drawInRect rect: CGRect){
		gameSurfaceDrawFrame();
	}

	override func viewDidLayoutSubviews(){
		super.viewDidLayoutSubviews();
		gameSurfaceChanged(Int32(self.view.frame.size.width), Int32(self.view.frame.size.height), 1.0);
	}

	static func applicationWillResignActive(){}
	static func applicationDidEnterBackground(){}
	static func applicationWillEnterForeground(){}
	static func applicationDidBecomeActive(){}
	static func applicationWillTerminate(){}

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

	deinit{
		gameSurfaceDestroy();
		if(EAGLContext.currentContext() === self.context){
			EAGLContext.setCurrentContext(nil);
		}
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

