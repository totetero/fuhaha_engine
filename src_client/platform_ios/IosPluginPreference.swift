import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginPreference: NSObject{
	// ----------------------------------------------------------------

	// 設定読み込み
	static internal func platformPluginPreferenceGet(_ key: String) -> NSString?{
		let userDefaults = UserDefaults.standard;
		return userDefaults.object(forKey: key) as! String? as NSString?;
	}

	// 設定書き込み
	static internal func platformPluginPreferenceSet(_ key: String, value: String){
		let userDefaults = UserDefaults.standard;
		userDefaults.set(value, forKey: key);
	}

	// 設定保存
	static internal func platformPluginPreferenceCommit(){
		let userDefaults = UserDefaults.standard;
		userDefaults.synchronize();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

