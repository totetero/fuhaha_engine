#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// リクエスト識別子
typedef int engineDataRequestId;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- json処理

// 構造体の前方宣言
struct engineDataJsonArray;

struct engineDataJsonValue{
	enum engineDataJsonType{
		ENGINEDATAJSONTYPE_NULL = 0,
		ENGINEDATAJSONTYPE_INT,
		ENGINEDATAJSONTYPE_FLOAT,
		ENGINEDATAJSONTYPE_BOOL,
		ENGINEDATAJSONTYPE_STRING,
		ENGINEDATAJSONTYPE_OBJECT,
		ENGINEDATAJSONTYPE_ARRAY,
	} type;
	union{
		int64_t jInt;
		double jFloat;
		bool jBool;
		char *jString;
		struct engineDataJsonArray *jArray;
	};
};

struct engineDataJsonArray{
	struct engineDataJsonValue value;
	struct engineDataJsonArray *next;
	char key[1]; // 可変長配列メンバ
};

// ----------------------------------------------------------------

// 文字列のjson解釈
void engineDataJsonParseChar(struct engineDataJsonValue *this, char *json);
void engineDataJsonParse(struct engineDataJsonValue *this, byte *json, size_t size);
// jsonを文字列に変換
char *engineDataJsonStringify(struct engineDataJsonValue *this);

// jsonテスト出力
void engineDataJsonTrace(struct engineDataJsonValue *this);

// 整数設定
void engineDataJsonSetInt(struct engineDataJsonValue *this, int64_t value);
// 浮動小数設定
void engineDataJsonSetFloat(struct engineDataJsonValue *this, double value);
// 真偽値設定
void engineDataJsonSetBool(struct engineDataJsonValue *this, bool value);
// 文字列設定
void engineDataJsonSetString(struct engineDataJsonValue *this, char *value);
// 連想配列に要素を作成して追加
struct engineDataJsonValue *engineDataJsonObjectCreateValue(struct engineDataJsonValue *this, char *key);
// 配列に要素を作成して追加
struct engineDataJsonValue *engineDataJsonArrayCreateValue(struct engineDataJsonValue *this);

// 整数読み取り
int64_t engineDataJsonGetInt(struct engineDataJsonValue *this, int64_t defaultValue);
// 浮動小数読み取り
double engineDataJsonGetFloat(struct engineDataJsonValue *this, double defaultValue);
// 真偽値読み取り
bool engineDataJsonGetBool(struct engineDataJsonValue *this, bool defaultValue);
// 文字列読み取り
char *engineDataJsonGetString(struct engineDataJsonValue *this, char *defaultValue);
// 連想配列から要素取得
struct engineDataJsonValue *engineDataJsonObjectGetValue(struct engineDataJsonValue *this, char *key);
// 配列から要素取得
struct engineDataJsonValue *engineDataJsonArrayGetValue(struct engineDataJsonValue *this, int index);
// 配列の長さ取得
int engineDataJsonArrayGetLength(struct engineDataJsonValue *this);

// 連想配列から要素削除
void engineDataJsonObjectRemoveValue(struct engineDataJsonValue *this, char *key);
// 配列から要素削除
void engineDataJsonArrayRemoveValue(struct engineDataJsonValue *this, int index);
// json解放
void engineDataJsonFree(struct engineDataJsonValue *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- base64処理

// base64形式に変換 返値文字列は揮発性バッファで解放禁止
char *engineDataBase64encode(byte *data, size_t size);
// base64形式から解読 返値データは揮発性バッファで解放禁止
byte *engineDataBase64decodeChar(char *data, int *length);
byte *engineDataBase64decode(byte *data, size_t size);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- フレームベースのデータリクエスト処理

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

