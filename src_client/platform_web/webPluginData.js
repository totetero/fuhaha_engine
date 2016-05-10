
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginDataInit: function(){
	},

	// ----------------------------------------------------------------
	// ----------------------------------------------------------------
	// ----------------------------------------------------------------

	// HTTP通信 コールバックバッファは要解放
	platformPluginDataHttp: function(param, url, request, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginUtilCallbackSet", null, [null, null], [param, callback]);
		var xhr = new XMLHttpRequest();
		xhr.open("POST", "../" + Pointer_stringify(url), true);
		//xhr.open("POST", Pointer_stringify(ccall("corePluginUtilUrlGet")) + "/" + Pointer_stringify(url), true);
		xhr.setRequestHeader("Content-Type", "application/octet-stream");
		xhr.responseType = "arraybuffer";
		xhr.onreadystatechange = function(e){
			if(xhr.readyState == 4){
				var size = 0;
				var buff = null;
				if(xhr.status == 200 && xhr.response != null){
					// 通信成功
					var byteArray = new Uint8Array(xhr.response);
					size = byteArray.byteLength;
					if(size > 0){
						buff = Module._malloc(size);
						Module.writeArrayToMemory(byteArray, buff);
					}
				}else{
					//// 通信失敗
					//var message = "failed";
					//size = message.length + 1;
					//buff = Module._malloc(size);
					//Module.writeStringToMemory(message, buff);
				}
				Module.nativePluginUtilLoadingDecrement();
				ccall("gamePluginUtilCallbackCall", null, [null, null, null], [callbackId, buff, size]);
			}
		};
		xhr.send(Pointer_stringify(request));
	},

	// ----------------------------------------------------------------

	// ローカルデータ読み込み コールバックバッファは要解放
	platformPluginDataLocal: function(param, src, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginUtilCallbackSet", null, [null, null], [param, callback]);
		var xhr = new XMLHttpRequest();
		xhr.open("GET", Pointer_stringify(src), true);
		xhr.setRequestHeader("Content-Type", "application/octet-stream");
		xhr.responseType = "arraybuffer";
		xhr.onreadystatechange = function(e){
			if(xhr.readyState == 4){
				var size = 0;
				var buff = null;
				if(xhr.status == 200 && xhr.response != null){
					// 通信成功
					var byteArray = new Uint8Array(xhr.response);
					size = byteArray.byteLength;
					if(size > 0){
						buff = Module._malloc(size);
						Module.writeArrayToMemory(byteArray, buff);
					}
				}else{
					//// 通信失敗
					//var message = "failed";
					//size = message.length + 1;
					//buff = Module._malloc(size);
					//Module.writeStringToMemory(message, buff);
				}
				Module.nativePluginUtilLoadingDecrement();
				ccall("gamePluginUtilCallbackCall", null, [null, null, null], [callbackId, buff, size]);
			}
		};
		xhr.send();
	},

	// ----------------------------------------------------------------

	// 設定読み込み 返値文字列は揮発性バッファで解放禁止
	platformPluginDataPreferenceGet: function(key){
		var str = localStorage.getItem(Pointer_stringify(key));
		if(str == null){return null;}
		var buff = ccall("corePluginUtilTemporaryBuffer", null, [null], [str.length + 1]);
		Module.writeStringToMemory(str, buff);
		return buff;
	},

	// 設定書き込み
	platformPluginDataPreferenceSet: function(key, value){
		localStorage.setItem(Pointer_stringify(key), Pointer_stringify(value));
	},

	// 設定保存
	platformPluginDataPreferenceCommit: function(){
		// webでは何もしない 
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

