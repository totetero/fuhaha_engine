
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// ゲーム初期化完了後のプラットフォーム初期化
	webPlatformScreenInit: function(){
		// 画面サイズ変更処理初期化
		var resizeCanvas = function(){
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
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

