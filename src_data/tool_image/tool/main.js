
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

(function(jsonList){
	// ----------------------------------------------------------------
	// ユーティリティ

	var util = (function(){
		var util = {};
		util.canvas = null;
		util.context = null;
		util.update = false;
		util.target = null;
		return util;
	})();


	// ----------------------------------------------------------------
	// ローダー

	var loader = (function(){
		var loader = {};
		loader.json = [];
		loader.imgs = [];

		// サーバと通信する
		loader.send = function(url, json, callback){
			var xhr = new XMLHttpRequest();
			xhr.open((json == null ? "GET" : "POST"), url, true);
			xhr.setRequestHeader("Content-Type", "application/json");
			xhr.responseType = "json";
			xhr.onreadystatechange = function(e){
				if(xhr.readyState == 4){
					if(xhr.status == 200 && xhr.response != null){
						// 通信成功
						callback(xhr.response);
					}else{
						// 通信失敗
						console.error("sendPost error");
					}
				}
			};
			xhr.send(json == null ? null : JSON.stringify(json));
		};

		// 読み込まれていない画像の読み込み
		loader.loadImage = function(json){
			for(var i = 0; i < json.length; i++){
				var imgs = json[i]["imgs"];
				if(imgs == null){
					for(var j = 0; j < i; j++){
						if(json[i]["tag"] != json[j]["tag"]){continue;}
						imgs = json[j]["imgs"];
						break;
					}
				}
				if(imgs == null){continue;}
				for(var j = 0; j < imgs.length; j++){
					var prefixId = imgs[j]["prefixId"];
					var prefix = json[i]["prefixes"][prefixId];
					var src = prefix + imgs[j]["src"];
					if(loader.imgs[src] != null){continue;}
					var img = new Image();
					img.onload = function(){util.update = true;};
					img.src = "/tool_image/" + src;
					loader.imgs[src] = img;
				}
			}
		};

		// json書式確認
		var checkType = function(type, obj){return (Object.prototype.toString.call(obj).slice(8, -1) === type);};
		
		var validate = function(json){
			var countJson = 0;
			var isErrorJson = false;
			if(!checkType("Array", json)){setError("validateError json"); isErrorJson = true;}
			else{
				for(var i = 0; i < json.length; i++){
					// データの書式確認
					var isErrorData = false;
					var setErrorData = function(err){console.error(err); isErrorData = true;};
					if(!checkType("Object", json[i])){setErrorData("validateError json[" + i + "]");}
					else{
						var isDuplicate = false;
						if(!checkType("String", json[i]["dst"])){setErrorData("validateError json[" + i + "]['dst']");}
						if(!checkType("String", json[i]["tag"])){setErrorData("validateError json[" + i + "]['tag']");}
						else{for(var j = 0; j < i; j++){if(json[i]["tag"] == json[j]["tag"]){isDuplicate = true; break;}}} // 同一タグが既に存在するか確認
						if(!checkType("Object", json[i]["prefixes"])){setErrorData("validateError json[" + i + "]['prefixes']");}
						else{for(var j in json[i]["prefixes"]){if(!checkType("String", json[i]["prefixes"][j])){setErrorData("validateError json[" + i + "]['prefixes']['" + j + "']");}}}
						if(!isDuplicate){
							// 同一タグが存在しなければ書式確認を続ける
							if(!checkType("Number", json[i]["width"])){setErrorData("validateError json[" + i + "]['width']");}
							if(!checkType("Number", json[i]["height"])){setErrorData("validateError json[" + i + "]['height']");}
							if(!checkType("Array", json[i]["imgs"])){setErrorData("validateError json[" + i + "]['imgs']");}
							else{
								for(var j = 0; j < json[i]["imgs"].length; j++){
									if(!checkType("Object", json[i]["imgs"][j])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]");}
									if(!checkType("String", json[i]["imgs"][j]["tag"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['tag']");}
									if(!checkType("Number", json[i]["imgs"][j]["x"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['x']");}
									if(!checkType("Number", json[i]["imgs"][j]["y"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['y']");}
									if(json[i]["imgs"][j]["w"] != null && !checkType("Number", json[i]["imgs"][j]["w"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['w']");}
									if(json[i]["imgs"][j]["h"] != null && !checkType("Number", json[i]["imgs"][j]["h"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['h']");}
									if(json[i]["imgs"][j]["isTurn"] != null && !checkType("Boolean", json[i]["imgs"][j]["isTurn"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['isTurn']");}
									if(!checkType("String", json[i]["imgs"][j]["prefixId"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['prefixId']");}
									if(!checkType("String", json[i]["imgs"][j]["src"])){setErrorData("validateError json[" + i + "]['imgs'][" + j + "]['src']");}
								}
							}
						}
					}
					
					if(!isErrorData){
						// 正しいデータ
						json[i]["isExist"] = true;
						countJson++;
					}else{
						// エラーデータ
						json[i]["isExist"] = false;
						isErrorJson = true;
					}
				}
			}
			//return !isErrorJson; // エラーデータを一つも許さない場合
			return (countJson > 0); // 一つ以上の正しいデータがあれば良い場合
		};

		// json設定
		loader.setJson = function(json){
			if(!validate(json)){return;}
			loader.json = json;
			loader.loadImage(json);

			// セレクトボックス作成
			var selects = "";
			for(var i = 0; i < json.length; i++){if(json[i]["isExist"]){selects += "<option>" + json[i]["dst"] + "</option>";}}
			document.getElementById("domSelectDst").innerHTML = selects;
		};

		return loader;
	})();

	// ----------------------------------------------------------------

	// 計算処理
	var calc = function(){
		// DOM確認
		var domSelectDst = document.getElementById("domSelectDst");
		var selectedDst = domSelectDst.options[domSelectDst.selectedIndex].innerHTML;

		if(util.target == null || util.target.dst != selectedDst){
			for(var i = 0; i < loader.json.length; i++){
				if(loader.json[i]["dst"] != selectedDst){continue;}
				if(!loader.json[i]["isExist"]){break;}
				// データコピー
				util.target = {};
				util.target.dst = loader.json[i]["dst"];
				util.target.tag = loader.json[i]["tag"];
				util.target.prefixes = loader.json[i]["prefixes"];
				if(loader.json[i]["imgs"] != null){
					util.target.width = loader.json[i]["width"];
					util.target.height = loader.json[i]["height"];
					util.target.imgs = loader.json[i]["imgs"];
				}else{
					for(var j = 0; j < i; j++){
						if(loader.json[i]["tag"] != loader.json[j]["tag"]){continue;}
						util.target.width = loader.json[j]["width"];
						util.target.height = loader.json[j]["height"];
						util.target.imgs = loader.json[j]["imgs"];
						break;
					}
				}
				// オブジェクトのディープコピー
				util.target.prefixes = JSON.parse(JSON.stringify(util.target.prefixes));
				util.target.imgs = JSON.parse(JSON.stringify(util.target.imgs));
				break;
			}
		}
	};

	// 情報保存ボタン押下時
	document.getElementById("domButtonSaveHeader").addEventListener("click", function(){
		// 情報作成
		var data = "// image data\n";
		var w = util.target.width;
		var h = util.target.height;
		data += "#define TEXSIZ_" + util.target.tag + "_W " + w + "\n";
		data += "#define TEXSIZ_" + util.target.tag + "_H " + h + "\n";
		data += "#define TEXSIZ_" + util.target.tag + "_WH " + w + ", " + h + "\n";
		for(var i = 0; i < util.target.imgs.length; i++){
			// 同じタグ名を既に使っていたらスキップ
			var isSkip = false;
			for(var j = 0; j < i; j++){if(util.target.imgs[i]["tag"] == util.target.imgs[j]["tag"]){isSkip = true;}}
			if(isSkip){continue;}

			var tag = util.target.tag + "_" + util.target.imgs[i]["tag"];
			var prefixId = util.target.imgs[i]["prefixId"];
			var prefix = util.target.prefixes[prefixId];
			var src = prefix + util.target.imgs[i]["src"];
			var img = loader.imgs[src];
			var x = util.target.imgs[i]["x"];
			var y = util.target.imgs[i]["y"];
			var w = util.target.imgs[i]["w"] || img.width; 
			var h = util.target.imgs[i]["h"] || img.height;
			data += "#define TEXPOS_" + tag + "_X " + x + "\n";
			data += "#define TEXPOS_" + tag + "_Y " + y + "\n";
			data += "#define TEXPOS_" + tag + "_W " + w + "\n";
			data += "#define TEXPOS_" + tag + "_H " + h + "\n";
			data += "#define TEXPOS_" + tag + "_XYWH " + x + ", " + y + ", " + w + ", " + h + "\n";
		}
		// 情報送信
		loader.send("/tool_image/save_text", {
			"path": util.target.tag + ".h",
			"data": data
		}, function(json){
			console.log("callback success");
		});
	});

	// 画像保存ボタン押下時
	document.getElementById("domButtonSaveImage").addEventListener("click", function(){
		// 画像送信
		loader.send("/tool_image/save_image", {
			"path": util.target.dst,
			"data": util.canvas.toDataURL()
		}, function(json){
			console.log("callback success");
		});
	});

	// DOM切り替え時処理
	document.getElementById("domSelectDst").addEventListener("change", function(){util.update = true;});
	document.getElementById("domSelectScale").addEventListener("change", function(){util.update = true;});
	document.getElementById("domToggleBack").addEventListener("change", function(){util.update = true;});

	// ----------------------------------------------------------------

	// 描画処理
	var draw = function(){
		if(!util.update){return;}
		util.update = false;

		// DOM確認
		var domSelectScale = document.getElementById("domSelectScale");
		var domToggleBack = document.getElementById("domToggleBack");
		var selectedScale = parseFloat(domSelectScale.options[domSelectScale.selectedIndex].value);
		var isBack = domToggleBack.checked;

		// キャンバス作成
		if(util.canvas == null || util.canvas.width != util.target.width || util.canvas.height != util.target.height){
			// キャンバス作成
			var canvasDiv = document.getElementById("domCanvas");
			if(util.canvas != null){canvasDiv.removeChild(util.canvas);}
			util.canvas = document.createElement("canvas");
			util.canvas.width = util.target.width;
			util.canvas.height = util.target.height;
			util.canvas.style.border = "solid 2px black"
			util.context = util.canvas.getContext("2d");
			canvasDiv.appendChild(util.canvas);
		}
		// キャンバス設定
		util.canvas.style.width = Math.floor(selectedScale * util.target.width) + "px";
		util.canvas.style.height = Math.floor(selectedScale * util.target.height) + "px";

		// 画面クリア
		util.context.clearRect(0, 0, util.canvas.width, util.canvas.height);

		// 画像描画
		for(var i = 0; i < util.target.imgs.length; i++){
			//var tag = util.target.tag + "_" + util.target.imgs[j]["tag"];
			var prefixId = util.target.imgs[i]["prefixId"];
			var prefix = util.target.prefixes[prefixId];
			var src = prefix + util.target.imgs[i]["src"];
			var img = loader.imgs[src];
			if(!img.complete){continue;}
			var isTurn = util.target.imgs[i]["isTurn"];
			var x = util.target.imgs[i]["x"] + (isTurn ? img.height : 0);
			var y = util.target.imgs[i]["y"];
			var w = util.target.imgs[i]["w"] || img.width; 
			var h = util.target.imgs[i]["h"] || img.height;
			var r = (isTurn ? 90 : 0);
			util.context.save();
			util.context.translate(x, y);
			util.context.rotate(r / 180 * Math.PI);
			if(isBack){util.context.fillStyle = "black"; util.context.fillRect(0, 0, w, h);}
			util.context.drawImage(img, 0, 0, w, h);
			util.context.restore();
		}
	};

	// ----------------------------------------------------------------

	// メインループ
	var mainloop = function(){
		calc();
		draw();
		setTimeout(mainloop, 16);
	};

	// 読み込みが終わったら処理開始
	document.addEventListener("DOMContentLoaded", function(e){
		loader.setJson(jsonList);
		mainloop();
	});

	// ----------------------------------------------------------------
})(document.imageDataJsonList = document.imageDataJsonList || []);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

