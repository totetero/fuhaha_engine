
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

var libFs = require("fs");
var libPath = require("path");
var fuhahaUtil = require("./util");
var fuhahaFileServer = module.exports;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ディレクトリ中身表示
module.exports.showIndex = function(req, res, directoryFiles, isRoot){
	var contentType = "text/html";
	res.writeHead(200, Object.assign({}, fuhahaUtil.header, {"Content-Type": contentType,}));
	res.write("<!DOCTYPE html><html><head>");
	res.write("</head><body>");

	res.write("<h1>test server</h1>");
	res.write("<hr>");

	res.write("<h2>file system</h2>");
	if(!isRoot){res.write("<a href=../>...</a></br>");}
	for(var i = 0; i < directoryFiles.length; i++){
		var name = directoryFiles[i].name + (directoryFiles[i].isDirectory ? "/" : "");
		res.write("<a href=" + libPath.join(req.url, name) + ">" + name + "</a><br>");
	}
	res.write("<hr>");

	if(isRoot){
		res.write("<h2>tools</h2>");
		res.write("<a href=/tool_image>image tool</a></br>");
		res.write("<hr>");
	}

	res.write("</body><html>");
	res.end();
};

// ファイル表示
module.exports.showFile = function(req, res, filePath){
	libFs.readFile(filePath, "binary", function(err, file){
		if(err){fuhahaUtil.showErr500(res, err); return;}

		var prefix = libPath.extname(filePath)
		var contentType = "text/plain";
		if(prefix === ".js"){contentType = "text/javascript";}
		if(prefix === ".css"){contentType = "text/css";}
		if(prefix === ".html"){contentType = "text/html";}
		if(prefix === ".png"){contentType = "image/png";}
		if(prefix === ".m4a"){contentType = "audio/aac";}
		if(prefix === ".ogg"){contentType = "audio/ogg";}
		if(prefix === ".json"){contentType = "application/json";}
		if(prefix === ".wasm"){contentType = "application/wasm";}
		res.writeHead(200, Object.assign({}, fuhahaUtil.header, {"Content-Type": contentType,}));
		res.write(file, "binary");

		 // 遅延処理
		var delay = 0.0;
		if(req.url.indexOf("/test.png") == 0){delay = 1.0;}
		setTimeout(function(){res.end();}, delay * 1000);
	});
};

// ファイルサーバ処理
module.exports.fileServer = function(req, res, filePaths){
	// ファイルサーバ起動
	var isDirectory = false;
	var directoryFiles = [];
	var checkFilePaths = function(filePathIndex){
		if(filePathIndex < filePaths.length){
			// ファイルパス存在確認
			var filePath = filePaths[filePathIndex];
			libFs.exists(filePath, function(exist){
				if(exist){
					// ファイルパスの種類確認
					libFs.stat(filePath, function(err, stats){
						if(err){fuhahaUtil.showErr500(res, err); return;}

						if(stats.isDirectory()){
							isDirectory = true;
							// ディレクトリ中身確認
							libFs.readdir(filePath, function(err, fileNames){
								if(err){fuhahaUtil.showErr500(res, err); return;}

								var checkFileNames = function(fileNameIndex){
									if(fileNameIndex < fileNames.length){
										// ディレクトリ中身追加
										libFs.stat(libPath.join(filePath, fileNames[fileNameIndex]), function(err, stats){
											if(err){fuhahaUtil.showErr500(res, err); return;}

											// 要素の更新もしくは追加を行う
											var isPush = true;
											var name = fileNames[fileNameIndex];
											var isDirectory = stats.isDirectory();
											for(var i = 0; i < directoryFiles.length; i++){
												var temp = directoryFiles[i];
												if(name == temp.name){
													temp.isDirectory = (temp.isDirectory && isDirectory);
													isPush = false;
													break;
												}
											}
											if(isPush){directoryFiles.push({"name": name, "isDirectory": isDirectory});}
											// 次のディレクトリ中身追加を行う
											checkFileNames(fileNameIndex + 1);
										});
									}else{
										// ディレクトリ中身追加を終えたので次のファイルパス存在確認へ行く
										checkFilePaths(filePathIndex + 1);
									}
								};
								// 再帰的なディレクトリ中身探索を開始する
								checkFileNames(0);
							});
						}else{
							// ファイルが見つかったので送信して終了
							fuhahaFileServer.showFile(req, res, filePath);
						}
					});
				}else{
					// ファイルパスが存在しなくても次のファイルパス存在確認へ行く
					checkFilePaths(filePathIndex + 1);
				}
			});
		}else{
			// 全てのファイルパス存在確認完了したので結果を送信して終了
			if(isDirectory){
				fuhahaFileServer.showIndex(req, res, directoryFiles.sort(function(a, b){ 
					var an = a.name.toLowerCase();
					var bn = b.name.toLowerCase();
					if(an < bn){return -1;}
					if(an > bn){return 1;}
					return 0;
				}), (req.url == "/"));
			}else{fuhahaUtil.showErr404(res);}
		}
	};
	// 再帰的なパスリスト確認を開始する
	checkFilePaths(0);
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

