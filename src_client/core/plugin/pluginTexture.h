#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 文字テクスチャ情報
struct pluginTextureFontCode{
	int code;
	int x;
	int y;
	int w;
	int h;
};

// コールバック識別子
typedef int pluginTextureLocalCallbackId;
typedef int pluginTextureFontCallbackId;

// コールバック引数
#define PLUGINTEXTURE_LOCAL_CALLBACKPARAMS int glId, int w, int h
#define PLUGINTEXTURE_FONT_CALLBACKPARAMS int glId, int w, int h, struct pluginTextureFontCode *codeList

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にゲーム側から呼び出す

// テクスチャバインド状態の記録と重複確認
bool corePluginTextureIsBind(GLuint glId);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// コールバック関数の登録
pluginTextureLocalCallbackId gamePluginTextureLocalCallbackSet(void *param, void(*callback)(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS));
pluginTextureFontCallbackId gamePluginTextureFontCallbackSet(void *param, void(*callback)(void *param, PLUGINTEXTURE_FONT_CALLBACKPARAMS));

// コールバック関数の実行と解放
bool gamePluginTextureLocalCallbackCall(pluginTextureLocalCallbackId callbackId, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS);
bool gamePluginTextureFontCallbackCall(pluginTextureFontCallbackId callbackId, PLUGINTEXTURE_FONT_CALLBACKPARAMS);

// フォントテクスチャ用文字リスト作成
struct pluginTextureFontCode *gamePluginTextureFontCodeListCreate(int letterLength);
void gamePluginTextureFontCodeListSet(struct pluginTextureFontCode *codeList, int index, int code, int x, int y, int w, int h);
void gamePluginTextureFontCodeListDispose(struct pluginTextureFontCode *codeList);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// ローカルデータ読み込み glIdの解放はゲーム側で管理する
void platformPluginTextureLocal(void *param, char *src, void(*callback)(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS));

// フォントテクスチャ作成 glIdとコールバックバッファの解放はプラットフォーム側で管理する
void platformPluginTextureFont(void *param, int fontSetId, char *letterList, int letterLenght, void(*callback)(void *param, PLUGINTEXTURE_FONT_CALLBACKPARAMS));
void platformPluginTextureFontDispose(int glId, struct pluginTextureFontCode *codeList);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

