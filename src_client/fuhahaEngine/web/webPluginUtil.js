
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginUtilInit: function(){
		// 読み込み中カウンタ
		Module.privatePluginUtilCounter = 0;

		// 読み込み中カウンタ加算
		Module.nativePluginUtilLoadingIncrement = function(){
			Module.privatePluginUtilCounter++;
		};

		// 読み込み中カウンタ減算
		Module.nativePluginUtilLoadingDecrement = function(){
			Module.privatePluginUtilCounter--;
		};
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// プラットフォーム名取得 返値文字列は揮発性バッファで解放禁止
	platformPluginUtilPlatformGet: function(){
		var value = "web";
		var size = lengthBytesUTF8(value) + 1;
		var buff = ccall("corePluginUtilMemoryTemporary", null, [null], [size]);
		stringToUTF8(value, buff, size);
		return buff;
	},

	// ユーザーID取得 返値文字列は揮発性バッファで解放禁止
	platformPluginUtilUidGet: function(){
		var key = "fuhaha_user_id";
		var value = localStorage.getItem(key);
		if(value == null){
			var S4 = function(){return (((1 + Math.random()) * 0x10000) | 0).toString(16).substring(1);};
			value = (S4() + S4() + "-" + S4() + "-" + S4() + "-" + S4() + "-" + S4() + S4() + S4());
			localStorage.setItem(key, value);
		}
		var size = lengthBytesUTF8(value) + 1;
		var buff = ccall("corePluginUtilMemoryTemporary", null, [null], [size]);
		stringToUTF8(value, buff, size);
		return buff;
	},

	// ----------------------------------------------------------------

	// unix時間取得
	platformPluginUtilTimeGet: function(){
		return Math.floor((new Date()).getTime() / 1000);
	},

	// ----------------------------------------------------------------

	// 読み込み中確認
	platformPluginUtilIsLoading: function(){
		return (Module.privatePluginUtilCounter > 0);
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	nativePluginUtilLoadingIncrement: function(){return Module.nativePluginUtilLoadingIncrement.apply(Module, arguments);},
	nativePluginUtilLoadingDecrement: function(){return Module.nativePluginUtilLoadingDecrement.apply(Module, arguments);},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

