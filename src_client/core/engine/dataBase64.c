#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static char localGlobal_enc[0x41] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char localGlobal_dec[0x80] = "";

// base64形式に変換 引数非破壊 返値は解放必要
char *dataBase64encode(uint8_t *data, uint32_t length){
	uint32_t rawLen = length;
	uint32_t b64Len = ((rawLen + 2) / 3) * 4;
	uint8_t *buff = (uint8_t*)malloc((b64Len + 1) * sizeof(uint8_t));

	// base64エンコード
	uint32_t x = 0;
	uint32_t i = 0;
	uint32_t j = 0;
	while(i < rawLen){
		x = (x << 8) | data[i++];
		switch(i % 3){
			case 1: buff[j++] = localGlobal_enc[(x >> 2) & 0x3f]; break;
			case 2: buff[j++] = localGlobal_enc[(x >> 4) & 0x3f]; break;
			case 0: buff[j++] = localGlobal_enc[(x >> 6) & 0x3f]; buff[j++] = localGlobal_enc[x & 0x3f]; break;
		}
	}
	switch(i % 3){
		case 1: buff[j++] = localGlobal_enc[(x << 4) & 0x3f]; buff[j++] = localGlobal_enc[0x40]; buff[j++] = localGlobal_enc[0x40]; break;
		case 2: buff[j++] = localGlobal_enc[(x << 2) & 0x3f]; buff[j++] = localGlobal_enc[0x40]; break;
		case 0: break;
	}
	buff[j++] = '\0';

	return (char*)buff;
}

// base64形式から解読 引数破壊 返値は解放不要
uint8_t *dataBase64decode(char *data, uint32_t *length){
	uint32_t b64Len = (data == NULL) ? 0 : (uint32_t)strlen(data);

	uint32_t rawLen = (b64Len / 4) * 3;
	if(rawLen > 0 && data[b64Len - 1] == localGlobal_enc[0x40]){rawLen -= 1;}
	if(rawLen > 0 && data[b64Len - 2] == localGlobal_enc[0x40]){rawLen -= 1;}
	if(length != NULL){*length = rawLen;}

	// データが存在しなかった場合
	if(rawLen <= 0){return NULL;}

	uint8_t *buff = (uint8_t*)data;

	// デコードテーブル作成
	for(uint32_t i = 0; i < 0x41; i++){localGlobal_dec[localGlobal_enc[i]] = i % 0x40;}

	// base64デコード
	for(uint32_t i = 0, j = 0; i < rawLen; i++){
		switch(i % 3){
			case 0: buff[i] = localGlobal_dec[data[j + 0]] << 2 | localGlobal_dec[data[j + 1]] >> 4; break;
			case 1: buff[i] = localGlobal_dec[data[j + 1]] << 4 | localGlobal_dec[data[j + 2]] >> 2; break;
			case 2: buff[i] = localGlobal_dec[data[j + 2]] << 6 | localGlobal_dec[data[j + 3]]; j += 4; break;
		}
	}

	return buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

