package com.totetero.fuhaha;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
public class AndroidPluginSound{
	// ----------------------------------------------------------------

	// 開始時
	public static void onResume(){
	}

	// 停止時
	public static void onPause(){
	}

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// JNI連携 BGM読込
	public static void platformPluginSoundBgmLoad(int bgmId, String src){
	}

	// JNI連携 BGM再生
	public static void platformPluginSoundBgmPlay(int bgmId){
	}

	// JNI連携 BGMトーンダウン
	public static void platformPluginSoundBgmToneDown(double volume){
	}

	// JNI連携 BGM設定音量
	public static void platformPluginSoundBgmVolume(double volume){
	}

	// ----------------------------------------------------------------

	// JNI連携 SE読込
	public static void platformPluginSoundSeLoad(int seId, String src){
	}

	// JNI連携 SE再生
	public static void platformPluginSoundSePlay(int seId){
	}

	// JNI連携 SE設定音量
	public static void platformPluginSoundSeVolume(double volume){
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

