
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// プラグイン初期化
	webPluginTextureInit: function(){
		this.globalWebPluginTexture = {};

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
	},

	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	platformPluginTextureLocal: function(param, src, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginTextureLocalCallbackSet", null, [null, null], [param, callback]);
		var gl = Module.ctx;
		var image = new Image();
		image.onload = function(){
			var texture = GLctx.createTexture();
			var glId = GL.getNewId(GL.textures);
			texture.name = glId;
			GL.textures[glId] = texture;
			ccall("corePluginTextureIsBind", null, [null], [glId]);
			gl.bindTexture(gl.TEXTURE_2D, GL.textures[glId]);
			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
			gl.generateMipmap(gl.TEXTURE_2D);
			Module.nativePluginUtilLoadingDecrement();
			ccall("gamePluginTextureLocalCallbackCall", null, [null, null, null, null], [callbackId, glId, image.width, image.height]);
		};
		image.src = Pointer_stringify(src);
	},

	// ----------------------------------------------------------------

	// フォントテクスチャ作成
	platformPluginTextureFont: function(param, fontSetId, letterList, letterLenght, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginTextureFontCallbackSet", null, [null, null], [param, callback]);
		var gl = Module.ctx;
		var strLetterList = Pointer_stringify(letterList);
		var objLetterList = {};
		var margin = 2;
		var lineWidth = margin;
		var lineHeight = margin;
		var maxWidth = margin;
		var maxHeight = margin;
		var limitWidth = (1 << 10);
		var limitHeight = (1 << 10);
		// 文字サイズの計測
		globalWebPluginTexture.measureContext.font = globalWebPluginTexture.getFontSet(fontSetId);
		for(var i = 0; i < letterLenght; i++){
			var objLetter = objLetterList[i] = {};
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
			for(var i = 0; i < letterLenght; i++){
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
			var texture = GLctx.createTexture();
			var glId = GL.getNewId(GL.textures);
			texture.name = glId;
			GL.textures[glId] = texture;
			ccall("corePluginTextureIsBind", null, [null], [glId]);
			gl.bindTexture(gl.TEXTURE_2D, GL.textures[glId]);
			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, canvas);
			gl.generateMipmap(gl.TEXTURE_2D);
			// ネイティブデータ作成
			var codeList = ccall("gamePluginTextureFontCodeListCreate", null, [null], [letterLenght]);
			for(var i = 0; i < letterLenght; i++){
				var objLetter = objLetterList[i];
				ccall("gamePluginTextureFontCodeListSet", null, [null, null, null, null, null, null, null], [codeList, i, objLetter.code, objLetter.x, objLetter.y, objLetter.w, objLetter.h]);
			}
			Module.nativePluginUtilLoadingDecrement();
			ccall("gamePluginTextureFontCallbackCall", null, [null, null, null, null, null], [callbackId, glId, canvas.width, canvas.Height, codeList]);
		}else{
			// 必要領域が大きすぎる
			// TODO
		}
	},

	// フォントテクスチャ破棄
	platformPluginTextureFontDispose: function(glId, codeList){
		// テクスチャ除去
		var texture = GL.textures[glId];
		GLctx.deleteTexture(texture);
		texture.name = 0;
		GL.textures[glId] = null;
		// ネイティブデータ破棄
		ccall("gamePluginTextureFontCodeListDispose", null, [null], [codeList]);
	}

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

