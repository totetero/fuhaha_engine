#include "../library.h"
#include "platform.h"
#include "engineData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// 一時バッファ
	char *tempBuff;
	uint32_t tempBuffIndex;
	uint32_t tempBuffLength;
} localGlobal = {0};

// 一時バッファの長さ確保
static void tempBuffSetLength(uint32_t length){
	if(localGlobal.tempBuffLength < localGlobal.tempBuffIndex + length){
		uint32_t newBuffLength = localGlobal.tempBuffLength + (length > 1024 ? length : 1024);
		char *newBuff = (char*)malloc(newBuffLength * sizeof(char));
		if(localGlobal.tempBuffLength > 0){
			memcpy(newBuff, localGlobal.tempBuff, localGlobal.tempBuffLength * sizeof(char));
			free(localGlobal.tempBuff);
		}
		localGlobal.tempBuffLength = newBuffLength;
		localGlobal.tempBuff = newBuff;
	}
}
// 一時バッファに挿入
static void tempBuffPutChar(char c){tempBuffSetLength(1); localGlobal.tempBuff[localGlobal.tempBuffIndex++] = c;}
static void tempBuffPutInt(int64_t value){size_t n = 64; tempBuffSetLength((uint32_t)n); localGlobal.tempBuffIndex += snprintf(localGlobal.tempBuff + localGlobal.tempBuffIndex, n, "%lld", value);}
static void tempBuffPutFloat(double value){size_t n = 64; tempBuffSetLength((uint32_t)n); localGlobal.tempBuffIndex += snprintf(localGlobal.tempBuff + localGlobal.tempBuffIndex, n, "%lf", value);}
static void tempBuffPutString1(char *value){size_t n = strlen(value)     + 1; tempBuffSetLength((uint32_t)n); localGlobal.tempBuffIndex += snprintf(localGlobal.tempBuff + localGlobal.tempBuffIndex, n,     "%s", value);}
static void tempBuffPutString2(char *value){size_t n = strlen(value) + 2 + 1; tempBuffSetLength((uint32_t)n); localGlobal.tempBuffIndex += snprintf(localGlobal.tempBuff + localGlobal.tempBuffIndex, n, "\"%s\"", value);}

// ----------------------------------------------------------------

// 値文字列エスケープ読み取り
static bool parseStringEscape(char **p){
	char *c = *p;

	if(*c != '\\'){return false;}
	c++;
	if(*c == '\0'){return false;}
	else if(*c == '\n'){return false;}
	else if(*c == '\\'){tempBuffPutChar('\\'); c++;}
	else if(*c == '\''){tempBuffPutChar('\''); c++;}
	else if(*c == '\"'){tempBuffPutChar('\"'); c++;}
	else if(*c == 'n'){tempBuffPutChar('\n'); c++;}
	else if(*c == 't'){tempBuffPutChar('\t'); c++;}
	else if(*c == 'u'){
		// utf-16 めんどいのでいろいろ略
		c++; if(*c == '\0' || *c == '\n' || *c == '\'' || *c == '\"'){return false;}
		c++; if(*c == '\0' || *c == '\n' || *c == '\'' || *c == '\"'){return false;}
		c++; if(*c == '\0' || *c == '\n' || *c == '\'' || *c == '\"'){return false;}
		c++; if(*c == '\0' || *c == '\n' || *c == '\'' || *c == '\"'){return false;}
	}else{return false;}

	*p = c;
	return true;
}

// 値文字列読み取り
static bool parseString(struct dataJsonValue *this, char **p){
	char *c = *p;
	localGlobal.tempBuffIndex = 0;

	// ダブルクォーテーション開始
	if(*(c++) != '\"'){return false;}

	// ダブルクォーテーション内部
	while(*c != '\"'){
		if(*c == '\0' || *c == '\n'){
			// クォーテーションで閉じずに文字列の終わりが来たら読み取り失敗
			return false;
		}else if(!parseStringEscape(&c)){
			// エスケープしないのであれば一文字読み取り
			tempBuffPutChar(*(c++));
		}
	}

	// ダブルクォーテーション完了
	c++;

	// 文字列登録
	localGlobal.tempBuff[localGlobal.tempBuffIndex++] = '\0';
	if(this != NULL){
		this->type = DATAJSONTYPE_STRING;
		this->jString = localGlobal.tempBuff;
	}

	*p = c;
	return true;
}

