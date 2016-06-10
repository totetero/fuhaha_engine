
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// ゲーム初期化完了後のプラットフォーム初期化
	webFuhahaCtrlInit: function(){
		// ----------------------------------------------------------------
		// タッチ入力イベント処理初期化

		var ua = window.navigator.userAgent;
		var isIos = false;
		if(ua.indexOf('iPhone') > 0){isIos = true;}
		if(ua.indexOf('iPad') > 0){isIos = true;}
		if(ua.indexOf('iPod') > 0){isIos = true;}

		var isTouch = ('ontouchstart' in window);
		// タッチ1状態
		var t1x = 0;
		var t1y = 0;
		var t1dn = false;
		var t1id = -1;
		// タッチ2状態
		var t2x = 0;
		var t2y = 0;
		var t2dn = false;
		var t2id = -1;

		function onTouch(){
			ccall("gameEvenTouch", "null", ["null", "null", "null", "null"], [0, t1x, t1y, t1dn]);
			ccall("gameEvenTouch", "null", ["null", "null", "null", "null"], [1, t2x, t2y, t2dn]);
		}

		// タッチ開始関数
		function tdnfn(e){
			var rect = Module.canvas.getBoundingClientRect();
			if(isTouch){
				for(var i = 0; i < e.changedTouches.length; i++){
					var tinfo = e.changedTouches[i];
					if(t1id < 0 && t1id != tinfo.identifier && t2id != tinfo.identifier){
						t1id = tinfo.identifier;
						t1x = tinfo.clientX - rect.left;
						t1y = tinfo.clientY - rect.top;
						t1dn = true;
					}
					if(t2id < 0 && t1id != tinfo.identifier && t2id != tinfo.identifier){
						t2id = tinfo.identifier;
						t2x = tinfo.clientX - rect.left;
						t2y = tinfo.clientY - rect.top;
						t2dn = true;
					}
				}
			}else{
				t1x = e.clientX - rect.left;
				t1y = e.clientY - rect.top;
				t1dn = true;
			}
			onTouch();
			e.preventDefault();
			e.stopPropagation();

			// タッチしないと音を再生開始しない端末のための無音再生
			if(platformSound && platformSound.soundTouch){platformSound.soundTouch();}
		}

		// タッチ移動関数
		function tmvfn(e){
			var rect = Module.canvas.getBoundingClientRect();
			if(isTouch){
				for(var i = 0; i < e.changedTouches.length; i++){
					var tinfo = e.changedTouches[i];
					if(t1id == tinfo.identifier){
						t1x = tinfo.clientX - rect.left;
						t1y = tinfo.clientY - rect.top;
					}
					if(t2id == tinfo.identifier){
						t2x = tinfo.clientX - rect.left;
						t2y = tinfo.clientY - rect.top;
					}
				}
			}else{
				t1x = e.clientX - rect.left;
				t1y = e.clientY - rect.top;
			}
			onTouch();
			e.preventDefault();
			e.stopPropagation();
		}

		// タッチ完了関数
		function tupfn(e){
			var rect = Module.canvas.getBoundingClientRect();
			if(isTouch){
				for(var i = 0; i < e.changedTouches.length; i++){
					var tinfo = e.changedTouches[i];
					if(t1id == tinfo.identifier){
						t1id = -1;
						t1x = tinfo.clientX - rect.left;
						t1y = tinfo.clientY - rect.top;
						t1dn = false;
					}
					if(t2id == tinfo.identifier){
						t2id = -1;
						t2x = tinfo.clientX - rect.left;
						t2y = tinfo.clientY - rect.top;
						t2dn = false;
					}
				}
			}else{
				t1x = e.clientX - rect.left;
				t1y = e.clientY - rect.top;
				t1dn = false;
			}
			onTouch();
			e.preventDefault();
			e.stopPropagation();
		}

		// タッチイベント設定
		if(isTouch){
			Module.canvas.addEventListener("touchstart", tdnfn);
			Module.canvas.addEventListener("touchmove", tmvfn);
			Module.canvas.addEventListener("touchend", tupfn);
			Module.canvas.addEventListener("touchcancel", tupfn);
		}else{
			Module.canvas.addEventListener("mousedown", tdnfn);
			Module.canvas.addEventListener("mousemove", tmvfn);
			Module.canvas.addEventListener("mouseup", tupfn);
			Module.canvas.addEventListener("mouseout", function(e){
				var x = e.clientX;
				var y = e.clientY;
				var w = window.innerWidth;
				var h = window.innerHeight;
				if(x <= 0 || w <= x || y <= 0 || h <= y){tupfn(e);}
			});
		}
		document.oncontextmenu = function(){return false;}

		// ----------------------------------------------------------------
		// キー入力イベント処理初期化

		// キー押下状態
		var kup = false;
		var kdn = false;
		var krt = false;
		var klt = false;
		var kzb = false;
		var kxb = false;
		var kcb = false;
		var ksb = false;

		function onKey(){ccall("gameEventKey", "null", ["null", "null", "null", "null", "null", "null", "null", "null"], [kup, kdn, krt, klt, kzb, kxb, kcb, ksb]);}

		// キーを押し込む
		document.addEventListener("keydown", function(e){
			switch(e.keyCode){
				case 37: klt = true; break;
				case 38: kup = true; break;
				case 39: krt = true; break;
				case 40: kdn = true; break;
				case 88: kxb = true; break;
				case 90: kzb = true; break;
				case 67: kcb = true; break;
				case 32: ksb = true; break;
			}
			onKey();
			// キーイベント終了
			e.preventDefault();
			e.stopPropagation();
		});

		// キーを離す
		document.addEventListener("keyup", function(e){
			switch(e.keyCode){
				case 37: klt = false; break;
				case 38: kup = false; break;
				case 39: krt = false; break;
				case 40: kdn = false; break;
				case 88: kxb = false; break;
				case 90: kzb = false; break;
				case 67: kcb = false; break;
				case 32: ksb = false; break;
			}
			onKey();
			// キーイベント終了
			e.preventDefault();
			e.stopPropagation();
		});

		// ----------------------------------------------------------------
		// 加速度イベント

		window.addEventListener("devicemotion", function(e){
			var accx = e.accelerationIncludingGravity.x;
			var accy = e.accelerationIncludingGravity.y;
			var accz = e.accelerationIncludingGravity.z;
			if(isIos){accx *= -1; accy *= -1; accz *= -1;}
			ccall("gameEventAcceleration", "null", ["null", "null", "null"], [accx, accy, accz]);
		});

		// ----------------------------------------------------------------
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

