#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- json処理

// 構造体の前方宣言
struct dataJsonArray;

struct dataJsonValue{
	enum dataJsonType{
		DATAJSONTYPE_INT,
		DATAJSONTYPE_FLOAT,
		DATAJSONTYPE_BOOL,
		DATAJSONTYPE_STRING,
		DATAJSONTYPE_OBJECT,
		DATAJSONTYPE_ARRAY,
		DATAJSONTYPE_NULL,
	} type;
	union{
		int64_t jInt;
		double jFloat;
		bool jBool;
		char *jString;
		struct dataJsonArray *jArray;
	};
};

struct dataJsonArray{
	struct dataJsonValue value;
	struct dataJsonArray *next;
	char key[1]; // 可変長配列メンバ
};

// ----------------------------------------------------------------

// 文字列のjson解釈
void dataJsonParse(struct dataJsonValue *this, char *json);
// jsonを文字列に変換
char *dataJsonStringify(struct dataJsonValue *this);

// jsonテスト出力
void dataJsonTrace(struct dataJsonValue *this);

// 整数設定
void dataJsonSetInt(struct dataJsonValue *this, int64_t value);
// 浮動小数設定
void dataJsonSetFloat(struct dataJsonValue *this, double value);
// 真偽値設定
void dataJsonSetBool(struct dataJsonValue *this, bool value);
// 文字列設定
void dataJsonSetString(struct dataJsonValue *this, char *value);
// 連想配列に要素を作成して追加
struct dataJsonValue *dataJsonObjectCreateValue(struct dataJsonValue *this, char *key);
// 配列に要素を作成して追加
struct dataJsonValue *dataJsonArrayCreateValue(struct dataJsonValue *this);

// 整数読み取り
int64_t dataJsonGetInt(struct dataJsonValue *this, int64_t defaultValue);
// 浮動小数読み取り
double dataJsonGetFloat(struct dataJsonValue *this, double defaultValue);
// 真偽値読み取り
bool dataJsonGetBool(struct dataJsonValue *this, bool defaultValue);
// 文字列読み取り
char *dataJsonGetString(struct dataJsonValue *this, char *defaultValue);
// 連想配列から要素取得
struct dataJsonValue *dataJsonObjectGetValue(struct dataJsonValue *this, char *key);
// 配列から要素取得
struct dataJsonValue *dataJsonArrayGetValue(struct dataJsonValue *this, uint32_t index);
// 配列の長さ取得
uint32_t dataJsonArrayGetLength(struct dataJsonValue *this);

// 連想配列から要素削除
void dataJsonObjectRemoveValue(struct dataJsonValue *this, char *key);
// 配列から要素削除
void dataJsonArrayRemoveValue(struct dataJsonValue *this, uint32_t index);
// json解放
void dataJsonFree(struct dataJsonValue *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- base64処理

// base64形式に変換 引数非破壊 返値は解放必要
char *dataBase64encode(uint8_t *data, uint32_t length);
// base64形式から解読 引数破壊 返値は解放不要
uint8_t *dataBase64decode(char *data, uint32_t *length);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 読み込み処理

// リクエスト識別子
typedef uint32_t dataRequestId;

// ----------------------------------------------------------------

// 読み込み完了確認
bool dataRequestIsLoading();

// 通信リクエスト
dataRequestId dataRequestHttp(char *url, char *request);
// ローカルファイルリクエスト
dataRequestId dataRequestLocal(char *src);

// リクエスト応答
bool dataRequestResponse(dataRequestId reqId, void **buff, size_t *size);

// リクエスト解放
void dataRequestFree(dataRequestId reqId);
// 全リクエスト解放
void dataRequestFreeAll();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

