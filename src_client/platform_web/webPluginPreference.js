
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginPreferenceInit: function(){
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// 設定読み込み 返値文字列は揮発性バッファで解放禁止
	platformPluginPreferenceGet: function(key){
		var str = localStorage.getItem(Pointer_stringify(key));
		if(str == null){return null;}
		var buff = ccall("corePluginUtilTemporaryBuffer", null, [null], [str.length + 1]);
		Module.writeStringToMemory(str, buff);
		return buff;
	},

	// 設定書き込み
	platformPluginPreferenceSet: function(key, value){
		localStorage.setItem(Pointer_stringify(key), Pointer_stringify(value));
	},

	// 設定保存
	platformPluginPreferenceCommit: function(){
		// webでは何もしない 
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

