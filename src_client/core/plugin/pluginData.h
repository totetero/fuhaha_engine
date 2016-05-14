#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// リクエスト識別子
typedef int pluginDataRequestId;

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
void corePluginDataRequestFreeAll();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// コールバック関数の登録
int gamePluginDataCallbackSet(void *param, void(*callback)(void *param, void *buff, size_t size));
// コールバック関数の実行と解放
bool gamePluginDataCallbackCall(int callbackId, void *buff, size_t size);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// HTTP通信 コールバックバッファは要解放
void platformPluginDataHttp(void *param, char *url, char *request, void(*callback)(void *param, void *buff, size_t size));
// ローカルデータ読み込み コールバックバッファは要解放
void platformPluginDataLocal(void *param, char *src, void(*callback)(void *param, void *buff, size_t size));

// ----------------------------------------------------------------

// 設定読み込み 返値文字列は揮発性バッファで解放禁止
char *platformPluginDataPreferenceGet(char *key);
// 設定書き込み
void platformPluginDataPreferenceSet(char *key, char *value);
// 設定保存
void platformPluginDataPreferenceCommit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

