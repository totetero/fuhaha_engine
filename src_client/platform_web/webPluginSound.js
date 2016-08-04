
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginSoundInit: function(){
		this.globalWebPluginSound = {};

		// WebAudioAPIのAudioContextを作成
		var soundContext = null;
		if(soundContext == null){try{soundContext = new AudioContext();}catch(e){}}
		if(soundContext == null){try{soundContext = new webkitAudioContext();}catch(e){}}
		if(soundContext != null){
			// WebAudioAPIのAudioContext作成成功
			var createGain = function(){return (!!soundContext.createGain) ? soundContext.createGain() : soundContext.createGainNode();};
			var sourceStart = function(source, when){if(!!source.start){source.start(when);}else{source.noteOn(when);}};
			var sourceStop = function(source, when){if(!!source.stop){source.stop(when);}else{source.noteOff(when);}};
			globalWebPluginSound.bgmVolumeGain = createGain();
			globalWebPluginSound.seVolumeGain = createGain();
			globalWebPluginSound.bgmVolumeGain.connect(soundContext.destination);
			globalWebPluginSound.seVolumeGain.connect(soundContext.destination);

			globalWebPluginSound.bgmList = {};
			globalWebPluginSound.seList = {};
			globalWebPluginSound.bgmZeroId = 0;
			globalWebPluginSound.bgmCurrentId = 0;
			globalWebPluginSound.bgmToneDown = 1.0;

			// 音楽ファイル拡張子
			globalWebPluginSound.soundSuffix = ".m4a";
			var userAgent = window.navigator.userAgent.toLowerCase();
			if(userAgent.indexOf('firefox') > 0){globalWebPluginSound.soundSuffix = ".ogg";}
			if(userAgent.indexOf('opera') > 0){globalWebPluginSound.soundSuffix = ".ogg";}

			// BGM再生関数
			globalWebPluginSound.bgmPlay = function(bgmId){
				globalWebPluginSound.bgmToneDown = 1.0;

				// 同じBGMを再生中なら何もしない
				if(globalWebPluginSound.bgmCurrentId == bgmId){return;}				
				var playData = globalWebPluginSound.bgmList[bgmId];
				if(playData == null){globalWebPluginSound.bgmCurrentId = 0; return;}
				globalWebPluginSound.bgmCurrentId = bgmId;
				if(playData.isPlaying){return;}

				playData.play(playData);
			};

			// SE再生関数
			globalWebPluginSound.sePlay = function(seId){
				var playData = globalWebPluginSound.seList[seId];
				if(playData.buffer == null){return;}
				var when = globalWebPluginSound.soundContext.currentTime;
				var source = globalWebPluginSound.soundContext.createBufferSource();
				source.buffer = playData.buffer;
				source.connect(globalWebPluginSound.seVolumeGain);
				sourceStart(source, when);
			};

			// 音楽読込関数
			globalWebPluginSound.soundLoader = function(playData){
				var xhr = new XMLHttpRequest();
				xhr.open("GET", playData.src, true);
				xhr.responseType = "arraybuffer";
				xhr.onreadystatechange = function(e){
					if(xhr.readyState == 4 && xhr.status == 200){
						soundContext.decodeAudioData(xhr.response, function(buffer){
							playData.buffer = buffer;
						});
					}
				};
				xhr.send();
			};
			globalWebPluginSound.soundLoaderBgm = function(src, bgmId){
				globalWebPluginSound.soundLoader(globalWebPluginSound.bgmList[bgmId] = {
					bgmId: bgmId,
					src: src + globalWebPluginSound.soundSuffix,
					countMax: 30,
					countPrev: 0,
					isPlaying: false,
					buffer: null,
					// 再生開始
					play: function(){this.callback(this);},
					// タイマーコールバック
					callback: function(self){if(self.playing()){setTimeout(self.callback, 100, self);}},
					// 再生関数
					playing: function(){
						var isActive = (globalWebPluginSound.bgmCurrentId == this.bgmId);
						if(isActive || this.countPrev > 0){
							if(!this.isPlaying && this.buffer != null){
								// BGM作成と再生
								this.gain = createGain();
								this.source = globalWebPluginSound.soundContext.createBufferSource();
								this.source.loop = true;
								this.source.buffer = this.buffer;
								this.source.connect(this.gain);
								this.gain.value = 0.0;
								this.gain.connect(globalWebPluginSound.bgmVolumeGain);
								sourceStart(this.source, globalWebPluginSound.soundContext.currentTime);
								this.countPrev = 0;
								this.isPlaying = true;
							}

							if(this.isPlaying){
								// BGMのボリュームフェード監視	
								var countNext = isActive ? Math.round(this.countMax * globalWebPluginSound.bgmToneDown) : 0;
								if(this.countPrev != countNext){
									if(countNext > this.countPrev){this.countPrev++;}
									if(countNext < this.countPrev){this.countPrev--;}
									var effectVolume = this.countPrev / this.countMax;
									this.gain.gain.setValueAtTime(effectVolume, globalWebPluginSound.soundContext.currentTime);
								}
							}

							return true;
						}else{
							// BGMの停止と破棄
							sourceStop(this.source, globalWebPluginSound.soundContext.currentTime);
							this.gain = null;
							this.source = null;
							this.isPlaying = false;
						}

						return false;
					},
				});
			};
			globalWebPluginSound.soundLoaderSe = function(src, seId){
				globalWebPluginSound.soundLoader(globalWebPluginSound.seList[seId] = {
					src: src + globalWebPluginSound.soundSuffix,
					buffer: null,
				});
			};

			// タッチしないと音を再生開始しない端末のための無音再生
			globalWebPluginSound.soundTouch = function(){
				if(globalWebPluginSound.soundTrigger){
					globalWebPluginSound.soundTrigger = false;
					var source = globalWebPluginSound.soundContext.createBufferSource();
					source.connect(globalWebPluginSound.soundContext.destination);
					sourceStart(source, globalWebPluginSound.soundContext.currentTime);
				}
			};

			globalWebPluginSound.soundTrigger = true;
			globalWebPluginSound.soundContext = soundContext;
		}
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// BGM読込
	platformPluginSoundBgmLoad: function(bgmId, src){
		if(globalWebPluginSound.soundContext == null){return;}
		if(bgmId <= 0){return;}
		globalWebPluginSound.soundLoaderBgm(Pointer_stringify(src), bgmId);
	},

	// BGM再生
	platformPluginSoundBgmPlay: function(bgmId){
		if(globalWebPluginSound.soundContext == null){return;}
		globalWebPluginSound.bgmZeroId = bgmId;
		var oldVolume = globalWebPluginSound.bgmVolumeGain.gain.value;
		if(oldVolume > 0){globalWebPluginSound.bgmPlay(bgmId);}
	},

	// BGMトーンダウン
	platformPluginSoundBgmToneDown: function(volume){
		if(globalWebPluginSound.soundContext == null){return;}
		globalWebPluginSound.bgmToneDown = volume;
	},

	// BGM設定音量
	platformPluginSoundBgmVolume: function(volume){
		if(globalWebPluginSound.soundContext == null){return;}

		// 音量をゼロにした瞬間とゼロから戻した瞬間
		var oldVolume = globalWebPluginSound.bgmVolumeGain.gain.value;
		if(oldVolume > 0 && volume <= 0){globalWebPluginSound.bgmPlay(0);}
		if(oldVolume <= 0 && volume > 0){globalWebPluginSound.bgmPlay(globalWebPluginSound.bgmZeroId);}

		globalWebPluginSound.bgmVolumeGain.gain.value = volume;
	},

	// ----------------------------------------------------------------

	// SE読込
	platformPluginSoundSeLoad: function(seId, src){
		if(globalWebPluginSound.soundContext == null){return;}
		if(seId <= 0){return;}
		globalWebPluginSound.soundLoaderSe(Pointer_stringify(src), seId);
	},

	// SE再生
	platformPluginSoundSePlay: function(seId){
		if(globalWebPluginSound.soundContext == null){return;}
		globalWebPluginSound.sePlay(seId);
	},

	// SE設定音量
	platformPluginSoundSeVolume: function(volume){
		if(globalWebPluginSound.soundContext == null){return;}
		globalWebPluginSound.seVolumeGain.gain.value = volume;
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

