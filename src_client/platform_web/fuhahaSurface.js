
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// ゲーム初期化完了後のプラットフォーム初期化
	webFuhahaSurfaceInit: function(){
		this.globalWebFuhahaSurface = {};

		globalWebFuhahaSurface.isExit = false;

		// 画面サイズ変更処理初期化
		var resizeCanvas = function(){
			if(globalWebFuhahaSurface && globalWebFuhahaSurface.isExit){return;}
			var w = window.innerWidth;
			var h = window.innerHeight;
			var pixelRatio = window.devicePixelRatio;
			Module.setCanvasSize(w * pixelRatio, h * pixelRatio);
			Module.canvas.width = w * pixelRatio;
			Module.canvas.height = h * pixelRatio;
			Module.canvas.style.width = w + "px";
			Module.canvas.style.height = h + "px";
			Module.canvas.style.marginLeft = (w * -0.5) + "px";
			Module.canvas.style.marginTop = (h * -0.5) + "px";
			ccall("gameSurfaceChanged", null, [null, null, null], [w, h, pixelRatio]);
		};
		var resizeTimer;
		window.addEventListener("resize", function(e){
			clearTimeout(resizeTimer);
			resizeTimer = setTimeout(resizeCanvas, 300);
		});
		resizeCanvas();
	},

	// ----------------------------------------------------------------

	// アプリ終了命令
	platformSurfaceExit: function(){
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