// キー文字列読み取り
static bool parseKeyString(struct dataJsonValue *this, char **p){
	char *c = *p;
	localGlobal.tempBuffIndex = 0;

	// 文字列読み取り
	while(('0' <= *c && *c <= '9') || ('a' <= *c && *c <= 'z') || ('A' <= *c && *c <= 'Z') || *c == '_'){
		tempBuffPutChar(*(c++));
	}

	// 文字列登録
	localGlobal.tempBuff[localGlobal.tempBuffIndex++] = '\0';
	if(this != NULL){
		this->type = DATAJSONTYPE_STRING;
		this->jString = localGlobal.tempBuff;
	}

	*p = c;
	return true;
}

// 数字読み取り
static bool parseNumber(struct dataJsonValue *this, char **p){
	char *c = *p;
	localGlobal.tempBuffIndex = 0;

	// 数字を表す文字列を読み込む
	bool isFloat = false;
	// 負値
	if(*c == '-'){tempBuffPutChar(*(c++));}
	// 整数部
	if(*c == '0'){tempBuffPutChar(*(c++));}
	else if('1' <= *c && *c <= '9'){do{tempBuffPutChar(*(c++));}while('0' <= *c && *c <= '9');}
	else{return false;}
	// 小数部
	if(*c == '.'){
		isFloat = true;
		tempBuffPutChar(*(c++));
		if('0' <= *c && *c <= '9'){do{tempBuffPutChar(*(c++));}while('0' <= *c && *c <= '9');}
		else{return false;}
	}
	// 浮動小数
	if(*c == 'e' || *c == 'E'){
		isFloat = true;
		tempBuffPutChar(*(c++));
		if(*c == '+'){tempBuffPutChar(*(c++));}
		else if(*c == '-'){tempBuffPutChar(*(c++));}
		if('0' <= *c && *c <= '9'){do{tempBuffPutChar(*(c++));}while('0' <= *c && *c <= '9');}
		else{return false;}
	}

	// 数字の確認
	localGlobal.tempBuff[localGlobal.tempBuffIndex++] = '\0';
	if(this != NULL){
		if(isFloat){
			this->type = DATAJSONTYPE_FLOAT;
			sscanf(localGlobal.tempBuff, "%lf", &this->jFloat);
		}else{
			this->type = DATAJSONTYPE_INT;
			sscanf(localGlobal.tempBuff, "%lld", &this->jInt);
		}
	}

	*p = c;
	return true;
}

// true読み取り
static bool parseTrue(struct dataJsonValue *this, char **p){
	char *c = *p;

	if(*(c++) != 't'){return false;}
	if(*(c++) != 'r'){return false;}
	if(*(c++) != 'u'){return false;}
	if(*(c++) != 'e'){return false;}
	if(this != NULL){
		this->type = DATAJSONTYPE_BOOL;
		this->jBool = true;
	}

	*p = c;
	return true;
}

// false読み取り
static bool parseFalse(struct dataJsonValue *this, char **p){
	char *c = *p;

	if(*(c++) != 'f'){return false;}
	if(*(c++) != 'a'){return false;}
	if(*(c++) != 'l'){return false;}
	if(*(c++) != 's'){return false;}
	if(*(c++) != 'e'){return false;}
	if(this != NULL){
		this->type = DATAJSONTYPE_BOOL;
		this->jBool = false;
	}

	*p = c;
	return true;
}

// null読み取り
static bool parseNull(struct dataJsonValue *this, char **p){
	char *c = *p;

	if(*(c++) != 'n'){return false;}
	if(*(c++) != 'u'){return false;}
	if(*(c++) != 'l'){return false;}
	if(*(c++) != 'l'){return false;}
	if(this != NULL){
		this->type = DATAJSONTYPE_NULL;
	}

	*p = c;
	return true;
}

// ----------------------------------------------------------------

// スペースの削除
static bool skipSpace(char **c){
	// スペースの削除
	while(**c == ' ' || **c == '\t' || **c == '\n'){(*c)++;}
	if(**c == '\0'){return false;}
	// コメントチェック
	bool isComment1 = (*(*c + 0) == '/' && *(*c + 1) == '*');
	bool isComment2 = (*(*c + 0) == '/' && *(*c + 1) == '/');
	while(isComment1 || isComment2){
		*c += 2;
		while(isComment1 && **c != '\0' && !(*(*c - 2) ==  '*' && *(*c - 1) ==  '/')){(*c)++;}
		while(isComment2 && **c != '\0' && !(*(*c - 2) != '\\' && *(*c - 1) == '\n')){(*c)++;}
		// スペースの削除
		while(**c == ' ' || **c == '\t' || **c == '\n'){(*c)++;}
		if(**c == '\0'){return false;}
		// コメントチェック
		isComment1 = (*(*c + 0) == '/' && *(*c + 1) == '*');
		isComment2 = (*(*c + 0) == '/' && *(*c + 1) == '/');
	}
	return true;
}

