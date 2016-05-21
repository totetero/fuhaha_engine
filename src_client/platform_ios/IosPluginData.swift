import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginData: NSObject{
	// ----------------------------------------------------------------

	// HTTP通信 
	static internal func platformPluginDataHttp(callbackId: Int32, url: String, request: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		// 通信準備
		let baseUrl: String = String.fromCString(UnsafePointer<CChar>(gamePluginUtilUrlGet()))!;
		let nsurl: NSURL = NSURL(string: baseUrl + "/" + url)!;
		let nsreq: NSMutableURLRequest = NSMutableURLRequest(URL: nsurl);
		nsreq.HTTPMethod = "POST";
		nsreq.HTTPBody = request.dataUsingEncoding(NSUTF8StringEncoding);
		nsreq.addValue("application/octet-stream", forHTTPHeaderField: "Content-Type");
		let task = NSURLSession.sharedSession().dataTaskWithRequest(nsreq, completionHandler: {(nsdat: NSData?, nsres: NSURLResponse?, error: NSError?) in
			var resData: NSData? = nsdat;
			// エラー確認
			let statusCode: Int? = (nsres as? NSHTTPURLResponse)?.statusCode;
			if(statusCode != 200){resData = nil;}
			//if(resData == nil){resData = "failed".dataUsingEncoding(NSUTF8StringEncoding);}
	
			// データ処理
			var size: Int = 0;
			var buff: UnsafeMutablePointer<CChar> = nil;
			if(resData != nil){
				size = resData!.length;
				buff = UnsafeMutablePointer<CChar>.alloc(size)
				resData!.getBytes(buff, length: size);
			}
			dispatch_async(dispatch_get_main_queue(), {
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginDataCallbackCall(callbackId, buff, size);
			});
		});
		task.resume();
	}

	// ローカルデータ読み込み
	static internal func platformPluginDataLocal(callbackId: Int32, src: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), {
			let path: String = NSBundle.mainBundle().pathForResource(src, ofType: nil, inDirectory: "assets")!;
			let resData: NSData? = NSData(contentsOfFile: path);
			//if(resData == nil){resData = "failed".dataUsingEncoding(NSUTF8StringEncoding);}

			// データ処理
			var size: Int = 0;
			var buff: UnsafeMutablePointer<CChar> = nil;
			if(resData != nil){
				size = resData!.length;
				buff = UnsafeMutablePointer<CChar>.alloc(size)
				resData!.getBytes(buff, length: size);
			}
			dispatch_async(dispatch_get_main_queue(), {
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginDataCallbackCall(callbackId, buff, size);
			});
		});
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

