import Foundation
import UIKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginTexture: NSObject{
	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	static internal func platformPluginTextureLocal(callbackId: Int32, src: String){
		IosPluginUtil.nativePluginUtilLoadingIncrement();

		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), {
			let path: String = NSBundle.mainBundle().pathForResource(src, ofType: nil, inDirectory: "assets")!;
			let image: UIImage = UIImage(contentsOfFile: path)!;
			let imageRef: CGImageRef = image.CGImage!;
			let width: Int = CGImageGetWidth(imageRef);
			let height: Int = CGImageGetHeight(imageRef);
			let buffSize: Int = width * height * 4 * sizeof(GLubyte);
			let buff: UnsafeMutablePointer<Void> = UnsafeMutablePointer<Void>.alloc(buffSize);
			let colorSpace: CGColorSpaceRef = CGColorSpaceCreateDeviceRGB()!;
			let bitmapInfo: CGBitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.PremultipliedLast.rawValue);
			let context: CGContextRef = CGBitmapContextCreate(buff, width, height, 8, width * 4, colorSpace, bitmapInfo.rawValue)!;
			CGContextDrawImage(context, CGRectMake(0, 0, CGFloat(width), CGFloat(height)), imageRef);
			dispatch_async(dispatch_get_main_queue(), {
				var glId: GLuint = 0;
				glGenTextures(GLsizei(1), &glId);
				glBindTexture(GLenum(GL_TEXTURE_2D), glId);
				glTexImage2D(GLenum(GL_TEXTURE_2D), GLint(0), GL_RGBA, GLsizei(width), GLsizei(height), 0, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE), buff);
				buff.dealloc(buffSize);
				glGenerateMipmap(GLenum(GL_TEXTURE_2D));
				IosPluginUtil.nativePluginUtilLoadingDecrement();
				gamePluginTextureCallbackCall(UInt32(callbackId), UInt32(glId), UInt16(width), UInt16(height), UInt16(width), UInt16(height));
			});
		});
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