// json要素読み込み
static bool parseValue(struct dataJsonValue *this, char **c){
	skipSpace(c);

	if(**c == '{'){
		(*c)++;
		this->type = DATAJSONTYPE_OBJECT;
		this->jArray = NULL;
		// 連想配列読み取り
		skipSpace(c);
		while(**c != '}'){
			// 連想配列キー
			struct dataJsonValue key;
			bool isKey = false;
			if(!isKey){isKey = parseString(&key, c);}
			if(!isKey){isKey = parseKeyString(&key, c);}
			if(!isKey){return false;}
			// コロン
			skipSpace(c);
			if(**c != ':'){return false;}
			(*c)++;
			// 連想配列値
			struct dataJsonValue *value = dataJsonObjectCreateValue(this, key.jString);
			if(!parseValue(value, c)){return false;}
			// カンマ
			skipSpace(c);
			if(**c != ','){break;}
			(*c)++;
			skipSpace(c);
		}
		if(**c != '}'){return false;}
		(*c)++;
		return true;
	}

	if(**c == '['){
		(*c)++;
		this->type = DATAJSONTYPE_ARRAY;
		this->jArray = NULL;
		// 配列読み取り
		skipSpace(c);
		while(**c != ']'){
			// 配列値
			struct dataJsonValue *value = dataJsonArrayCreateValue(this);
			if(!parseValue(value, c)){return false;}
			// カンマ
			skipSpace(c);
			if(**c != ','){break;}
			(*c)++;
			skipSpace(c);
		}
		if(**c != ']'){return false;}
		(*c)++;
		return true;
	}

	if(parseString(this, c)){
		// 文字列読み取り成功 メモリ領域確保
		uint32_t length = (uint32_t)strlen(this->jString);
		char *buff = (char*)malloc((length + 1) * sizeof(char));
		strcpy(buff, this->jString);
		this->jString = buff;
		return true;
	}

	if(parseNumber(this, c)){return true;}
	if(parseTrue(this, c)){return true;}
	if(parseFalse(this, c)){return true;}
	if(parseNull(this, c)){return true;}
	return false;
}

// ----------------------------------------------------------------

// jsonの文字列化
static void jsonStringify(struct dataJsonValue *this, int32_t indent){
	if(this == NULL){return;}
	switch(this->type){
		case DATAJSONTYPE_INT: tempBuffPutInt(this->jInt); break;
		case DATAJSONTYPE_FLOAT: tempBuffPutFloat(this->jFloat); break;
		case DATAJSONTYPE_BOOL: tempBuffPutString1(this->jBool ? "true" : "false"); break;
		case DATAJSONTYPE_STRING: tempBuffPutString2(this->jString); break;
		case DATAJSONTYPE_NULL: tempBuffPutString1("null"); break;
		case DATAJSONTYPE_OBJECT:
			tempBuffPutChar('{');
			if(this->jArray != NULL){
				if(indent >= 0){tempBuffPutChar('\n');}
				if(indent >= 0){for(uint32_t i = 0; i < indent + 1; i++){tempBuffPutChar('\t');}}
				struct dataJsonArray *tempObj = this->jArray;
				while(tempObj != NULL){
					tempBuffPutString1(tempObj->key);
					tempBuffPutChar(':');
					if(indent >= 0){tempBuffPutChar(' ');}
					jsonStringify(&tempObj->value, indent + (indent < 0 ? 0 : 1));
					tempObj = tempObj->next;
					if(tempObj == NULL){break;}
					tempBuffPutChar(',');
					if(indent >= 0){tempBuffPutChar(' ');}
				}
				if(indent >= 0){tempBuffPutChar('\n');}
				if(indent >= 0){for(uint32_t i = 0; i < indent; i++){tempBuffPutChar('\t');}}
			}
			tempBuffPutChar('}');
			break;
		case DATAJSONTYPE_ARRAY:
			tempBuffPutChar('[');
			if(this->jArray != NULL){
				if(indent >= 0){tempBuffPutChar('\n');}
				if(indent >= 0){for(uint32_t i = 0; i < indent + 1; i++){tempBuffPutChar('\t');}}
				struct dataJsonArray *tempArr = this->jArray;
				while(tempArr != NULL){
					jsonStringify(&tempArr->value, indent + (indent < 0 ? 0 : 1));
					tempArr = tempArr->next;
					if(tempArr == NULL){break;}
					tempBuffPutChar(',');
					if(indent >= 0){tempBuffPutChar(' ');}
				}
				if(indent >= 0){tempBuffPutChar('\n');}
				if(indent >= 0){for(uint32_t i = 0; i < indent; i++){tempBuffPutChar('\t');}}
			}
			tempBuffPutChar(']');
			break;
		default: break;
	}
}

