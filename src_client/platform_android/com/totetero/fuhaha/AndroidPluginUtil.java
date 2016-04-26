package com.totetero.fuhaha;

import java.util.UUID;
import android.content.SharedPreferences;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
public class AndroidPluginUtil{
	 private static final Object lock = new Object();
	 private static int counter = 0;

	// ----------------------------------------------------------------

	// JNI連携
	static{System.loadLibrary("fuhaha_native");}
	public static native String gamePluginUtilUrlGet();
	public static native void gamePluginUtilCallbackBinary(int callbackId, byte[] buff, int size);
	public static native void gamePluginUtilCallbackString(int callbackId, String buff);

	// ----------------------------------------------------------------

	// ユーザーID取得
	public static String platformPluginUtilUidGet(){
		SharedPreferences pref = FuhahaGLView.activity.getSharedPreferences("fuhaha_uid", FuhahaGLView.activity.MODE_PRIVATE);
		String key = "fuhaha_user_id";
		String value = pref.getString(key, "");
		if(value == ""){
			value = UUID.randomUUID().toString();
			SharedPreferences.Editor editor = pref.edit();
			editor.putString(key, value);
			editor.commit();
		}
		return value;
	}

	// ----------------------------------------------------------------

	// 読み込み中確認
	public static int platformPluginUtilIsLoading(){
		boolean isLoading = false;
		synchronized(AndroidPluginUtil.lock){isLoading = (AndroidPluginUtil.counter > 0);}
		return isLoading ? 1 : 0;
	}

	// 読み込み中カウンタ加算
	public static void nativePluginUtilIncrementLoading(){
		synchronized(AndroidPluginUtil.lock){AndroidPluginUtil.counter++;}
	}

	// 読み込み中カウンタ減算
	public static void nativePluginUtilDecrementLoading(){
		synchronized(AndroidPluginUtil.lock){AndroidPluginUtil.counter--;}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

