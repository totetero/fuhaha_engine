#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../engineUtil/engineUtil.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3DオブジェクトVBO構造体
struct engineGraphicObjectVBOCage{
	GLuint glId;
	int length;
	GLfloat *vertices;
};

// 3DオブジェクトIBO構造体
struct engineGraphicObjectIBOCage{
	GLuint glId;
	int length;
	GLushort *indexes;
};

// 3DオブジェクトTex構造体
struct engineGraphicObjectTexCage{
	struct engineGraphicObjectTexData *data;
	enum engineGraphicObjectTexType type;
};

// テクスチャ引数構造体
struct engineGraphicObjectTexArg{
	enum engineGraphicObjectTexArgType{
		ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL,
		ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT,
		ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST,
	} type;
	union{
		struct{char *src;} pluginTextureLocal;
		struct{enum pluginTextureFontSetId fontSetId; char *letterList;} pluginTextureFont;
		struct{} pluginTest;
	};
};

// テクスチャ情報構造体
struct engineGraphicObjectTexData{
	struct engineGraphicObjectTexData *next;
	struct engineGraphicObjectTexArg arg;
	union{
		struct{GLuint glId; int imgw; int imgh;} pluginTextureLocal;
		struct{int codeListIndex; int codeListLength;} pluginTextureFont;
		struct{} pluginTest;
	};
	enum engineGraphicObjectTexDataStatus{
		ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING,
		ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED,
		ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL,
	} status;
};

static struct{
	// デフォルトテクスチャ
	struct{
		GLuint glId;
		int imgw;
		int imgh;
	} defaultTexture;
	// 3Dオブジェクトリスト
	struct engineGraphicObjectVBOCage *egoVBOList;
	struct engineGraphicObjectIBOCage *egoIBOList;
	struct engineGraphicObjectTexCage *egoTexList;
	struct engineGraphicObjectTexData *texDataList;
	int egoVBOLength;
	int egoIBOLength;
	int egoTexLength;
} localGlobal = {0};

// ----------------------------------------------------------------

static int egoVBOId2Index(engineGraphicObjectVBOId egoId){return (int)(egoId - 1);}
static int egoIBOId2Index(engineGraphicObjectIBOId egoId){return (int)(egoId - 1);}
static int egoTexId2Index(engineGraphicObjectTexId egoId){return (int)(egoId - 1);}
static engineGraphicObjectVBOId egoVBOIndex2Id(int egoIndex){return (engineGraphicObjectVBOId)(egoIndex + 1);}
static engineGraphicObjectIBOId egoIBOIndex2Id(int egoIndex){return (engineGraphicObjectIBOId)(egoIndex + 1);}
static engineGraphicObjectTexId egoTexIndex2Id(int egoIndex){return (engineGraphicObjectTexId)(egoIndex + 1);}

// ----------------------------------------------------------------

// 3DオブジェクトVBO解放
static void egoVBOFree(struct engineGraphicObjectVBOCage *this){
	if(this->vertices == NULL){return;}
	glDeleteBuffers(1, &this->glId);
	engineUtilMemoryInfoFree("engineGraphicObject vbo2", this->vertices);
	this->vertices = NULL;
}

// 3DオブジェクトIBO解放
static void egoIBOFree(struct engineGraphicObjectIBOCage *this){
	if(this->indexes == NULL){return;}
	glDeleteBuffers(1, &this->glId);
	engineUtilMemoryInfoFree("engineGraphicObject ibo2", this->indexes);
	this->indexes = NULL;
}

// テクスチャ情報解放
static void texDataFree(struct engineGraphicObjectTexData *this){
	if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
		if(this->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED){
			// 解放
			if(this->pluginTextureLocal.glId != localGlobal.defaultTexture.glId){glDeleteTextures(1, &this->pluginTextureLocal.glId);}
			engineUtilMemoryInfoFree("engineGraphicObject tex3", this->arg.pluginTextureLocal.src);
			this->arg.pluginTextureLocal.src = NULL;
			engineUtilMemoryInfoFree("engineGraphicObject tex2", this);
		}else{
			// ロードが完了していないのでコールバックで破棄
			this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL;
		}
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
		if(this->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED){
			// 解放
			platformPluginTextureFontDispose(this->pluginTextureFont.codeListIndex);
			engineUtilMemoryInfoFree("engineGraphicObject tex3", this->arg.pluginTextureFont.letterList);
			this->arg.pluginTextureFont.letterList = NULL;
			engineUtilMemoryInfoFree("engineGraphicObject tex2", this);
		}else{
			// ロードが完了していないのでコールバックで破棄
			this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL;
		}
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
		// 解放
		engineUtilMemoryInfoFree("engineGraphicObject tex2", this);
	}
}

