package com.totetero.fuhaha;

import java.util.HashMap;
import java.nio.ByteBuffer;
import java.io.InputStream;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
public class AndroidPluginTexture{
	private static HashMap<Integer, AndroidPluginTexture.FontData> fontList = new HashMap<Integer, AndroidPluginTexture.FontData>();

	// ----------------------------------------------------------------

	// JNI連携
	static{System.loadLibrary("fuhaha_native");}
	public static native boolean corePluginTextureIsBind(int glId);
	public static native void gamePluginTextureLocalCallbackCall(int callbackId, int glId, int w, int h);
	public static native void gamePluginTextureFontCallbackCall(int callbackId, int codeListIndex, int codeListLength);
	public static native int gamePluginTextureFontCodeListCreate(int codeListLength);
	public static native void gamePluginTextureFontCodeListSet(int codeListIndex, int index, int fontSetId, int code, int glId, int imgw, int imgh, int x, int y, int w, int h);
	public static native void gamePluginTextureFontCodeListDispose(int codeListIndex);

	// ----------------------------------------------------------------

	// テクスチャ作成
	private static int createTexture(Bitmap bitmap){
		int[] glIds = new int[1];
		GLES20.glGenTextures(1, glIds, 0);
		int glId = glIds[0];
		int width = bitmap.getWidth();
		int height = bitmap.getHeight();

		AndroidPluginTexture.corePluginTextureIsBind(glId);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, glId);
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
		GLES20.glGenerateMipmap(GLES20.GL_TEXTURE_2D);

