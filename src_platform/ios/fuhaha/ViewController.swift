import UIKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class ViewController: UIViewController{
	private var glViewController : FuhahaGLViewController = FuhahaGLViewController();

	override func viewDidLoad(){
		super.viewDidLoad();
		// Do any additional setup after loading the view, typically from a nib.
		view.addSubview(glViewController.view);
	}

	override func didReceiveMemoryWarning(){
		super.didReceiveMemoryWarning();
		// Dispose of any resources that can be recreated.
	}
}
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

