#import "./fuhaha-Swift.h"
#include "fuhahaEngine.h"
#include "./native.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ローカルデータ読み込み glIdの解放はゲーム側で管理する
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS)){
	pluginTextureLocalCallbackId callbackId = gamePluginTextureLocalCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginTexture platformPluginTextureLocal: callbackId src: nsstr1];
}

// フォントテクスチャ作成 glIdとコールバックバッファの解放はプラットフォーム側で管理する
void platformPluginTextureFont(void *param, enum pluginTextureFontSetId fontSetId, char *letterList, void(*callback)(void *param, PLUGINTEXTURE_FONT_CALLBACKPARAMS)){
	pluginTextureFontCallbackId callbackId = gamePluginTextureFontCallbackSet(param, callback);
	NSString *nsstr1 = [NSString stringWithCString: letterList encoding: NSUTF8StringEncoding];
	[IosPluginTexture platformPluginTextureFont: callbackId fontSetId: fontSetId letterList: nsstr1];
}

// フォントテクスチャ破棄
void platformPluginTextureFontDispose(int codeListIndex){
	[IosPluginTexture platformPluginTextureFontDispose: codeListIndex];
	
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