// ----------------------------------------------------------------

// 文字列のjson解釈
void dataJsonParse(struct dataJsonValue *this, char *json){
	char **c = &json;
	if(parseValue(this, c)){
		if(!skipSpace(c)){
			// 読み取り成功
			return;
		}
	}
	// 読み取り失敗
	dataJsonFree(this);
	this->type = DATAJSONTYPE_NULL;
}

// jsonを文字列に変換
char *dataJsonStringify(struct dataJsonValue *this){
	if(this == NULL){return NULL;}
	localGlobal.tempBuffIndex = 0;
	jsonStringify(this, -1);
	tempBuffPutChar('\0');

	uint32_t length = (uint32_t)strlen(localGlobal.tempBuff);
	char *buff = (char*)malloc((length + 1) * sizeof(char));
	strcpy(buff, localGlobal.tempBuff);
	return buff;
}

// ----------------------------------------------------------------

// jsonテスト出力
void dataJsonTrace(struct dataJsonValue *this){
	if(this == NULL){return;}
	localGlobal.tempBuffIndex = 0;
	jsonStringify(this, 0);
	tempBuffPutChar('\0');
	trace("%s", localGlobal.tempBuff);
}

// ----------------------------------------------------------------

// 整数設定
void dataJsonSetInt(struct dataJsonValue *this, int64_t value){
	this->type = DATAJSONTYPE_INT;
	this->jInt = value;
}

// 浮動小数設定
void dataJsonSetFloat(struct dataJsonValue *this, double value){
	this->type = DATAJSONTYPE_FLOAT;
	this->jFloat = value;
}

// 真偽値設定
void dataJsonSetBool(struct dataJsonValue *this, bool value){
	this->type = DATAJSONTYPE_BOOL;
	this->jBool = value;
}

// 文字列設定
void dataJsonSetString(struct dataJsonValue *this, char *value){
	uint32_t length = (uint32_t)strlen(value);
	char *buff = (char*)malloc((length + 1) * sizeof(char));
	strcpy(buff, this->jString);
	this->type = DATAJSONTYPE_STRING;
	this->jString = buff;
}

// 連想配列要素作成
static struct dataJsonArray *createObject(char *key){
	uint32_t length = (uint32_t)strlen(key);
	struct dataJsonArray *array = (struct dataJsonArray*)malloc(sizeof(struct dataJsonArray) + length);
	strcpy(array->key, key);
	array->value.type = DATAJSONTYPE_NULL;
	array->next = NULL;
	return array;
}

// 連想配列に要素を作成して追加
struct dataJsonValue *dataJsonObjectCreateValue(struct dataJsonValue *this, char *key){
	if(this->type == DATAJSONTYPE_NULL){
		this->type = DATAJSONTYPE_OBJECT;
		this->jArray = NULL;
	}else if(this->type != DATAJSONTYPE_OBJECT){return NULL;}

	if(this->jArray == NULL){
		this->jArray = createObject(key);
		return &this->jArray->value;
	}else{
		struct dataJsonArray *temp = this->jArray;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = createObject(key);
		return &temp->next->value;
	}
}

// 配列要素を作成
static struct dataJsonArray *createArray(){
	struct dataJsonArray *array = (struct dataJsonArray*)malloc(sizeof(struct dataJsonArray));
	array->key[0] = '\0';
	array->value.type = DATAJSONTYPE_NULL;
	array->next = NULL;
	return array;
}

// 配列に要素を作成して追加
struct dataJsonValue *dataJsonArrayCreateValue(struct dataJsonValue *this){
	if(this->type == DATAJSONTYPE_NULL){
		this->type = DATAJSONTYPE_ARRAY;
		this->jArray = NULL;
	}else if(this->type != DATAJSONTYPE_ARRAY){return NULL;}

	if(this->jArray == NULL){
		this->jArray = createArray();
		return &this->jArray->value;
	}else{
		struct dataJsonArray *temp = this->jArray;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = createArray();
		return &temp->next->value;
	}
}

// ----------------------------------------------------------------

