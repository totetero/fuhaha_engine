#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック識別子
typedef int pluginDataHttpCallbackId;
typedef int pluginDataLocalCallbackId;

// コールバック引数
#define PLUGINDATA_HTTP_CALLBACKPARAMS void *buff, size_t size
#define PLUGINDATA_LOCAL_CALLBACKPARAMS void *buff, size_t size

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にゲーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// コールバック関数の登録
pluginDataHttpCallbackId gamePluginDataHttpCallbackSet(void *param, void(*callback)(void *param, PLUGINDATA_HTTP_CALLBACKPARAMS));
pluginDataLocalCallbackId gamePluginDataLocalCallbackSet(void *param, void(*callback)(void *param, PLUGINDATA_LOCAL_CALLBACKPARAMS));

// コールバック関数の実行と解放
bool gamePluginDataHttpCallbackCall(pluginDataHttpCallbackId callbackId, PLUGINDATA_HTTP_CALLBACKPARAMS);
bool gamePluginDataLocalCallbackCall(pluginDataLocalCallbackId callbackId, PLUGINDATA_LOCAL_CALLBACKPARAMS);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, PLUGINDATA_HTTP_CALLBACKPARAMS));

// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, PLUGINDATA_LOCAL_CALLBACKPARAMS));

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

