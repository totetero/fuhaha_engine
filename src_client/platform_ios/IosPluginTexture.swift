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
	static internal func platformPluginTextureFont(_ callbackId: Int32, fontSetId: Int32, letterList: String, letterLenght: Int32){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		// フォント準備
		let font: UIFont = IosPluginTexture.getFontSet(fontSetId);

		// 文字サイズの計測
		let strLetterList: [String] = letterList.characters.map{(c: Character) -> String in return String(c);}
		var objLetterList: [IosPluginTexture.ObjLetter?] = Array<IosPluginTexture.ObjLetter?>(repeating: nil, count: Int(letterLenght));
		let margin: Float = 2;
		var lineWidth: Float = margin;
		var lineHeight: Float = margin;
		var maxWidth: Float = margin;
		var maxHeight: Float = margin;
		let limitWidth: Int = (1 << 10);
		let limitHeight: Int = (1 << 10);
		for i: Int in (0 ..< Int(letterLenght)){
			objLetterList[i] = IosPluginTexture.ObjLetter();
			let objLetter: IosPluginTexture.ObjLetter = objLetterList[i]!;
			let size: CGSize = strLetterList[i].size(attributes: [NSFontAttributeName : font]);
			objLetter.code = Int32(strLetterList[i].unicodeScalars.first!.value);
			objLetter.w = Float(size.width);
			objLetter.h = Float(size.height);
			let marginTexw: Float = objLetter.w + margin;
			let marginTexh: Float = objLetter.h + margin;
			if(lineWidth + marginTexw <= Float(limitWidth)){
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

		if(maxWidth <= Float(limitWidth) && maxHeight <= Float(limitHeight)){
			// 2の累乗にする
			for i: Int in (0..<12){let size: Int = (1 << i); if(maxWidth <= Float(size)){maxWidth = Float(size); break;}}
			for i: Int in (0..<12){let size: Int = (1 << i); if(maxHeight <= Float(size)){maxHeight = Float(size); break;}}
			// 文字キャンバス作成
			UIGraphicsBeginImageContext(CGSize(width: Int(maxWidth), height: Int(maxHeight)));
			let attrs: [String: NSObject] = [NSFontAttributeName: font, NSForegroundColorAttributeName: UIColor.white, NSParagraphStyleAttributeName: NSMutableParagraphStyle()];
			lineWidth = margin;
			lineHeight = margin;
			maxHeight = margin;
			// 文字キャンバス描画
			for i: Int in (0 ..< Int(letterLenght)){
				let objLetter: IosPluginTexture.ObjLetter = objLetterList[i]!;
				let marginTexw: Float = objLetter.w + margin;
				let marginTexh: Float = objLetter.h + margin;
				if(lineWidth + marginTexw <= Float(limitWidth)){
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
				let codeListIndex: Int32 = gamePluginTextureFontCodeListCreate(letterLenght);
				for i: Int in (0 ..< Int(letterLenght)){
					let objLetter: IosPluginTexture.ObjLetter = objLetterList[i]!;
					gamePluginTextureFontCodeListSet(codeListIndex, Int32(i), fontSetId, objLetter.code, Int32(glId), Int32(width), Int32(height), Int32(objLetter.x), Int32(objLetter.y), Int32(objLetter.w), Int32(objLetter.h));
				}

				// フォントデータ保持
				let fontData: IosPluginTexture.FontData = IosPluginTexture.FontData();
				fontData.glId = Int32(glId);
				IosPluginTexture.fontList!.updateValue(fontData, forKey: codeListIndex);

				// コールバック
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginTextureFontCallbackCall(callbackId, codeListIndex);
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
		var x: Float = 0;
		var y: Float = 0;
		var w: Float = 0;
		var h: Float = 0;
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