// 3DオブジェクトTex解放
static void egoTexFree(struct engineGraphicObjectTexCage *this){
	if(this->data == NULL){return;}
	this->data = NULL;
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void texDataLocalCallback(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS){
	struct engineGraphicObjectTexData *this = (struct engineGraphicObjectTexData*)param;
	enum engineGraphicObjectTexDataStatus beforeStatus = this->status;
	this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED;
	if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING){
		// ロード完了
		this->pluginTextureLocal.glId = glId;
		this->pluginTextureLocal.imgw = imgw;
		this->pluginTextureLocal.imgh = imgh;
	}else if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL){
		// ロード中止
		texDataFree(this);
	}
}

// ロード完了時コールバック
static void texDataFontCallback(void *param, PLUGINTEXTURE_FONT_CALLBACKPARAMS){
	struct engineGraphicObjectTexData *this = (struct engineGraphicObjectTexData*)param;
	enum engineGraphicObjectTexDataStatus beforeStatus = this->status;
	this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED;
	if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING){
		// ロード完了
		this->pluginTextureFont.codeListIndex = codeListIndex;
		this->pluginTextureFont.codeListLength = codeListLength;
	}else if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL){
		// ロード中止
		texDataFree(this);
	}
}

// ----------------------------------------------------------------

// VBO作成
static void egoVBOLoad(struct engineGraphicObjectVBOCage *this){
	if(this->vertices == NULL){return;}
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->length, this->vertices, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetVBO();
}

// IBO作成
static void egoIBOLoad(struct engineGraphicObjectIBOCage *this){
	if(this->indexes == NULL){return;}
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->length, this->indexes, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetIBO();
}

// テクスチャ作成
static void texDataLoad(struct engineGraphicObjectTexData *this){
	if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
		// 読み込み中のデフォルトテクスチャ設定
		this->pluginTextureLocal.glId = localGlobal.defaultTexture.glId;
		this->pluginTextureLocal.imgw = localGlobal.defaultTexture.imgw;
		this->pluginTextureLocal.imgh = localGlobal.defaultTexture.imgh;
		// テクスチャロード
		this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING;
		platformPluginTextureLocal(this, this->arg.pluginTextureLocal.src, texDataLocalCallback);
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
		// フォント未ロード値設定
		this->pluginTextureFont.codeListIndex = -1;
		this->pluginTextureFont.codeListLength = 0;
		// テクスチャロード
		this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING;
		platformPluginTextureFont(this, this->arg.pluginTextureFont.fontSetId, this->arg.pluginTextureFont.letterList, texDataFontCallback);
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
	}
}

// ----------------------------------------------------------------

// 3DオブジェクトTex読み込み完了確認
bool engineGraphicObjectTexIsComplete(engineGraphicObjectTexId egoId){
	int egoIndex = egoTexId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoTexLength <= egoIndex){return true;}
	struct engineGraphicObjectTexCage *temp = &localGlobal.egoTexList[egoIndex];
	return (temp->data != NULL && temp->data->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED);
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO作成
engineGraphicObjectVBOId engineGraphicObjectVBOCreate(int length, GLfloat *vertices){
	int egoIndex = -1;
	// 領域リストに空きがないか確認する
	for(int i = 0; i < localGlobal.egoVBOLength; i++){if(localGlobal.egoVBOList[i].vertices == NULL){egoIndex =  i;}}
	if(egoIndex < 0){
		// 空きがなければ領域リストを拡張する
		egoIndex = localGlobal.egoVBOLength;
		int egoVBOLength = localGlobal.egoVBOLength + 10;
		struct engineGraphicObjectVBOCage *egoVBOList = (struct engineGraphicObjectVBOCage*)engineUtilMemoryInfoCalloc("(permanent) engineGraphicObject vbo1", egoVBOLength, sizeof(struct engineGraphicObjectVBOCage));
		if(localGlobal.egoVBOLength > 0){
			memcpy(egoVBOList, localGlobal.egoVBOList, localGlobal.egoVBOLength * sizeof(struct engineGraphicObjectVBOCage));
			engineUtilMemoryInfoFree("(permanent) engineGraphicObject vbo1", localGlobal.egoVBOList);
		}
		localGlobal.egoVBOLength = egoVBOLength;
		localGlobal.egoVBOList = egoVBOList;
	}

	// データ作成
	struct engineGraphicObjectVBOCage *obj = &localGlobal.egoVBOList[egoIndex];
	obj->length = length;
	size_t size = length * sizeof(GLfloat);
	obj->vertices = (GLfloat*)engineUtilMemoryInfoMalloc("engineGraphicObject vbo2", size);
	memcpy(obj->vertices, vertices, size);
	//for(int i = 0; i < length; i++){obj->vertices[i] = (GLfloat)vertices[i];}
	egoVBOLoad(obj);

	return egoVBOIndex2Id(egoIndex);
}

