import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginPreference: NSObject{
	// ----------------------------------------------------------------

	// 設定読み込み
	static internal func platformPluginPreferenceGet(key: String) -> NSString?{
		let userDefaults = NSUserDefaults.standardUserDefaults();
		return userDefaults.objectForKey(key) as! String?;
	}

	// 設定書き込み
	static internal func platformPluginPreferenceSet(key: String, value: String){
		let userDefaults = NSUserDefaults.standardUserDefaults();
		userDefaults.setObject(value, forKey: key);
	}

	// 設定保存
	static internal func platformPluginPreferenceCommit(){
		let userDefaults = NSUserDefaults.standardUserDefaults();
		userDefaults.synchronize();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

