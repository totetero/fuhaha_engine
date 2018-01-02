
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// 設定読み込み 返値文字列は揮発性バッファで解放禁止
	platformPluginPreferenceGet: function(key){
		var str = localStorage.getItem(Pointer_stringify(key));
		if(str == null){return null;}
		var size = lengthBytesUTF8(str) + 1;
		var buff = ccall("corePluginUtilMemoryTemporary", null, [null], [size]);
		stringToUTF8(str, buff, size);
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

