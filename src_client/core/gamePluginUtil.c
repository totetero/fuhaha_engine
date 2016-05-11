#include "engine/engine.h"
#include "gamePluginUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// 揮発性一時バッファ
	struct{
		void *buff;
		size_t size;
	} temporary;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 揮発性一時バッファ 返値領域は解放禁止
void *corePluginUtilTemporaryBuffer(size_t size){
	if(localGlobal.temporary.size < size){
		if(localGlobal.temporary.size > 0){free(localGlobal.temporary.buff);}
		localGlobal.temporary.size = size;
		localGlobal.temporary.buff = malloc(localGlobal.temporary.size);
	}
	return localGlobal.temporary.buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// url取得 返値文字列は揮発性バッファで解放禁止
char *gamePluginUtilUrlGet(){
	char *value = "http://totetero.com/cgi-bin/php";
	char *buff = (char*)corePluginUtilTemporaryBuffer(strlen(value) + 1);
	strcpy(buff, value);
	return buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