// 3DオブジェクトIBO作成
engineGraphicObjectIBOId engineGraphicObjectIBOCreate(int length, GLushort *indexes){
	int egoIndex = -1;
	// 領域リストに空きがないか確認する
	for(int i = 0; i < localGlobal.egoIBOLength; i++){if(localGlobal.egoIBOList[i].indexes == NULL){egoIndex =  i;}}
	if(egoIndex < 0){
		// 空きがなければ領域リストを拡張する
		egoIndex = localGlobal.egoIBOLength;
		int egoIBOLength = localGlobal.egoIBOLength + 10;
		struct engineGraphicObjectIBOCage *egoIBOList = (struct engineGraphicObjectIBOCage*)engineUtilMemoryInfoCalloc("(permanent) engineGraphicObject ibo1", egoIBOLength, sizeof(struct engineGraphicObjectIBOCage));
		if(localGlobal.egoIBOLength > 0){
			memcpy(egoIBOList, localGlobal.egoIBOList, localGlobal.egoIBOLength * sizeof(struct engineGraphicObjectIBOCage));
			engineUtilMemoryInfoFree("(permanent) engineGraphicObject ibo1", localGlobal.egoIBOList);
		}
		localGlobal.egoIBOLength = egoIBOLength;
		localGlobal.egoIBOList = egoIBOList;
	}

	// データ作成
	struct engineGraphicObjectIBOCage *obj = &localGlobal.egoIBOList[egoIndex];
	obj->length = length;
	size_t size = length * sizeof(GLushort);
	obj->indexes = (GLushort*)engineUtilMemoryInfoMalloc("engineGraphicObject ibo2", size);
	memcpy(obj->indexes, indexes, size);
	//for(int i = 0; i < length; i++){obj->indexes[i] = (GLushort)indexes[i];}
	egoIBOLoad(obj);

	return egoIBOIndex2Id(egoIndex);
}

// テクスチャ情報作成
static struct engineGraphicObjectTexData *texDataCreate(struct engineGraphicObjectTexArg *arg){
	// 重複確認
	struct engineGraphicObjectTexData *temp = localGlobal.texDataList;
	while(temp != NULL){
		if(arg->type == temp->arg.type){
			if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
				if(strcmp(temp->arg.pluginTextureLocal.src, arg->pluginTextureLocal.src) == 0){return temp;}
			}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
				bool isSame = false;
				if(!isSame && temp->arg.pluginTextureFont.fontSetId == arg->pluginTextureFont.fontSetId){isSame = true;}
				if(!isSame && strcmp(temp->arg.pluginTextureFont.letterList, arg->pluginTextureFont.letterList) == 0){isSame = true;}
				if(isSame){return temp;}
			}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
				return temp;
			}
		}
		temp = temp->next;
	}
	// 重複がなければ新規作成
	struct engineGraphicObjectTexData *obj = (struct engineGraphicObjectTexData*)engineUtilMemoryInfoCalloc("engineGraphicObject tex2", 1, sizeof(struct engineGraphicObjectTexData));
	obj->arg.type = arg->type;
	if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
		obj->arg.pluginTextureLocal.src = engineUtilMemoryInfoStrdup("engineGraphicObject tex3", arg->pluginTextureLocal.src);
	}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
		obj->arg.pluginTextureFont.fontSetId = arg->pluginTextureFont.fontSetId;
		obj->arg.pluginTextureFont.letterList = engineUtilMemoryInfoStrdup("engineGraphicObject tex3", arg->pluginTextureFont.letterList);
	}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
	}
	texDataLoad(obj);
	// リスト登録
	if(localGlobal.texDataList == NULL){
		localGlobal.texDataList = obj;
	}else{
		struct engineGraphicObjectTexData *temp = localGlobal.texDataList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj;
}

