import Foundation
import AVFoundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginSound: NSObject{
	fileprivate static var bgmList: Dictionary<UInt32, IosPluginSound.FuhahaBgmItem> = Dictionary<UInt32, IosPluginSound.FuhahaBgmItem>();
	fileprivate static var seList: Dictionary<UInt32, IosPluginSound.FuhahaSeItem> = Dictionary<UInt32, IosPluginSound.FuhahaSeItem>();
	fileprivate static var bgmZeroId: UInt32 = 0;
	fileprivate static var bgmCurrentId: UInt32 = 0;
	fileprivate static var seAudioPlayer: Array<AVAudioPlayer> = Array<AVAudioPlayer>();
	fileprivate static var bgmToneDown: Float = 1.0;
	fileprivate static var bgmVolume: Float = 1.0;
	fileprivate static var seVolume: Float = 1.0;
	fileprivate static let soundSuffix: String = ".m4a";

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// BGM読込
	static internal func platformPluginSoundBgmLoad(_ bgmId: UInt32, src: String){
		if(bgmId <= 0){return;}
		if(IosPluginSound.bgmList[bgmId] != nil){return;}
		IosPluginSound.bgmList[bgmId] = IosPluginSound.FuhahaBgmItem(src: src, bgmId: bgmId);
	}

	// BGM再生
	static internal func platformPluginSoundBgmPlay(_ bgmId: UInt32){
		IosPluginSound.bgmZeroId = bgmId;
		let oldVolume = IosPluginSound.bgmVolume;
		if(oldVolume > 0){IosPluginSound.bgmPlay(bgmId);}
	}

	// BGM再生
	static internal func bgmPlay(_ bgmId: UInt32){
		IosPluginSound.bgmToneDown = 1.0;

		// 同じBGMを再生中なら何もしない
		if(IosPluginSound.bgmCurrentId == bgmId){return;}
		let playData: FuhahaBgmItem? = IosPluginSound.bgmList[bgmId];
		if(playData == nil){IosPluginSound.bgmCurrentId = 0; return;}
		IosPluginSound.bgmCurrentId = bgmId;
		if(playData!.isPlaying){return;}

		playData!.play();
	}

	// BGMトーンダウン
	static internal func platformPluginSoundBgmToneDown(_ volume: Double){
		IosPluginSound.bgmToneDown = Float(volume);
	}

	// BGM設定音量
	static internal func platformPluginSoundBgmVolume(_ volume: Double){
		// 音量をゼロにした瞬間とゼロから戻した瞬間
		let oldVolume = IosPluginSound.bgmVolume;
		if(oldVolume > 0 && volume <= 0){IosPluginSound.bgmPlay(0);}
		if(oldVolume <= 0 && volume > 0){IosPluginSound.bgmPlay(IosPluginSound.bgmZeroId);}

		IosPluginSound.bgmVolume = Float(volume);
	}

	// BGM情報クラス
	class FuhahaBgmItem: NSObject, AVAudioPlayerDelegate{
		fileprivate let bgmId: UInt32;
		fileprivate let src: URL;

		fileprivate let countMax: Int = 30;
		fileprivate var countPrev: Int = 0;
		fileprivate var isPlaying: Bool = false;
		fileprivate var audioPlayer: AVAudioPlayer? = nil;
		fileprivate var settingVolume: Float = 0.0;

		// コンストラクタ
		init(src: String, bgmId: UInt32){
			self.bgmId = bgmId;
			self.src = Bundle.main.url(forResource: src, withExtension: IosPluginSound.soundSuffix, subdirectory: "assets")!;
		}

		// 再生開始
		func play(){
			self.playTimer(nil);
		}

		// タイマーコールバック
		func playTimer(_ timer: Timer?){
			if(self.playing()){
				Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(self.playTimer(_:)), userInfo: nil, repeats: false);
			}
		}

		// 再生関数
		fileprivate func playing() -> Bool{
			let isActive: Bool = (IosPluginSound.bgmCurrentId == self.bgmId);
			if(isActive || self.countPrev > 0){
				if(!self.isPlaying){
					// BGM作成と再生
					do{
						let audioPlayer: AVAudioPlayer = try AVAudioPlayer(contentsOf: self.src);
						audioPlayer.volume = 0.0;
						audioPlayer.delegate = self;
						audioPlayer.numberOfLoops = -1;
						audioPlayer.prepareToPlay();
						audioPlayer.play();
						self.countPrev = 0;
						self.isPlaying = true;
						self.audioPlayer = audioPlayer;
						self.settingVolume = 0.0;
					}catch{}
				}

				if(self.isPlaying){
					// BGMのボリュームフェード監視
					let countNext: Int = isActive ? Int(round(Float(self.countMax) * IosPluginSound.bgmToneDown)) : 0;
					if(self.countPrev != countNext || self.settingVolume != IosPluginSound.bgmVolume){
						self.settingVolume = IosPluginSound.bgmVolume;
						if(countNext > self.countPrev){self.countPrev += 1;}
						if(countNext < self.countPrev){self.countPrev -= 1;}
						let effectVolume: Float = Float(self.countPrev) / Float(self.countMax);
						self.audioPlayer!.volume = self.settingVolume * effectVolume;
					}
				}

				return true;
			}else{
				// BGMの停止と破棄
				self.audioPlayer!.stop();
				self.audioPlayer = nil;
				self.isPlaying = false;
			}

			return false;
		}
	}

	// ----------------------------------------------------------------

	// SE読込
	static internal func platformPluginSoundSeLoad(_ seId: UInt32, src: String){
		if(seId <= 0){return;}
		if(IosPluginSound.seList[seId] != nil){return;}
		IosPluginSound.seList[seId] = IosPluginSound.FuhahaSeItem(src: src);
	}

	// SE再生
	static internal func platformPluginSoundSePlay(_ seId: UInt32){
		IosPluginSound.seList[seId]?.play();
	}

	// SE設定音量
	static internal func platformPluginSoundSeVolume(_ volume: Double){
		IosPluginSound.seVolume = Float(volume);
	}

	// SE情報クラス
	class FuhahaSeItem: NSObject, AVAudioPlayerDelegate{
		fileprivate let src: URL;

		// コンストラクタ
		init(src: String){
			self.src = Bundle.main.url(forResource: src, withExtension: IosPluginSound.soundSuffix, subdirectory: "assets")!;
		}

		// 再生開始
		func play(){
			do{
				let audioPlayer: AVAudioPlayer = try AVAudioPlayer(contentsOf: self.src);
				IosPluginSound.seAudioPlayer.append(audioPlayer);
				audioPlayer.volume = IosPluginSound.seVolume;
				audioPlayer.delegate = self;
				audioPlayer.numberOfLoops = 0;
				audioPlayer.prepareToPlay();
				audioPlayer.play();
			}catch{}
		}

		// 再生完了
		func audioPlayerDidFinishPlaying(_ player: AVAudioPlayer, successfully flag: Bool){
			let index: Int? = IosPluginSound.seAudioPlayer.index(of: player);
			if(index != nil){IosPluginSound.seAudioPlayer.remove(at: index!);}
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

