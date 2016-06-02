package com.totetero.fuhaha;

import android.media.MediaPlayer;
import android.media.SoundPool;
import android.media.AudioManager;
import android.media.AudioAttributes;
import android.content.res.AssetFileDescriptor;

import java.util.HashMap;
import java.util.Map.Entry;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
public class AndroidPluginSound{
	private static HashMap<Integer, FuhahaBgmItem> bgmList = new HashMap<Integer, FuhahaBgmItem>();
	private static HashMap<Integer, FuhahaSeItem> seList = new HashMap<Integer, FuhahaSeItem>();
	private static int bgmCurrentId = 0;
	private static SoundPool seSoundPool = null;
	private static float bgmToneDown = 1.0f;
	private static float bgmVolume = 1.0f;
	private static float seVolume = 1.0f;
	private static String soundSuffix = ".m4a";

	// ----------------------------------------------------------------

	// 開始時
	public static void onResume(){
		// BGM再開
		AndroidPluginSound.platformPluginSoundBgmPlay(AndroidPluginSound.bgmCurrentId);
		// SE読込
		if(android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP){
			AndroidPluginSound.seSoundPool = new SoundPool(3, AudioManager.STREAM_MUSIC, 0);
		}else{
			AudioAttributes attr = new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_MEDIA).setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build();
			AndroidPluginSound.seSoundPool = new SoundPool.Builder().setAudioAttributes(attr).setMaxStreams(3).build();
		}
		for(Entry<Integer, FuhahaSeItem> entry : AndroidPluginSound.seList.entrySet()){entry.getValue().load();}
	}

	// 停止時
	public static void onPause(){
		// BGM停止
		AndroidPluginSound.bgmCurrentId = 0;
		// SE解放
		for(Entry<Integer, FuhahaSeItem> entry : AndroidPluginSound.seList.entrySet()){entry.getValue().unload();}
		AndroidPluginSound.seSoundPool.release();
	}

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// BGM読込
	public static void platformPluginSoundBgmLoad(int bgmId, String src){
		if(bgmId <= 0){return;}
		AndroidPluginSound.FuhahaBgmItem playData = new AndroidPluginSound.FuhahaBgmItem(src, bgmId);
		AndroidPluginSound.bgmList.put(bgmId, playData);
	}

	// BGM再生
	public static void platformPluginSoundBgmPlay(int bgmId){
		AndroidPluginSound.bgmToneDown = 1.0f;

		// 同じBGMを再生中なら何もしない
		if(AndroidPluginSound.bgmCurrentId == bgmId){return;}
		AndroidPluginSound.FuhahaBgmItem playData = AndroidPluginSound.bgmList.get(bgmId);
		if(playData == null){AndroidPluginSound.bgmCurrentId = 0; return;}
		AndroidPluginSound.bgmCurrentId = bgmId;
		if(playData.isPlaying){return;}

		playData.play();
	}

	// BGMトーンダウン
	public static void platformPluginSoundBgmToneDown(double volume){
		AndroidPluginSound.bgmToneDown = (float)volume;
	}

	// BGM設定音量
	public static void platformPluginSoundBgmVolume(double volume){
		AndroidPluginSound.bgmVolume = (float)volume;
	}

	// BGM情報クラス
	static class FuhahaBgmItem implements Runnable{
		private int bgmId;
		private String src;

		private int countMax = 30;
		private int countPrev = 0;
		private boolean isPlaying = false;
		private MediaPlayer mediaPlayer = null;
		private float settingVolume = 0.0f;

		public FuhahaBgmItem(String src, int bgmId){
			this.bgmId = bgmId;
			this.src = src + AndroidPluginSound.soundSuffix;
		}

		// 再生開始
		public void play(){
			new Thread(this).start();
		}

		// スレッド
		public void run(){
			while(this.playing()){
				try{Thread.sleep(100);}catch(Exception e){e.printStackTrace();}
			}
		}

		// 再生関数
		private boolean playing(){
			boolean isActive = (AndroidPluginSound.bgmCurrentId == this.bgmId);
			if(isActive || this.countPrev > 0){
				if(!this.isPlaying){
					// BGM作成と再生
					try{
						this.mediaPlayer = new MediaPlayer();
						AssetFileDescriptor afd = FuhahaGLView.activity.getAssets().openFd(this.src);
						this.mediaPlayer.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
						this.mediaPlayer.prepare();
					}catch(Exception e){e.printStackTrace();}
					this.mediaPlayer.setLooping(true);
					this.mediaPlayer.setVolume(0.0f, 0.0f);
					this.mediaPlayer.seekTo(0);
					this.mediaPlayer.start();
					this.countPrev = 0;
					this.isPlaying = true;
					this.settingVolume = 0.0f;
				}

				if(this.isPlaying){
					// BGMのボリュームフェード監視
					int countNext = isActive ? Math.round(this.countMax * AndroidPluginSound.bgmToneDown) : 0;
					if(this.countPrev != countNext || this.settingVolume != AndroidPluginSound.bgmVolume){
						this.settingVolume = AndroidPluginSound.bgmVolume;
						if(countNext > this.countPrev){this.countPrev++;}
						if(countNext < this.countPrev){this.countPrev--;}
						float effectVolume = (float)this.countPrev / this.countMax;
						float volume = this.settingVolume * effectVolume;
						this.mediaPlayer.setVolume(volume, volume);
					}
				}

				return true;
			}else{
				// BGMの停止と破棄
				this.mediaPlayer.stop();
				this.mediaPlayer.release();
				this.mediaPlayer = null;
				this.isPlaying = false;
			}

			return false;
		}
	}

	// ----------------------------------------------------------------

	// SE読込
	public static void platformPluginSoundSeLoad(int seId, String src){
		if(seId <= 0){return;}
		AndroidPluginSound.FuhahaSeItem playData = new AndroidPluginSound.FuhahaSeItem(src);
		AndroidPluginSound.seList.put(seId, playData);
		playData.load();
	}

	// SE再生
	public static void platformPluginSoundSePlay(int seId){
		AndroidPluginSound.FuhahaSeItem playData = AndroidPluginSound.seList.get(seId);
		if(playData != null){playData.play();}
	}

	// SE設定音量
	public static void platformPluginSoundSeVolume(double volume){
		AndroidPluginSound.seVolume = (float)volume;
	}

	// SE情報クラス
	static class FuhahaSeItem{
		private String src;
		private int soundId;

		public FuhahaSeItem(String src){
			this.src = src + AndroidPluginSound.soundSuffix;
		}

		public void load(){
			try{
				AssetFileDescriptor afd = FuhahaGLView.activity.getAssets().openFd(this.src);
				this.soundId = AndroidPluginSound.seSoundPool.load(afd, 1);
			}catch(Exception e){e.printStackTrace();}
		}

		public void play(){
			float volume = AndroidPluginSound.seVolume;
			AndroidPluginSound.seSoundPool.play(this.soundId, volume, volume, 0, 0, 1);
		}

		public void unload(){
			AndroidPluginSound.seSoundPool.unload(this.soundId);
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

