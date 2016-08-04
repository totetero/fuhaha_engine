
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// ゲーム初期化開始時のプラットフォーム初期化
	webFuhahaSurfaceInit: function(){
		this.globalWebFuhahaSurface = {};
		globalWebFuhahaSurface.isExit = false;
	},

	// ----------------------------------------------------------------

	// アプリ終了命令
	platformMainSurfaceExit: function(){
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

