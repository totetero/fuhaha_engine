import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginUtil: NSObject{
	private static let semaphore = dispatch_semaphore_create(1);
	private static var counter: Int = 0;

	// ----------------------------------------------------------------

	// ユーザーID取得
	static func platformPluginUtilUidGet() -> NSString{
		let userDefaults = NSUserDefaults.standardUserDefaults();
		let key = "fuhaha_user_id";
		var value = userDefaults.objectForKey(key) as! String!;
		if(value == nil){
			value = NSUUID().UUIDString;
			userDefaults.setObject(value, forKey: key);
			userDefaults.synchronize();
		}
		return (value as NSString);
	}

	// ----------------------------------------------------------------

	// 読み込み中確認
	static func platformPluginUtilIsLoading() -> Bool{
		dispatch_semaphore_wait(IosPluginUtil.semaphore, DISPATCH_TIME_FOREVER);
		let isLoading = (IosPluginUtil.counter > 0);
		dispatch_semaphore_signal(IosPluginUtil.semaphore);
		return isLoading;
	}

	// 読み込み中カウンタ加算
	static func nativePluginUtilLoadingIncrement(){
		dispatch_semaphore_wait(IosPluginUtil.semaphore, DISPATCH_TIME_FOREVER);
		IosPluginUtil.counter += 1;
		dispatch_semaphore_signal(IosPluginUtil.semaphore);
	}

	// 読み込み中カウンタ減算
	static func nativePluginUtilLoadingDecrement(){
		dispatch_semaphore_wait(IosPluginUtil.semaphore, DISPATCH_TIME_FOREVER);
		IosPluginUtil.counter -= 1;
		dispatch_semaphore_signal(IosPluginUtil.semaphore);
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

