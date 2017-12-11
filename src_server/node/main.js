
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

var libPath = require("path");
var libHttp = require("http");
var fuhahaUtil = require("./util");
var fuhahaToolImage = require("./toolImage");
var fuhahaFileServer = require("./fileServer");

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// サーバメイン処理
libHttp.createServer(function(req, res){
	fuhahaUtil.sequence().add(function(next, value){
		// POSTのjsonデータが存在すれば取得
		if(req.method == "POST" && req.headers["content-type"] == "application/json"){
			var dataArray = [];
			req.on("data", function(chunk){dataArray.push(chunk);});
			req.on("end", function(){
				req.bodyJson = JSON.parse(Buffer.concat(dataArray).toString("utf-8"));
				next(null);
			});
		}else{
			req.bodyJson = null;
			next(null);
		}
	}).add(function(next, value){
		// サーバ処理
		if(fuhahaToolImage.server(req, res)){
		}else{
			// ファイルサーバ
			var filePaths = [];
			filePaths.push(libPath.join(process.cwd(), "/src_client/platform_web/bin" + req.url));
			fuhahaFileServer.fileServer(req, res, filePaths);
		}
		next(null);
	}).exec(null);
}).listen(8080);

console.log("start test server");

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

