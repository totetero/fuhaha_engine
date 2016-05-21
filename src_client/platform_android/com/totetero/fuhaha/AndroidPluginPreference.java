package com.totetero.fuhaha;

import android.content.SharedPreferences;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
public class AndroidPluginPreference{
	// ----------------------------------------------------------------

	// 設定準備
	private static SharedPreferences pref = null;
	private static SharedPreferences.Editor editor = null;
	private static SharedPreferences getPref(){if(AndroidPluginPreference.pref == null){AndroidPluginPreference.pref = FuhahaGLView.activity.getSharedPreferences("fuhaha", FuhahaGLView.activity.MODE_PRIVATE);} return AndroidPluginPreference.pref;}
	private static SharedPreferences.Editor getEdit(){if(AndroidPluginPreference.editor == null){AndroidPluginPreference.editor = AndroidPluginPreference.getPref().edit();} return AndroidPluginPreference.editor;}

	// 設定読み込み
	public static String platformPluginPreferenceGet(String key){
		return AndroidPluginPreference.getPref().getString(key, null);
	}

	// 設定書き込み
	public static void platformPluginPreferenceSet(String key, String value){
		AndroidPluginPreference.getEdit().putString(key, value);
	}

	// 設定保存
	public static void platformPluginPreferenceCommit(){
		AndroidPluginPreference.getEdit().commit();
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

