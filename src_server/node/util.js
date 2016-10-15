
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// シーケンス処理
module.exports.sequence = function(){
	var funcList = [];
	var nextFunc = function(index, value){
		// シーケンス遷移
		if(funcList.length > index){
			// 次のシーケンスが存在する
			var func = funcList[index];
			funcList[index] = null;
			if(func != null){
				// 次のシーケンスを呼び出す
				var next = function(value){nextFunc(index + 1, value);};
				func(next, value);
			}else{
				// エラー 多重処理
			}
		}else{
			// シーケンス終了
		}
	};
	var seq = {};
	seq.add = function(func){funcList.push(func); return seq;};
	seq.exec = function(value){nextFunc(0, value);};
	return seq;
};

// 型確認
module.exports.checkType = function(type, obj){return (Object.prototype.toString.call(obj).slice(8, -1) === type);};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ヘッダ設定
module.exports.header = {
	"Access-Control-Allow-Origin":"*",
	"Pragma": "no-cache",
	"Cache-Control" : "no-cache"       
};

// 404エラー表示
module.exports.showErr404 = function(res){
	res.writeHead(404, {"Content-Type": "application/json"});
	res.write(JSON.stringify({code: 404, message: "404 Not Found"}));
	res.end();
};

// 500エラー表示
module.exports.showErr500 = function(res, err){
	res.writeHead(500, {"Content-Type": "application/json"});
	res.write(JSON.stringify({code: 500, message: err}));
	res.end();
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