// 3DオブジェクトTex作成
static engineGraphicObjectTexId texCageCreate(struct engineGraphicObjectTexArg *arg, enum engineGraphicObjectTexType type){
	int egoIndex = -1;
	// 領域リストに空きがないか確認する
	for(int i = 0; i < localGlobal.egoTexLength; i++){if(localGlobal.egoTexList[i].data == NULL){egoIndex =  i;}}
	if(egoIndex < 0){
		// 空きがなければ領域リストを拡張する
		egoIndex = localGlobal.egoTexLength;
		int egoTexLength = localGlobal.egoTexLength + 10;
		struct engineGraphicObjectTexCage *egoTexList = (struct engineGraphicObjectTexCage*)engineUtilMemoryInfoCalloc("(permanent) engineGraphicObject tex1", egoTexLength, sizeof(struct engineGraphicObjectTexCage));
		if(localGlobal.egoTexLength > 0){
			memcpy(egoTexList, localGlobal.egoTexList, localGlobal.egoTexLength * sizeof(struct engineGraphicObjectTexCage));
			engineUtilMemoryInfoFree("(permanent) engineGraphicObject tex1", localGlobal.egoTexList);
		}
		localGlobal.egoTexLength = egoTexLength;
		localGlobal.egoTexList = egoTexList;
	}

	// データ作成
	struct engineGraphicObjectTexCage *obj = &localGlobal.egoTexList[egoIndex];
	obj->data = texDataCreate(arg);
	obj->type = type;

	return egoTexIndex2Id(egoIndex);
}

// 3DオブジェクトTex作成
engineGraphicObjectTexId engineGraphicObjectTexCreateLocal(char *src, enum engineGraphicObjectTexType type){
	struct engineGraphicObjectTexArg arg;
	arg.type = ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL;
	arg.pluginTextureLocal.src = src;
	return texCageCreate(&arg, type);
}

// 3DオブジェクトTex作成
engineGraphicObjectTexId engineGraphicObjectTexCreateFont(enum pluginTextureFontSetId fontSetId, char *letterList, enum engineGraphicObjectTexType type){
	struct engineGraphicObjectTexArg arg;
	arg.type = ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT;
	arg.pluginTextureFont.fontSetId = fontSetId;
	arg.pluginTextureFont.letterList = letterList;
	return texCageCreate(&arg, type);
}

// ----------------------------------------------------------------

// VBOID取得
bool engineGraphicObjectVBOGetGLId(engineGraphicObjectVBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	int egoIndex = egoVBOId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoVBOLength <= egoIndex){return false;}
	struct engineGraphicObjectVBOCage *temp = &localGlobal.egoVBOList[egoIndex];
	if(temp->vertices == NULL){return false;}
	*glId = temp->glId;
	return true;
}

// IBOID取得
bool engineGraphicObjectIBOGetGLId(engineGraphicObjectIBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	int egoIndex = egoIBOId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoIBOLength <= egoIndex){return false;}
	struct engineGraphicObjectIBOCage *temp = &localGlobal.egoIBOList[egoIndex];
	if(temp->indexes == NULL){return false;}
	*glId = temp->glId;
	return true;
}

// テクスチャID取得
bool engineGraphicObjectTexGetGLId(engineGraphicObjectTexId egoId, GLuint *glId, enum engineGraphicObjectTexType *type){
	if(glId == NULL && type == NULL){return false;}
	int egoIndex = egoTexId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoTexLength <= egoIndex){return false;}
	struct engineGraphicObjectTexCage *temp = &localGlobal.egoTexList[egoIndex];
	if(temp->data == NULL){return false;}
	if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){*glId = temp->data->pluginTextureLocal.glId;}
	if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){*glId = localGlobal.defaultTexture.glId;}
	if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){*glId = localGlobal.defaultTexture.glId;}
	if(type != NULL){*type = temp->type;}
	return true;
}

// テクスチャID取得
bool engineGraphicObjectTexGetCodeList(engineGraphicObjectTexId egoId, int *codeListIndex, int *codeListLength, enum engineGraphicObjectTexType *type){
	if(codeListIndex == NULL && codeListLength == NULL && type == NULL){return false;}
	int egoIndex = egoTexId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoTexLength <= egoIndex){return false;}
	struct engineGraphicObjectTexCage *temp = &localGlobal.egoTexList[egoIndex];
	if(temp->data == NULL){return false;}
	if(temp->data->arg.type != ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){return false;}
	if(codeListIndex != NULL){*codeListIndex = temp->data->pluginTextureFont.codeListIndex;}
	if(codeListLength != NULL){*codeListLength = temp->data->pluginTextureFont.codeListLength;}
	if(type != NULL){*type = temp->type;}
	return true;
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO除去
void engineGraphicObjectVBODispose(engineGraphicObjectVBOId egoId){
	int egoIndex = egoVBOId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoVBOLength <= egoIndex){return;}
	egoVBOFree(&localGlobal.egoVBOList[egoIndex]);
}

