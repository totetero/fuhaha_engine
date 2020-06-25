<!--

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

var libFs = require("fs");
var libPath = require("path");
var libHttp = require("http");
var libHttps = require("https");
var fuhahaFileServer = require("../node/fileServer");

const isHttps = true;
const port = 8080;
(isHttps ? libHttps.createServer({
	pfx: libFs.readFileSync("src_server/test/mysslserver.pfx"),
	passphrase: "test"
}) : libHttp.createServer()).on("request", (req, res) => {
	var filePaths = [];
	filePaths.push(libPath.join(process.cwd(), "/src_client/platform_web/bin" + req.url));
	fuhahaFileServer.fileServer(req, res, filePaths);
}).listen(port, () => console.log(`Server ${isHttps ? "https" : "http"}://localhost:${port}`));

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
