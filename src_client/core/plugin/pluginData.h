#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// リクエスト識別子
typedef uint32_t pluginDataRequestId;

// コールバック識別子
typedef uint32_t pluginDataCallbackId;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にゲーム側から呼び出す

// HTTP通信リクエスト
pluginDataRequestId corePluginDataRequestHttp(char *url, char *request);
// ローカルデータリクエスト
pluginDataRequestId corePluginDataRequestLocal(char *src);

// リクエスト応答 応答バッファは要開放
bool corePluginDataRequestResponse(pluginDataRequestId reqId, void **buff, size_t *size);

// リクエスト開放
void corePluginDataRequestFree(pluginDataRequestId reqId);
// 全リクエスト開放
void corePluginDataRequestFreeAll(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// コールバック関数の登録
pluginDataCallbackId gamePluginDataCallbackSet(void *param, void(*callback)(void *param, void *buff, size_t size));
// コールバック関数の実行と解放
bool gamePluginDataCallbackCall(pluginDataCallbackId callbackId, void *buff, size_t size);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, void *buff, size_t size));
// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, void *buff, size_t size));

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
