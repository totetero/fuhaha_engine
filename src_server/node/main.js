
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

var fs = require("fs");
var url = require("url");
var path = require("path");
var http = require("http");

var header = {
	"Access-Control-Allow-Origin":"*",
	"Pragma": "no-cache",
	"Cache-Control" : "no-cache"       
};

var err404 = function(res){
	res.writeHead(404, {"Content-Type": "text/plain"});
	res.write("404 Not Found");
	res.end();
};

var err500 = function(res, err){
	res.writeHead(500, {"Content-Type": "text/plain"});
	res.write("500 " + err);
	res.end();
};

http.createServer(function(req, res){
	var uri = url.parse(req.url).pathname;
	var filename = path.join(process.cwd(), "/src_platform/web/bin" + uri);

	fs.exists(filename, function(exist){
		if(!exist){err404(res); return;}

		fs.stat(filename, function(err, stats){
			if(err){err500(res, err); return;}

			if(stats.isDirectory()){
				fs.readdir(filename, function(err, files){
					if(err){err500(res, err); return;}

					res.writeHead(200, header);
					res.write("<!DOCTYPE html><html><head>");
					res.write("</head><body>");
					for(var i = 0; i < files.length; i++){
						res.write("<a href=" + path.join(uri, files[i]) + ">" + files[i] + "</a><br>");
					}
					res.write("</body><html>");
					res.end();
				});
			}else{
				fs.readFile(filename, "binary", function(err, file){
					if(err){err500(res, err); return;}

					res.writeHead(200, header);
					res.write(file, "binary");
					setTimeout(function(){res.end();}, 0); // 遅延処理
				});
			}
		});
	});
}).listen(8080);

console.log("start simple static server");

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

