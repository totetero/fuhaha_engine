
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// ゲーム初期化開始時のプラットフォーム初期化
	webFuhahaSurfaceInit: function(){
		this.globalWebFuhahaSurface = {};
		// 全画面ボタン表示
		var isAcceleration = ccall("gameMainEventIsAcceleration", "null", [], []);
		if(isAcceleration){
			Module['canvas'].parentElement.insertBefore((function(){
				var div = document.createElement("div");
				div.style.position = "absolute";
				div.style.left = "0px";
				div.style.right = "0px";
				div.style.top = "0px";
				div.style.bottom = "0px";
				div.style.backgroundColor = "rgba(0, 0, 0, 0.5)";
				div.appendChild((function(){
					var div = document.createElement("div");
					div.style.position = "absolute";
					div.style.left = "50%";
					div.style.top = "50%";
					div.style.transform = "translate(-50%, -50%)";
					div.style.color = "white";
					div.innerHTML = "tap to start";
					return div;
				})());
				div.addEventListener("click", function(){
					// タッチしないと音を再生開始しない端末のための無音再生
					if(globalWebPluginSound && globalWebPluginSound.soundTouch){globalWebPluginSound.soundTouch();}
					// タッチしないと傾きを検知できない端末のための権限請求
					if(globalWebFuhahaSurface && globalWebFuhahaSurface.deviceMotionTouch){globalWebFuhahaSurface.deviceMotionTouch();}
					// 全画面ボタンの削除
					div.parentNode.removeChild(div);
				});
				return div;
			})(), null);
		}
		// 起動フラグ操作
		globalWebFuhahaSurface.isExit = false;
	},

	// ----------------------------------------------------------------

	// アプリ終了命令
	platformMainSurfaceExit: function(){
		// 起動フラグ操作
		globalWebFuhahaSurface.isExit = true;
		// 終了幕表示
		Module['canvas'].parentElement.insertBefore((function(){
			var div = document.createElement("div");
			div.style.position = "absolute";
			div.style.left = "0px";
			div.style.right = "0px";
			div.style.top = "0px";
			div.style.bottom = "0px";
			div.style.backgroundColor = "black";
			div.appendChild((function(){
				var div = document.createElement("div");
				div.style.position = "absolute";
				div.style.left = "50%";
				div.style.top = "50%";
				div.style.transform = "translate(-50%, -50%)";
				div.style.color = "white";
				div.innerHTML = "(・∀・)<br>bye-bye";
				return div;
			})());
			return div;
		})(), null);
		// 終了処理
		Module['noExitRuntime'] = false;
		Module['exit'](1);
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

