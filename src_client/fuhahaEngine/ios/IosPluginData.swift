import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginData: NSObject{
	// ----------------------------------------------------------------

	// HTTP通信 
	@objc static internal func platformPluginDataHttp(_ callbackId: Int32, url: String, request: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		// 通信準備
		let baseUrl: String = String(cString: UnsafePointer<CChar>(gamePluginUtilUrlGet()));
		let nsurl: URL = URL(string: baseUrl + "/" + url)!;
		let nsreq: NSMutableURLRequest = NSMutableURLRequest(url: nsurl);
		nsreq.httpMethod = "POST";
		nsreq.httpBody = request.data(using: String.Encoding.utf8);
		nsreq.addValue("application/octet-stream", forHTTPHeaderField: "Content-Type");
		let task = URLSession.shared.dataTask(with: nsreq as URLRequest, completionHandler: {(nsdat: Data?, nsres: URLResponse?, error: Error?) in
			var resData: Data? = nsdat;
			// エラー確認
			let statusCode: Int? = (nsres as? HTTPURLResponse)?.statusCode;
			if(statusCode != 200){resData = nil;}
			//if(resData == nil){resData = "failed".dataUsingEncoding(NSUTF8StringEncoding);}
	
			// データ処理
			var size: Int = 0;
			var buff: UnsafeMutableRawPointer? = nil;
			if(resData != nil){
				let info = UnsafeMutablePointer<Int8>(mutating: "pluginData".cString(using: String.Encoding.utf8));
				size = resData!.count;
				buff = gamePluginUtilMemoryMalloc(info, size); // バッファの解放はゲーム側で行う
				(resData! as NSData).getBytes(buff!, length: size);
			}
			DispatchQueue.main.async(execute: {
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginDataHttpCallbackCall(Int32(callbackId), buff, size);
			});
		});
		task.resume();
	}

	// ローカルデータ読み込み
	@objc static internal func platformPluginDataLocal(_ callbackId: Int32, src: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		DispatchQueue(label: "fuhahaLoadLocalData").async(execute: {
			let path: String = Bundle.main.path(forResource: src, ofType: nil, inDirectory: "assets")!;
			let resData: Data? = try? Data(contentsOf: URL(fileURLWithPath: path));
			//if(resData == nil){resData = "failed".dataUsingEncoding(NSUTF8StringEncoding);}

			// データ処理
			var size: Int = 0;
			var buff: UnsafeMutableRawPointer? = nil;
			if(resData != nil){
				let info = UnsafeMutablePointer<Int8>(mutating: "pluginData".cString(using: String.Encoding.utf8));
				size = resData!.count;
				buff = gamePluginUtilMemoryMalloc(info, size); // バッファの解放はゲーム側で行う
				(resData! as NSData).getBytes(buff!, length: size);
			}
			DispatchQueue.main.async(execute: {
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginDataLocalCallbackCall(Int32(callbackId), buff, size);
			});
		});
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

