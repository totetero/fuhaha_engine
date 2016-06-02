
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginSoundInit: function(){
		this.platformSound = {};

		// WebAudioAPIのAudioContextを作成
		var soundContext = null;
		if(soundContext == null){try{soundContext = new AudioContext();}catch(e){}}
		if(soundContext == null){try{soundContext = new webkitAudioContext();}catch(e){}}
		if(soundContext != null){
			// WebAudioAPIのAudioContext作成成功
			var createGain = function(){return (!!soundContext.createGain) ? soundContext.createGain() : soundContext.createGainNode();};
			var sourceStart = function(source, when){if(!!source.start){source.start(when);}else{source.noteOn(when);}};
			var sourceStop = function(source, when){if(!!source.stop){source.stop(when);}else{source.noteOff(when);}};
			platformSound.bgmVolumeGain = createGain();
			platformSound.seVolumeGain = createGain();
			platformSound.bgmVolumeGain.connect(soundContext.destination);
			platformSound.seVolumeGain.connect(soundContext.destination);

			platformSound.bgmList = {};
			platformSound.seList = {};
			platformSound.bgmCurrentId = 0;
			platformSound.bgmToneDown = 1.0;

			// 音楽ファイル拡張子
			platformSound.soundSuffix = ".m4a";
			var userAgent = window.navigator.userAgent.toLowerCase();
			if(userAgent.indexOf('firefox') > 0){platformSound.soundSuffix = ".ogg";}
			if(userAgent.indexOf('opera') > 0){platformSound.soundSuffix = ".ogg";}

			// BGM再生関数
			platformSound.bgmPlay = function(bgmId){
				platformSound.bgmToneDown = 1.0;

				// 同じBGMを再生中なら何もしない
				if(platformSound.bgmCurrentId == bgmId){return;}				
				var playData = platformSound.bgmList[bgmId];
				if(playData == null){platformSound.bgmCurrentId = 0; return;}
				platformSound.bgmCurrentId = bgmId;
				if(playData.isPlaying){return;}

				playData.play(playData);
			};

			// SE再生関数
			platformSound.sePlay = function(seId){
				var playData = platformSound.seList[seId];
				if(playData.buffer == null){return;}
				var when = platformSound.soundContext.currentTime;
				var source = platformSound.soundContext.createBufferSource();
				source.buffer = playData.buffer;
				source.connect(platformSound.seVolumeGain);
				sourceStart(source, when);
			};

			// 音楽読込関数
			platformSound.soundLoader = function(playData){
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
			platformSound.soundLoaderBgm = function(src, bgmId){
				platformSound.soundLoader(platformSound.bgmList[bgmId] = {
					bgmId: bgmId,
					src: src + platformSound.soundSuffix,
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
						var isActive = (platformSound.bgmCurrentId == this.bgmId);
						if(isActive || this.countPrev > 0){
							if(!this.isPlaying && this.buffer != null){
								// BGM作成と再生
								this.gain = createGain();
								this.source = platformSound.soundContext.createBufferSource();
								this.source.loop = true;
								this.source.buffer = this.buffer;
								this.source.connect(this.gain);
								this.gain.value = 0.0;
								this.gain.connect(platformSound.bgmVolumeGain);
								sourceStart(this.source, platformSound.soundContext.currentTime);
								this.countPrev = 0;
								this.isPlaying = true;
							}

							if(this.isPlaying){
								// BGMのボリュームフェード監視	
								var countNext = isActive ? Math.round(this.countMax * platformSound.bgmToneDown) : 0;
								if(this.countPrev != countNext){
									if(countNext > this.countPrev){this.countPrev++;}
									if(countNext < this.countPrev){this.countPrev--;}
									var effectVolume = this.countPrev / this.countMax;
									this.gain.gain.setValueAtTime(effectVolume, platformSound.soundContext.currentTime);
								}
							}

							return true;
						}else{
							// BGMの停止と破棄
							sourceStop(this.source, platformSound.soundContext.currentTime);
							this.gain = null;
							this.source = null;
							this.isPlaying = false;
						}

						return false;
					},
				});
			};
			platformSound.soundLoaderSe = function(src, seId){
				platformSound.soundLoader(platformSound.seList[seId] = {
					src: src + platformSound.soundSuffix,
					buffer: null,
				});
			};

			// タッチしないと音を再生開始しない端末のための無音再生
			platformSound.soundTouch = function(){
				if(platformSound.soundTrigger){
					platformSound.soundTrigger = false;
					var source = platformSound.soundContext.createBufferSource();
					source.connect(platformSound.soundContext.destination);
					sourceStart(source, platformSound.soundContext.currentTime);
				}
			};

			platformSound.soundTrigger = true;
			platformSound.soundContext = soundContext;
		}
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// BGM読込
	platformPluginSoundBgmLoad: function(bgmId, src){
		if(platformSound.soundContext == null){return;}
		if(bgmId <= 0){return;}
		platformSound.soundLoaderBgm(Pointer_stringify(src), bgmId);
	},

	// BGM再生
	platformPluginSoundBgmPlay: function(bgmId){
		if(platformSound.soundContext == null){return;}
		platformSound.bgmPlay(bgmId);
	},

	// BGMトーンダウン
	platformPluginSoundBgmToneDown: function(volume){
		if(platformSound.soundContext == null){return;}
		platformSound.bgmToneDown = volume;
	},

	// BGM設定音量
	platformPluginSoundBgmVolume: function(volume){
		if(platformSound.soundContext == null){return;}
		platformSound.bgmVolumeGain.gain.value = volume;
	},

	// ----------------------------------------------------------------

	// SE読込
	platformPluginSoundSeLoad: function(seId, src){
		if(platformSound.soundContext == null){return;}
		if(seId <= 0){return;}
		platformSound.soundLoaderSe(Pointer_stringify(src), seId);
	},

	// SE再生
	platformPluginSoundSePlay: function(seId){
		if(platformSound.soundContext == null){return;}
		platformSound.sePlay(seId);
	},

	// SE設定音量
	platformPluginSoundSeVolume: function(volume){
		if(platformSound.soundContext == null){return;}
		platformSound.seVolumeGain.gain.value = volume;
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

