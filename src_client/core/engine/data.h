#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// json処理

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

void dataJsonParse(struct dataJsonValue *this, char *json);
char *dataJsonStringify(struct dataJsonValue *this);

void dataJsonTrace(struct dataJsonValue *this);

void dataJsonSetInt(struct dataJsonValue *this, int64_t value);
void dataJsonSetFloat(struct dataJsonValue *this, double value);
void dataJsonSetBool(struct dataJsonValue *this, bool value);
void dataJsonSetString(struct dataJsonValue *this, char *value);
struct dataJsonValue *dataJsonObjectCreateValue(struct dataJsonValue *this, char *key);
struct dataJsonValue *dataJsonArrayCreateValue(struct dataJsonValue *this);

int64_t dataJsonGetInt(struct dataJsonValue *this, int64_t defaultValue);
double dataJsonGetFloat(struct dataJsonValue *this, double defaultValue);
bool dataJsonGetBool(struct dataJsonValue *this, bool defaultValue);
char *dataJsonGetString(struct dataJsonValue *this, char *defaultValue);
struct dataJsonValue *dataJsonObjectGetValue(struct dataJsonValue *this, char *key);
struct dataJsonValue *dataJsonArrayGetValue(struct dataJsonValue *this, uint32_t index);
uint32_t dataJsonArrayGetLength(struct dataJsonValue *this);

void dataJsonObjectRemoveValue(struct dataJsonValue *this, char *key);
void dataJsonArrayRemoveValue(struct dataJsonValue *this, uint32_t index);
void dataJsonFree(struct dataJsonValue *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// base64処理

char *dataBase64encode(uint8_t *data, uint32_t length);
uint8_t *dataBase64decode(char *data, uint32_t *length);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

