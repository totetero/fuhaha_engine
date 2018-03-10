#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// リクエスト識別子
typedef int engineDataRequestId;

// HTTP通信リクエスト
engineDataRequestId engineDataRequestHttp(char *url, char *request);
// ローカルデータリクエスト
engineDataRequestId engineDataRequestLocal(char *src);

// リクエスト応答 コールバックバッファは下記関数で解放するため解放禁止
bool engineDataRequestResponse(engineDataRequestId reqId, void **buff, size_t *size);

// リクエスト開放
void engineDataRequestFree(engineDataRequestId reqId);
// 全リクエスト開放
void engineDataRequestFreeAll(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

