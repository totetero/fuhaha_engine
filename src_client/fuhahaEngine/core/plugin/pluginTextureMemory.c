#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	GLuint texData;	
} localGlobal = {0};

// ----------------------------------------------------------------

// テクスチャバインド状態の記録と重複確認
bool corePluginTextureIsBind(GLuint glId){
	if(localGlobal.texData == glId){return true;}
	localGlobal.texData = glId;
	return false;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

