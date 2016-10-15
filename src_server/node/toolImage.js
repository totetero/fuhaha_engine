
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

var libFs = require("fs");
var libPath = require("path");
var fuhahaUtil = require("./util");
var fuhahaFileServer = require("./fileServer");

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像ツールサーバ
module.exports.server = function(req, res){
	var rootPath = "/tool_image";
	if(req.url.indexOf(rootPath) != 0){return false;}
	
	if(req.url == rootPath){
		// 画像ツールhtml
		var filePath = libPath.join(process.cwd(), "/src_data/tool_image/tool/index.html");
		fuhahaFileServer.showFile(req, res, filePath);
	}else if(req.url == rootPath + "/save_text"){
		// 書式確認
		if(!fuhahaUtil.checkType("Object", req.bodyJson)){fuhahaUtil.showErr500(res, "validateError json"); return;}
		if(!fuhahaUtil.checkType("String", req.bodyJson.path)){fuhahaUtil.showErr500(res, "validateError json['path']"); return;}
		if(!fuhahaUtil.checkType("String", req.bodyJson.data)){fuhahaUtil.showErr500(res, "validateError json['data']"); return;}
		// 情報保存
		var savePath = "src_data/tool_image/header/" + req.bodyJson.path;
		var saveData = req.bodyJson.data;
		libFs.writeFile(savePath, saveData, function(err){
			if(err){fuhahaUtil.showErr500(res, err); return;}

			res.write(JSON.stringify({"success": true}));
			res.end();
		});

	}else if(req.url == rootPath + "/save_image"){
		// 書式確認
		if(!fuhahaUtil.checkType("Object", req.bodyJson)){fuhahaUtil.showErr500(res, "validateError json"); return;}
		if(!fuhahaUtil.checkType("String", req.bodyJson.path)){fuhahaUtil.showErr500(res, "validateError json['path']"); return;}
		if(!fuhahaUtil.checkType("String", req.bodyJson.data)){fuhahaUtil.showErr500(res, "validateError json['data']"); return;}
		// 画像保存
		var savePath = "contents/img/" + req.bodyJson.path;
		var saveData = req.bodyJson.data.replace(/^data:image\/png;base64,/, "");
		libFs.writeFile(savePath, saveData, "base64", function(err){
			if(err){fuhahaUtil.showErr500(res, err); return;}

			res.write(JSON.stringify({"success": true}));
			res.end();
		});
	}else{
		// ファイルサーバ
		var filePaths = [];
		filePaths.push(libPath.join(process.cwd(), "/src_data/tool_image/image" + req.url.substring(rootPath.length)));
		filePaths.push(libPath.join(process.cwd(), "/src_data/tool_image/tool" + req.url.substring(rootPath.length)));
		fuhahaFileServer.fileServer(req, res, filePaths);
	}
	
	return true;
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

