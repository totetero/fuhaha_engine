import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginUtil: NSObject{
	fileprivate static let semaphore: DispatchSemaphore = DispatchSemaphore(value: 1);
	fileprivate static var counter: Int = 0;

	// ----------------------------------------------------------------

	// ユーザーID取得
	static internal func platformPluginUtilUidGet() -> NSString{
		let userDefaults: UserDefaults = UserDefaults.standard;
		let key: String = "fuhaha_user_id";
		var value: String! = userDefaults.object(forKey: key) as! String!;
		if(value == nil){
			value = UUID().uuidString;
			userDefaults.set(value, forKey: key);
			userDefaults.synchronize();
		}
		return (value as NSString);
	}

	// ----------------------------------------------------------------

	// 読み込み中確認
	static internal func platformPluginUtilIsLoading() -> Bool{
		IosPluginUtil.semaphore.wait();
		let isLoading: Bool = (IosPluginUtil.counter > 0);
		IosPluginUtil.semaphore.signal();
		return isLoading;
	}

	// 読み込み中カウンタ加算
	static internal func nativePluginUtilLoadingIncrement(){
		IosPluginUtil.semaphore.wait();
		IosPluginUtil.counter += 1;
		IosPluginUtil.semaphore.signal();
	}

	// 読み込み中カウンタ減算
	static internal func nativePluginUtilLoadingDecrement(){
		IosPluginUtil.semaphore.wait();
		IosPluginUtil.counter -= 1;
		IosPluginUtil.semaphore.signal();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

