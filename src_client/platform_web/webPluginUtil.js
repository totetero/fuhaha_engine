
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginUtilInit: function(){
		// 文字列領域
		var buff = null;

		// 文字列領域作成関数
		Module.privatePluginUtilPutBuff = function(value){
			if(buff != null){Module._free(buff);}
			buff = Module._malloc(value.length + 1);
			Module.writeStringToMemory(value, buff);
			return buff;
		};

		// ----------------------------------------------------------------

		// 読み込み中カウンタ
		Module.privatePluginUtilCounter = 0;

		// 読み込み中カウンタ加算
		Module.nativePluginUtilIncrementLoading = function(){
			Module.privatePluginUtilCounter++;
		};

		// 読み込み中カウンタ減算
		Module.nativePluginUtilDecrementLoading = function(){
			Module.privatePluginUtilCounter--;
		};
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// プラットフォーム名取得 返値文字列は解放禁止
	platformPluginUtilPlatformGet: function(){
		var value = "web";
		return Module.privatePluginUtilPutBuff(value);
	},

	// ユーザーID取得 返値文字列は解放禁止
	platformPluginUtilUidGet: function(){
		var key = "fuhaha_user_id";
		var value = localStorage.getItem(key);
		if(value == null){
			var S4 = function(){return (((1 + Math.random()) * 0x10000) | 0).toString(16).substring(1);};
			value = (S4() + S4() + "-" + S4() + "-" + S4() + "-" + S4() + "-" + S4() + S4() + S4());
			localStorage.setItem(key, value);
		}
		return Module.privatePluginUtilPutBuff(value);
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

	nativePluginUtilIncrementLoading: function(){return Module.nativePluginUtilIncrementLoading.apply(Module, arguments);},
	nativePluginUtilDecrementLoading: function(){return Module.nativePluginUtilDecrementLoading.apply(Module, arguments);},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

