import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginSound: NSObject{
	// ----------------------------------------------------------------

	// プラグイン初期化
	static internal func iosPluginSoundInit(){
	}

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// BGM読込
	static internal func platformPluginSoundBgmLoad(bgmId: UInt32, src: String){
	}

	// BGM再生
	static internal func platformPluginSoundBgmPlay(bgmId: UInt32){
	}

	// BGMトーンダウン
	static internal func platformPluginSoundBgmToneDown(volume: Double){
	}

	// BGM設定音量
	static internal func platformPluginSoundBgmVolume(volume: Double){
	}

	// ----------------------------------------------------------------

	// SE読込
	static internal func platformPluginSoundSeLoad(seId: UInt32, src: String){
	}

	// SE再生
	static internal func platformPluginSoundSePlay(seId: UInt32){
	}

	// SE設定音量
	static internal func platformPluginSoundSeVolume(volume: Double){
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

