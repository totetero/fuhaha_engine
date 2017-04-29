
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginTextureInit: function(){
		this.globalWebPluginTexture = {};

		// テクスチャ作成
		globalWebPluginTexture.createTexture = function(image){
			var gl = Module.ctx;
			var texture = GLctx.createTexture();
			var glId = GL.getNewId(GL.textures);
			texture.name = glId;
			GL.textures[glId] = texture;
			ccall("corePluginTextureIsBind", null, [null], [glId]);
			gl.bindTexture(gl.TEXTURE_2D, GL.textures[glId]);
			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
			gl.generateMipmap(gl.TEXTURE_2D);
			return glId;
		}

		// 文字サイズ計測キャンバス作成
		globalWebPluginTexture.measureCanvas = document.createElement("canvas");
		globalWebPluginTexture.measureCanvas.width = 10;
		globalWebPluginTexture.measureCanvas.height = 10;
		globalWebPluginTexture.measureContext = globalWebPluginTexture.measureCanvas.getContext("2d");

		// フォント文字列作成
		globalWebPluginTexture.getFontSet = function(fontSetId){
			var fontSize = globalWebPluginTexture.getFontSize(fontSetId);
			return "bolder " + fontSize + "px sans-serif";
		};

		// フォント文字列作成
		globalWebPluginTexture.getFontSize = function(fontSetId){
			return 32;
		};

		// 作成したフォントリスト
		globalWebPluginTexture.fontList = {};
	},

	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	platformPluginTextureLocal: function(param, src, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginTextureLocalCallbackSet", null, [null, null], [param, callback]);
		var image = new Image();
		image.onload = function(){
			var glId = globalWebPluginTexture.createTexture(image);
			Module.nativePluginUtilLoadingDecrement();
			ccall("gamePluginTextureLocalCallbackCall", null, [null, null, null, null], [callbackId, glId, image.width, image.height]);
		};
		image.src = Pointer_stringify(src);
	},

	// ----------------------------------------------------------------

	// フォントテクスチャ作成
	platformPluginTextureFont: function(param, fontSetId, letterList, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginTextureFontCallbackSet", null, [null, null], [param, callback]);

		// フォント準備
		globalWebPluginTexture.measureContext.font = globalWebPluginTexture.getFontSet(fontSetId);

		var strLetterList = Pointer_stringify(letterList);
		var objLetterList = {};
		var letterLength = strLetterList.length;

		// 文字サイズの計測
		var margin = 2;
		var lineWidth = margin;
		var lineHeight = margin;
		var maxWidth = margin;
		var maxHeight = margin;
		var limitWidth = (1 << 10);
		var limitHeight = (1 << 10);
		for(var i = 0; i < letterLength; i++){
			var objLetter = objLetterList[i] = {code: -1, x: 0, y: 0, w: 0, h: 0};
			objLetter.code = strLetterList.charCodeAt(i);
			objLetter.w = globalWebPluginTexture.measureContext.measureText(strLetterList[i]).width;
			objLetter.h = globalWebPluginTexture.getFontSize(fontSetId);
			var marginTexw = objLetter.w + margin;
			var marginTexh = objLetter.h + margin;
			if(lineWidth + marginTexw <= limitWidth){
				// 行に続ける
				lineWidth += marginTexw;
				if(lineHeight < marginTexh){lineHeight = marginTexh;}
			}else{
				// 改行
				if(maxWidth < lineWidth){maxWidth = lineWidth;}
				maxHeight += lineHeight;
				lineWidth = marginTexw;
				lineHeight = marginTexh;
			}
		}
		if(maxWidth < lineWidth){maxWidth = lineWidth;}
		maxHeight += lineHeight;

		if(maxWidth <= limitWidth && maxHeight <= limitHeight){
			// 2の累乗にする
			for(var i = 0; i < 12; i++){var size = (1 << i); if(maxWidth <= size){maxWidth = size; break;}}
			for(var i = 0; i < 12; i++){var size = (1 << i); if(maxHeight <= size){maxHeight = size; break;}}
			// 文字キャンバス作成
			var canvas = document.createElement("canvas");
			canvas.width = maxWidth;
			canvas.height = maxHeight;
			var context = canvas.getContext("2d");
			context.font = globalWebPluginTexture.getFontSet(fontSetId);
			context.textBaseline = "middle";
			context.fillStyle = "white";
			lineWidth = margin;
			lineHeight = margin;
			maxHeight = margin;
			// 文字キャンバス描画
			for(var i = 0; i < letterLength; i++){
				var objLetter = objLetterList[i];
				var marginTexw = objLetter.w + margin;
				var marginTexh = objLetter.h + margin;
				if(lineWidth + marginTexw <= limitWidth){
					// 行に続ける
					lineWidth += marginTexw;
					if(lineHeight < marginTexh){lineHeight = marginTexh;}
				}else{
					// 改行
					maxHeight += lineHeight;
					lineWidth = marginTexw;
					lineHeight = marginTexh;
				}
				// 文字描画
				objLetter.x = lineWidth - marginTexw;
				objLetter.y = maxHeight;
				context.fillText(strLetterList[i], objLetter.x, objLetter.y + objLetter.h * 0.5);
			}

			// テクスチャ作成
			var glId = globalWebPluginTexture.createTexture(canvas);

			// ネイティブデータ作成
			var codeListIndex = ccall("gamePluginTextureFontCodeListCreate", null, [null], [letterLength]);
			for(var i = 0; i < letterLength; i++){
				var objLetter = objLetterList[i];
				ccall("gamePluginTextureFontCodeListSet", null, [null, null, null, null, null, null, null, null, null, null, null], [codeListIndex, i, fontSetId, objLetter.code, glId, canvas.width, canvas.height, objLetter.x, objLetter.y, objLetter.w, objLetter.h]);
			}

			// フォントデータ保持
			var fontData = {};
			fontData.glId = glId;
			globalWebPluginTexture.fontList[codeListIndex] = fontData;

			// コールバック
			Module.nativePluginUtilLoadingDecrement();
			ccall("gamePluginTextureFontCallbackCall", null, [null, null, null], [callbackId, codeListIndex, letterLength]);
		}else{
			// 必要領域が大きすぎる
			// TODO
		}
	},

	// フォントテクスチャ破棄
	platformPluginTextureFontDispose: function(codeListIndex){
		var fontData = globalWebPluginTexture.fontList[codeListIndex];
		if(fontData == undefined){return;}
		var glId = fontData.glId;
		delete globalWebPluginTexture.fontList[codeListIndex];
		// テクスチャ除去
		var texture = GL.textures[glId];
		GLctx.deleteTexture(texture);
		texture.name = 0;
		GL.textures[glId] = null;
		// ネイティブデータ破棄
		ccall("gamePluginTextureFontCodeListDispose", null, [null], [codeListIndex]);
	}

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