		// nexus6で中断復帰直後にmipmapの生成がうまくいっていないっぽいので一呼吸おいて再生成してみる
		final int inner_glId = glId;
		(new Thread(new Runnable(){public void run(){
			try{Thread.sleep(100);}catch(Exception e){}
			FuhahaGLView.surfaceView.queueEvent(new Runnable(){public void run(){
				AndroidPluginTexture.corePluginTextureIsBind(inner_glId);
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, inner_glId);
				GLES20.glGenerateMipmap(GLES20.GL_TEXTURE_2D);
			}});
		}})).start();

		return glId;
	}

	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	public static void platformPluginTextureLocal(int callbackId, final String src){
		AndroidPluginUtil.nativePluginUtilLoadingIncrement();

		final int inner_callbackId = callbackId;
		new Thread(new Runnable(){public void run(){
			Bitmap bitmap = null;
			try{
				InputStream stream = FuhahaGLView.activity.getResources().getAssets().open(src);
				bitmap = BitmapFactory.decodeStream(stream);
				stream.close();
			}catch(Exception e){e.printStackTrace();}

			// メインスレッドでビットマップテクスチャ作成
			final Bitmap inner_bitmap = bitmap;
			FuhahaGLView.surfaceView.queueEvent(new Runnable(){public void run(){
				int glId = 0;
				int width = 0;
				int height = 0;
				if(inner_bitmap != null){
					width = inner_bitmap.getWidth();
					height = inner_bitmap.getHeight();
					glId = AndroidPluginTexture.createTexture(inner_bitmap);
					inner_bitmap.recycle();
				}

				// コールバック
				AndroidPluginUtil.nativePluginUtilLoadingDecrement();
				AndroidPluginTexture.gamePluginTextureLocalCallbackCall(inner_callbackId, glId, width, height);
			}});
		}}).start();
	}

	// ----------------------------------------------------------------

	// フォント文字列作成
	private static Typeface getFontSet(int fontSetId){
		return null;
	}

	// フォント文字列作成
	private static int getFontSize(int fontSetId){
		return 32;
	}

	// フォントテクスチャ作成
	public static void platformPluginTextureFont(int callbackId, int fontSetId, String letterList){
		AndroidPluginUtil.nativePluginUtilLoadingIncrement();

		// フォント準備
		Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
		paint.setTypeface(AndroidPluginTexture.getFontSet(fontSetId));
		paint.setTextSize(AndroidPluginTexture.getFontSize(fontSetId));
		paint.setColor(Color.WHITE);
		Paint.FontMetrics fontMetrics = paint.getFontMetrics();

		int letterLength = letterList.length();
		String[] strLetterList = letterList.split("(?<=.)");
		AndroidPluginTexture.ObjLetter[] objLetterList = new AndroidPluginTexture.ObjLetter[letterLength];

		// 文字サイズの計測
		float margin = 2;
		float lineWidth = margin;
		float lineHeight = margin;
		float maxWidth = margin;
		float maxHeight = margin;
		int limitWidth = (1 << 10);
		int limitHeight = (1 << 10);
		for(int i = 0; i < letterLength; i++){
			AndroidPluginTexture.ObjLetter objLetter = objLetterList[i] = new AndroidPluginTexture.ObjLetter();
			objLetter.code = Character.codePointAt(strLetterList[i], 0);
			objLetter.w = paint.measureText(strLetterList[i]);
			objLetter.h = fontMetrics.descent - fontMetrics.ascent;
			float marginTexw = objLetter.w + margin;
			float marginTexh = objLetter.h + margin;
			if(lineWidth + marginTexw <= limitWidth){
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

		if(maxWidth <= limitWidth && maxHeight <= limitHeight){
			// 2の累乗にする
			for(int i = 0; i < 12; i++){int size = (1 << i); if(maxWidth <= size){maxWidth = size; break;}}
			for(int i = 0; i < 12; i++){int size = (1 << i); if(maxHeight <= size){maxHeight = size; break;}}
			// 文字キャンバス作成
			Bitmap bitmap = Bitmap.createBitmap((int)maxWidth, (int)maxHeight, Bitmap.Config.ARGB_8888); 
			Canvas canvas = new Canvas(bitmap);
			lineWidth = margin;
			lineHeight = margin;
			maxHeight = margin;
			// 文字キャンバス描画
			for(int i = 0; i < letterLength; i++){
				AndroidPluginTexture.ObjLetter objLetter = objLetterList[i];
				float marginTexw = objLetter.w + margin;
				float marginTexh = objLetter.h + margin;
				if(lineWidth + marginTexw <= limitWidth){
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
				canvas.drawText(strLetterList[i], objLetter.x, objLetter.y - fontMetrics.ascent, paint);
			}

			// テクスチャ作成
			int width = bitmap.getWidth();
			int height = bitmap.getHeight();
			int glId = AndroidPluginTexture.createTexture(bitmap);
			bitmap.recycle();

			// ネイティブデータ作成
			int codeListIndex = AndroidPluginTexture.gamePluginTextureFontCodeListCreate(letterLength);
			for(int i = 0; i < letterLength; i++){
				AndroidPluginTexture.ObjLetter objLetter = objLetterList[i];
				AndroidPluginTexture.gamePluginTextureFontCodeListSet(codeListIndex, i, fontSetId, objLetter.code, glId, width, height, (int)objLetter.x, (int)objLetter.y, (int)objLetter.w, (int)objLetter.h);
			}

			// フォントデータ保持
			AndroidPluginTexture.FontData fontData = new AndroidPluginTexture.FontData();
			fontData.glId = glId;
			AndroidPluginTexture.fontList.put(codeListIndex, fontData);

			// コールバック
			AndroidPluginUtil.nativePluginUtilLoadingDecrement();
			AndroidPluginTexture.gamePluginTextureFontCallbackCall(callbackId, codeListIndex, letterLength);
		}else{
			// 必要領域が大きすぎる
			// TODO
		}
	}

	// フォントテクスチャ破棄
	public static void platformPluginTextureFontDispose(int codeListIndex){
		AndroidPluginTexture.FontData fontData = AndroidPluginTexture.fontList.get(codeListIndex);
		if(fontData == null){return;}
		int glId = fontData.glId;
		AndroidPluginTexture.fontList.remove(codeListIndex);
		// テクスチャ除去
		int[] glIds = new int[1];
		glIds[0] = glId;
		GLES20.glDeleteTextures(1, glIds, 0);
		// ネイティブデータ破棄
		AndroidPluginTexture.gamePluginTextureFontCodeListDispose(codeListIndex);
	}

	// 文字データ
	static class ObjLetter{
		int code = -1;
		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;
	}

	// 保持フォントデータ
	static class FontData{
		int glId = 0;
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

