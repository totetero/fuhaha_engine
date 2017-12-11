package com.totetero.fuhaha;

import java.io.InputStream;
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;

import java.net.HttpURLConnection;
import java.net.URL;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
public class AndroidPluginData{
	// ----------------------------------------------------------------

	// JNI連携
	static{System.loadLibrary("fuhaha_native");}
	public static native void gamePluginDataHttpCallbackCallBinary(int callbackId, byte[] buff);
	public static native void gamePluginDataHttpCallbackCallString(int callbackId, String buff);
	public static native void gamePluginDataLocalCallbackCallBinary(int callbackId, byte[] buff);
	public static native void gamePluginDataLocalCallbackCallString(int callbackId, String buff);

	// ----------------------------------------------------------------

	// HTTP通信
	public static void platformPluginDataHttp(int callbackId, final String url, final String request){
		final Callback callback = new CallbackHttp(callbackId);
		new Thread(new Runnable(){public void run(){
			byte[] buff = null;
			HttpURLConnection connect = null;
			try{
				URL newUrl = new URL(AndroidPluginUtil.gamePluginUtilUrlGet() + "/" + url);
				connect = (HttpURLConnection)newUrl.openConnection();
				connect.setRequestMethod("POST");
				connect.setDoInput(true);
				connect.setDoOutput(true);
				connect.setUseCaches(false);
				connect.setRequestProperty("Content-Type", "application/octet-stream");

				DataOutputStream postStream = new DataOutputStream(connect.getOutputStream());
				postStream.writeBytes(request);
				postStream.close();

				InputStream inputStream = connect.getInputStream();
				int tempLength;
				byte [] tempBuff = new byte[1024];
				ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
				while((tempLength = inputStream.read(tempBuff)) >= 0){outputStream.write(tempBuff, 0, tempLength);}
				buff = outputStream.toByteArray();
			}catch(Exception e){e.printStackTrace();}
			finally{if(connect != null){try{connect.disconnect();}catch(Exception e){e.printStackTrace();}}}
			FuhahaGLView.surfaceView.queueEvent(callback.setBinBuff(buff));
		}}).start();
	}

	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	public static void platformPluginDataLocal(int callbackId, final String src){
		final Callback callback = new CallbackLocal(callbackId);
		new Thread(new Runnable(){public void run(){
			byte[] buff = null;
			try{
				InputStream inputStream = new BufferedInputStream(FuhahaGLView.activity.getResources().getAssets().open(src));
				int tempLength;
				byte [] tempBuff = new byte[1024];
				ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
				while((tempLength = inputStream.read(tempBuff)) >= 0){outputStream.write(tempBuff, 0, tempLength);}
				buff = outputStream.toByteArray();
			}catch(Exception e){e.printStackTrace();}
			FuhahaGLView.surfaceView.queueEvent(callback.setBinBuff(buff));
		}}).start();
	}

	// ----------------------------------------------------------------

	// メインスレッドでコールバック呼び出しクラス
	private static abstract class Callback implements Runnable{
		protected int callbackId;
		protected byte[] binBuff = null;
		protected String strBuff = null;
		public Callback(int callbackId){
			AndroidPluginUtil.nativePluginUtilLoadingIncrement();
			this.callbackId = callbackId;
		}
		public Callback setBinBuff(byte[] binBuff){this.binBuff = binBuff; return this;}
		public Callback setStrBuff(String strBuff){this.strBuff = strBuff; return this;}
		public void run(){
			//if(this.binBuff == null && this.strBuff == null){this.strBuff = "failed";}
			AndroidPluginUtil.nativePluginUtilLoadingDecrement();
			if(this.binBuff == null && this.strBuff != null){this.callbackString();}else{this.callbackBinary();}
		}
		protected abstract void callbackBinary();
		protected abstract void callbackString();
	}
	private static class CallbackHttp extends Callback{
		public CallbackHttp(int callbackId){super(callbackId);}
		protected void callbackBinary(){AndroidPluginData.gamePluginDataHttpCallbackCallBinary(this.callbackId, this.binBuff);}
		protected void callbackString(){AndroidPluginData.gamePluginDataHttpCallbackCallString(this.callbackId, this.strBuff);}
	}
	private static class CallbackLocal extends Callback{
		public CallbackLocal(int callbackId){super(callbackId);}
		protected void callbackBinary(){AndroidPluginData.gamePluginDataLocalCallbackCallBinary(this.callbackId, this.binBuff);}
		protected void callbackString(){AndroidPluginData.gamePluginDataLocalCallbackCallString(this.callbackId, this.strBuff);}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

