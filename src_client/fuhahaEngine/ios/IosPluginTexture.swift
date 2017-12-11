import Foundation
import UIKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginTexture: NSObject{
	static var fontList: [Int32: IosPluginTexture.FontData]? = [:];

	// ----------------------------------------------------------------

	// テクスチャ作成
	static internal func createTextureBuff(_ imageRef: CGImage, _ width: Int, _ height: Int) -> [GLubyte]{
		var buff: [GLubyte] = [GLubyte](repeating: 0, count: width * height * 4);
		let colorSpace: CGColorSpace = CGColorSpaceCreateDeviceRGB();
		let bitmapInfo: CGBitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue);
		let context: CGContext = CGContext(data: &buff, width: width, height: height, bitsPerComponent: 8, bytesPerRow: width * 4, space: colorSpace, bitmapInfo: bitmapInfo.rawValue)!;
		context.draw(imageRef, in: CGRect(x: 0, y: 0, width: CGFloat(width), height: CGFloat(height)));
		return buff;
	}

	// テクスチャ作成
	static internal func createTextureGlId(_ buff: [GLubyte], _ width: Int, _ height: Int) -> GLuint{
		var glId: GLuint = 0;
		glGenTextures(GLsizei(1), &glId);
		corePluginTextureIsBind(glId);
		glBindTexture(GLenum(GL_TEXTURE_2D), glId);
		glTexImage2D(GLenum(GL_TEXTURE_2D), GLint(0), GL_RGBA, GLsizei(width), GLsizei(height), 0, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE), buff);
		glGenerateMipmap(GLenum(GL_TEXTURE_2D));
		return glId;
	}

	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	static internal func platformPluginTextureLocal(_ callbackId: Int32, src: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		DispatchQueue(label: "fuhahaCreateTextureLocal").async(execute: {
			// データロード
			let path: String = Bundle.main.path(forResource: src, ofType: nil, inDirectory: "assets")!;
			let image: UIImage = UIImage(contentsOfFile: path)!;

			// テクスチャ作成
			let imageRef: CGImage = image.cgImage!;
			let width: Int = imageRef.width;
			let height: Int = imageRef.height;
			let buff: [GLubyte] = IosPluginTexture.createTextureBuff(imageRef, width, height);
			DispatchQueue.main.async(execute: {
				let glId: GLuint = IosPluginTexture.createTextureGlId(buff, width, height);

				// コールバック
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginTextureLocalCallbackCall(callbackId, Int32(glId), Int32(width), Int32(height));
			});
		});
	}

	// ----------------------------------------------------------------

	// フォント文字列作成
	static internal func getFontSet(_ fontSetId: Int32) -> UIFont{
		let fontSize: CGFloat = self.getFontSize(fontSetId);
		return UIFont(name: "HelveticaNeue-Thin", size: fontSize)!;
	}

	// フォント文字列作成
	static internal func getFontSize(_ fontSetId: Int32) -> CGFloat{
		return 32;
	}

	// フォントテクスチャ作成
	static internal func platformPluginTextureFont(_ callbackId: Int32, fontSetId: Int32, letterList: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		// フォント準備
		let font: UIFont = IosPluginTexture.getFontSet(fontSetId);

		let letterLength: Int32 = Int32(letterList.characters.count);
		let strLetterList: [String] = letterList.characters.map{(c: Character) -> String in return String(c);}
		var objLetterList: [IosPluginTexture.ObjLetter?] = Array<IosPluginTexture.ObjLetter?>(repeating: nil, count: Int(letterLength));

		// 文字サイズの計測
		let margin: Double = 2;
		var lineWidth: Double = margin;
		var lineHeight: Double = margin;
		var maxWidth: Double = margin;
		var maxHeight: Double = margin;
		let limitWidth: Int = (1 << 10);
		let limitHeight: Int = (1 << 10);
		for i: Int in (0 ..< Int(letterLength)){
			objLetterList[i] = IosPluginTexture.ObjLetter();
			let objLetter: IosPluginTexture.ObjLetter = objLetterList[i]!;
			let size: CGSize = strLetterList[i].size(attributes: [NSFontAttributeName : font]);
			objLetter.code = Int32(strLetterList[i].unicodeScalars.first!.value);
			objLetter.w = Double(size.width);
			objLetter.h = Double(size.height);
			let marginTexw: Double = objLetter.w + margin;
			let marginTexh: Double = objLetter.h + margin;
			if(lineWidth + marginTexw <= Double(limitWidth)){
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

		if(maxWidth <= Double(limitWidth) && maxHeight <= Double(limitHeight)){
			// 2の累乗にする
			for i: Int in (0..<12){let size: Int = (1 << i); if(maxWidth <= Double(size)){maxWidth = Double(size); break;}}
			for i: Int in (0..<12){let size: Int = (1 << i); if(maxHeight <= Double(size)){maxHeight = Double(size); break;}}
			// 文字キャンバス作成
			UIGraphicsBeginImageContext(CGSize(width: Int(maxWidth), height: Int(maxHeight)));
			let attrs: [String: NSObject] = [NSFontAttributeName: font, NSForegroundColorAttributeName: UIColor.white, NSParagraphStyleAttributeName: NSMutableParagraphStyle()];
			lineWidth = margin;
			lineHeight = margin;
			maxHeight = margin;
			// 文字キャンバス描画
			for i: Int in (0 ..< Int(letterLength)){
				let objLetter: IosPluginTexture.ObjLetter = objLetterList[i]!;
				let marginTexw: Double = objLetter.w + margin;
				let marginTexh: Double = objLetter.h + margin;
				if(lineWidth + marginTexw <= Double(limitWidth)){
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
				strLetterList[i].draw(with: CGRect(x: Int(objLetter.x), y: Int(objLetter.y), width: Int(objLetter.w), height: Int(objLetter.h)), options: .usesLineFragmentOrigin, attributes: attrs, context: nil);
			}
			let image: UIImage = UIGraphicsGetImageFromCurrentImageContext()!;
			UIGraphicsEndImageContext();

			// テクスチャ作成
			let imageRef: CGImage = image.cgImage!;
			let width: Int = imageRef.width;
			let height: Int = imageRef.height;
			let buff: [GLubyte] = IosPluginTexture.createTextureBuff(imageRef, width, height);
			DispatchQueue.main.async(execute: {
				let glId: GLuint = IosPluginTexture.createTextureGlId(buff, width, height);

				// ネイティブデータ作成
				let codeListIndex: Int32 = gamePluginTextureFontCodeListCreate(letterLength);
				for i: Int in (0 ..< Int(letterLength)){
					let objLetter: IosPluginTexture.ObjLetter = objLetterList[i]!;
					gamePluginTextureFontCodeListSet(codeListIndex, Int32(i), pluginTextureFontSetId(UInt32(fontSetId)), objLetter.code, Int32(glId), Int32(width), Int32(height), objLetter.x, objLetter.y, objLetter.w, objLetter.h);
				}

				// フォントデータ保持
				let fontData: IosPluginTexture.FontData = IosPluginTexture.FontData();
				fontData.glId = Int32(glId);
				IosPluginTexture.fontList!.updateValue(fontData, forKey: codeListIndex);

				// コールバック
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginTextureFontCallbackCall(callbackId, codeListIndex, letterLength);
			});
		}else{
			// 必要領域が大きすぎる
			// TODO
		}
	}

	// フォントテクスチャ破棄
	static internal func platformPluginTextureFontDispose(_ codeListIndex: Int32){
		let fontData: IosPluginTexture.FontData? = IosPluginTexture.fontList![codeListIndex];
		if(fontData == nil){return;}
		var glId: GLuint = GLuint(fontData!.glId);
		IosPluginTexture.fontList!.removeValue(forKey: codeListIndex)
		// テクスチャ除去
		glDeleteTextures(GLsizei(1), &glId);
		// ネイティブデータ破棄
		gamePluginTextureFontCodeListDispose(codeListIndex);
	}

	// 文字データ
	class ObjLetter: NSObject{
		var code: Int32 = -1;
		var x: Double = 0;
		var y: Double = 0;
		var w: Double = 0;
		var h: Double = 0;
	}

	// 保持フォントデータ
	class FontData: NSObject{
		var glId: Int32 = 0;
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