// 整数読み取り
int64_t dataJsonGetInt(struct dataJsonValue *this, int64_t defaultValue){
	if(this != NULL){
		if(this->type == DATAJSONTYPE_INT){return this->jInt;}
		if(this->type == DATAJSONTYPE_FLOAT){return (int64_t)this->jFloat;}
	}
	return defaultValue;
}

// 浮動小数読み取り
double dataJsonGetFloat(struct dataJsonValue *this, double defaultValue){
	if(this != NULL){
		if(this->type == DATAJSONTYPE_INT){return (double)this->jInt;}
		if(this->type == DATAJSONTYPE_FLOAT){return this->jFloat;}
	}
	return defaultValue;
}

// 真偽値読み取り
bool dataJsonGetBool(struct dataJsonValue *this, bool defaultValue){
	return (this != NULL && this->type == DATAJSONTYPE_BOOL) ? this->jBool : defaultValue;
}

// 文字列読み取り
char *dataJsonGetString(struct dataJsonValue *this, char *defaultValue){
	return (this != NULL && this->type == DATAJSONTYPE_STRING) ? this->jString : defaultValue;
}

// 連想配列から要素取得
struct dataJsonValue *dataJsonObjectGetValue(struct dataJsonValue *this, char *key){
	if(this == NULL || this->type != DATAJSONTYPE_OBJECT){return NULL;}
	struct dataJsonArray *temp = this->jArray;
	struct dataJsonValue *value = NULL;
	while(temp != NULL){
		if(strcmp(temp->key, key) == 0){value = &temp->value;}
		temp = temp->next;
	}
	return value;
}

// 配列から要素取得
struct dataJsonValue *dataJsonArrayGetValue(struct dataJsonValue *this, uint32_t index){
	if(this == NULL || this->type != DATAJSONTYPE_ARRAY){return NULL;}
	uint32_t length = 0;
	struct dataJsonArray *temp = this->jArray;
	struct dataJsonValue *value = NULL;
	while(temp != NULL){
		if(index == length++){value = &temp->value; break;}
		temp = temp->next;
	}
	return value;
}

// 配列の長さ取得
uint32_t dataJsonArrayGetLength(struct dataJsonValue *this){
	uint32_t length = 0;
	if(this != NULL && (this->type == DATAJSONTYPE_OBJECT || this->type == DATAJSONTYPE_ARRAY)){
		struct dataJsonArray *temp = this->jArray;
		while(temp != NULL){
			length++;
			temp = temp->next;
		}
	}
	return length;
}

// ----------------------------------------------------------------

// 連想配列から要素削除
void dataJsonObjectRemoveValue(struct dataJsonValue *this, char *key){
	if(this == NULL || this->type != DATAJSONTYPE_OBJECT){return;}
	struct dataJsonArray *prev = NULL;
	struct dataJsonArray *temp = this->jArray;
	struct dataJsonArray *dispose = NULL;
	while(temp != NULL){
		if(strcmp(temp->key, key) == 0){
			// リストから要素を外す
			dispose = temp;
			temp = temp->next;
			if(prev == NULL){this->jArray = temp;}
			else{prev->next = temp;}
			// 要素の除去
			dataJsonFree(&dispose->value);
			free(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 配列から要素削除
void dataJsonArrayRemoveValue(struct dataJsonValue *this, uint32_t index){
	if(this == NULL || this->type != DATAJSONTYPE_ARRAY){return;}
	uint32_t length = 0;
	struct dataJsonArray *prev = NULL;
	struct dataJsonArray *temp = this->jArray;
	struct dataJsonArray *dispose = NULL;
	while(temp != NULL){
		if(index == length++){
			// リストから要素を外す
			dispose = temp;
			temp = temp->next;
			if(prev == NULL){this->jArray = temp;}
			else{prev->next = temp;}
			// 要素の除去
			dataJsonFree(&dispose->value);
			free(dispose);
			return;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// json解放
void dataJsonFree(struct dataJsonValue *this){
	if(this == NULL){return;}
	switch(this->type){
		case DATAJSONTYPE_STRING:
			free(this->jString);
			break;
		case DATAJSONTYPE_OBJECT:
		case DATAJSONTYPE_ARRAY:
			{
				struct dataJsonArray *temp = this->jArray;
				while(temp != NULL){
					struct dataJsonArray *dispose = temp;
					temp = temp->next;
					dataJsonFree(&dispose->value);
					free(dispose);
				}
				this->jArray = NULL;
			}
			break;
		default: break;
	}
	this->type = DATAJSONTYPE_NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

