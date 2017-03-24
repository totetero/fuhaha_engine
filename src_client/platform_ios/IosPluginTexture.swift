import Foundation
import UIKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginTexture: NSObject{
	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	static internal func platformPluginTextureLocal(_ callbackId: Int32, src: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		DispatchQueue(label: "fuhahaLoadLocalTexture").async(execute: {
			let path: String = Bundle.main.path(forResource: src, ofType: nil, inDirectory: "assets")!;
			let image: UIImage = UIImage(contentsOfFile: path)!;
			let imageRef: CGImage = image.cgImage!;
			let width: Int = imageRef.width;
			let height: Int = imageRef.height;
			var buff = [GLubyte](repeating: 0, count: width * height * 4);
			let colorSpace: CGColorSpace = CGColorSpaceCreateDeviceRGB();
			let bitmapInfo: CGBitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue);
			let context: CGContext = CGContext(data: &buff, width: width, height: height, bitsPerComponent: 8, bytesPerRow: width * 4, space: colorSpace, bitmapInfo: bitmapInfo.rawValue)!;
			context.draw(imageRef, in: CGRect(x: 0, y: 0, width: CGFloat(width), height: CGFloat(height)));
			DispatchQueue.main.async(execute: {
				var glId: GLuint = 0;
				glGenTextures(GLsizei(1), &glId);
				corePluginTextureIsBind(glId);
				glBindTexture(GLenum(GL_TEXTURE_2D), glId);
				glTexImage2D(GLenum(GL_TEXTURE_2D), GLint(0), GL_RGBA, GLsizei(width), GLsizei(height), 0, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE), buff);
				glGenerateMipmap(GLenum(GL_TEXTURE_2D));
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginTextureLocalCallbackCall(callbackId, Int32(glId), Int32(width), Int32(height));
			});
		});
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