// 3DオブジェクトIBO除去
void engineGraphicObjectIBODispose(engineGraphicObjectIBOId egoId){
	int egoIndex = egoIBOId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoIBOLength <= egoIndex){return;}
	egoIBOFree(&localGlobal.egoIBOList[egoIndex]);
}

// テクスチャ情報除去
static void texDataDispose(struct engineGraphicObjectTexData *this){
	// 使用中確認
	for(int i = 0; i < localGlobal.egoTexLength; i++){if(localGlobal.egoTexList[i].data == this){return;}}
	// 誰も使っていなければ除去
	struct engineGraphicObjectTexData *prevData = NULL;
	struct engineGraphicObjectTexData *tempData = localGlobal.texDataList;
	while(tempData != NULL){
		if(tempData == this){
			// リストから要素を外す
			tempData = tempData->next;
			if(prevData == NULL){localGlobal.texDataList = tempData;}
			else{prevData->next = tempData;}
		}else{
			prevData = tempData;
			tempData = tempData->next;
		}
	}
	// 除去
	texDataFree(this);
}

// 3DオブジェクトTex除去
void engineGraphicObjectTexDispose(engineGraphicObjectTexId egoId){
	int egoIndex = egoTexId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoTexLength <= egoIndex){return;}
	struct engineGraphicObjectTexCage *cage = &localGlobal.egoTexList[egoIndex];
	struct engineGraphicObjectTexData *data = cage->data;
	egoTexFree(cage);
	texDataDispose(data);
}

// 全3Dオブジェクト除去
void engineGraphicObjectDispose(void){
	for(int i = 0; i < localGlobal.egoVBOLength; i++){egoVBOFree(&localGlobal.egoVBOList[i]);}
	for(int i = 0; i < localGlobal.egoIBOLength; i++){egoIBOFree(&localGlobal.egoIBOList[i]);}
	for(int i = 0; i < localGlobal.egoTexLength; i++){egoTexFree(&localGlobal.egoTexList[i]);}
	engineUtilMemoryInfoFree("(permanent) engineGraphicObject vbo1", localGlobal.egoVBOList);
	engineUtilMemoryInfoFree("(permanent) engineGraphicObject ibo1", localGlobal.egoIBOList);
	engineUtilMemoryInfoFree("(permanent) engineGraphicObject tex1", localGlobal.egoTexList);
	localGlobal.egoVBOList = NULL;
	localGlobal.egoIBOList = NULL;
	localGlobal.egoTexList = NULL;
	localGlobal.egoVBOLength = 0;
	localGlobal.egoIBOLength = 0;
	localGlobal.egoTexLength = 0;

	struct engineGraphicObjectTexData *tempData = localGlobal.texDataList;
	while(tempData != NULL){
		struct engineGraphicObjectTexData *dispose = tempData;
		tempData = tempData->next;
		// 要素の除去
		texDataFree(dispose);
	}
	localGlobal.texDataList = NULL;

	// デフォルトテクスチャ除去
	glDeleteTextures(1, &localGlobal.defaultTexture.glId);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 全データロード再読み込み
void engineGraphicObjectReload(void){
	for(int i = 0; i < localGlobal.egoVBOLength; i++){egoVBOLoad(&localGlobal.egoVBOList[i]);}
	for(int i = 0; i < localGlobal.egoIBOLength; i++){egoIBOLoad(&localGlobal.egoIBOList[i]);}

	struct engineGraphicObjectTexData *tempTex = localGlobal.texDataList;
	while(tempTex != NULL){texDataLoad(tempTex); tempTex = tempTex->next;}

	// 読み込み中に使うデフォルトテクスチャ作成
	byte data[4] = {0xff, 0xff, 0xff, 0xff};
	localGlobal.defaultTexture.imgw = 1;
	localGlobal.defaultTexture.imgh = 1;
	glGenTextures(1, &localGlobal.defaultTexture.glId);
	corePluginTextureIsBind(localGlobal.defaultTexture.glId);
	glBindTexture(GL_TEXTURE_2D, localGlobal.defaultTexture.glId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, localGlobal.defaultTexture.imgw, localGlobal.defaultTexture.imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

