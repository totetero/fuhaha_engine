#include "../../library.h"
#include "platform.h"
#include "../../plugin/pluginTexture.h"
#include "../engineMath/engineMath.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3DオブジェクトVBO構造体
struct engineGraphicObjectVBO{
	struct engineGraphicObjectVBO *next;
	engineGraphicObjectVBOId egoId;
	// VBOデータ
	GLuint glId;
	uint32_t size;
	GLfloat *vertices;
};

// 3DオブジェクトIBO構造体
struct engineGraphicObjectIBO{
	struct engineGraphicObjectIBO *next;
	engineGraphicObjectIBOId egoId;
	// IBOデータ
	GLuint glId;
	uint32_t size;
	GLushort *indexes;
};

// 3DオブジェクトTex構造体
struct engineGraphicObjectTex{
	struct engineGraphicObjectTex *next;
	engineGraphicObjectTexId egoId;
	// テクスチャデータ
	struct engineGraphicObjectTexData *data;
	enum engineGraphicObjectTexType type;
};

// テクスチャ情報構造体
struct engineGraphicObjectTexData{
	struct engineGraphicObjectTexData *next;
	GLuint glId;
	char *src;
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
	} defaultTexture;
	// 3Dオブジェクトリスト
	uint32_t egoIdCount;
	struct engineGraphicObjectVBO *egoVBOList;
	struct engineGraphicObjectIBO *egoIBOList;
	struct engineGraphicObjectTex *egoTexList;
	struct engineGraphicObjectTexData *texDataList;
} localGlobal = {0};

// ----------------------------------------------------------------

// 3DオブジェクトVBO解放
static void egoVBOFree(struct engineGraphicObjectVBO *this){
	glDeleteBuffers(1, &this->glId);
	free(this->vertices);
	free(this);
}

// 3DオブジェクトIBO解放
static void egoIBOFree(struct engineGraphicObjectIBO *this){
	glDeleteBuffers(1, &this->glId);
	free(this->indexes);
	free(this);
}

// テクスチャ情報解放
static void texDataFree(struct engineGraphicObjectTexData *this){
	if(this->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED){
		// 解放
		if(this->glId != localGlobal.defaultTexture.glId){glDeleteTextures(1, &this->glId);}
		free(this->src);
		free(this);
	}else{
		// ロードが完了していないのでコールバックで破棄
		this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL;
	}
}

// 3DオブジェクトTex解放
static void egoTexFree(struct engineGraphicObjectTex *this){
	free(this);
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void texDataLocalCallback(void *param, int glId, int texw, int texh, int imgw, int imgh){
	struct engineGraphicObjectTexData *this = (struct engineGraphicObjectTexData*)param;
	enum engineGraphicObjectTexDataStatus beforeStatus = this->status;
	this->glId = glId;
	this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED;

	// ロード中止時の解放処理
	if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL){texDataFree(this);}
}

// ----------------------------------------------------------------

// VBO作成
static void egoVBOLoad(struct engineGraphicObjectVBO *this){
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->size, this->vertices, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetVBO();
}

// IBO作成
static void egoIBOLoad(struct engineGraphicObjectIBO *this){
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->size, this->indexes, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetIBO();
}

// テクスチャ作成
static void texDataLoad(struct engineGraphicObjectTexData *this){
	// 読み込み中のデフォルトテクスチャ設定
	this->glId = localGlobal.defaultTexture.glId;
	// テクスチャロード
	this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING;
	platformPluginTextureLocal(this, this->src, texDataLocalCallback);
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO作成
engineGraphicObjectVBOId engineGraphicObjectVBOCreate(uint32_t size, double *vertices){
	// データ作成
	struct engineGraphicObjectVBO *obj = (struct engineGraphicObjectVBO*)calloc(1, sizeof(struct engineGraphicObjectVBO));
	obj->egoId = ++localGlobal.egoIdCount;
	obj->size = size;
	uint32_t memsize = size * sizeof(GLfloat);
	obj->vertices = (GLfloat*)malloc(memsize);
	for(uint32_t i = 0; i < size; i++){obj->vertices[i] = (GLfloat)vertices[i];}
	egoVBOLoad(obj);
	// リスト登録
	if(localGlobal.egoVBOList == NULL){
		localGlobal.egoVBOList = obj;
	}else{
		struct engineGraphicObjectVBO *temp = localGlobal.egoVBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->egoId;
}

// 3DオブジェクトIBO作成
engineGraphicObjectIBOId engineGraphicObjectIBOCreate(uint32_t size, uint16_t *indexes){
	// データ作成
	struct engineGraphicObjectIBO *obj = (struct engineGraphicObjectIBO*)calloc(1, sizeof(struct engineGraphicObjectIBO));
	obj->egoId = ++localGlobal.egoIdCount;
	obj->size = size;
	uint32_t memsize = size * sizeof(GLushort);
	obj->indexes = (GLushort*)malloc(memsize);
	for(uint32_t i = 0; i < size; i++){obj->indexes[i] = (GLushort)indexes[i];}
	egoIBOLoad(obj);
	// リスト登録
	if(localGlobal.egoIBOList == NULL){
		localGlobal.egoIBOList = obj;
	}else{
		struct engineGraphicObjectIBO *temp = localGlobal.egoIBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->egoId;
}

// テクスチャ情報作成
static struct engineGraphicObjectTexData *texDataCreate(char *src){
	// 重複確認
	struct engineGraphicObjectTexData *temp = localGlobal.texDataList;
	while(temp != NULL){
		if(strcmp(temp->src, src) == 0){return temp;}
		temp = temp->next;
	}
	// 重複がなければ新規作成
	struct engineGraphicObjectTexData *obj = (struct engineGraphicObjectTexData*)calloc(1, sizeof(struct engineGraphicObjectTexData));
	uint32_t memsize = ((uint32_t)strlen(src) + 1) * sizeof(char);
	obj->src = (char*)malloc(memsize);
	memmove(obj->src, src, memsize);
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
engineGraphicObjectTexId engineGraphicObjectTexCreate(char *src, enum engineGraphicObjectTexType type){
	// データ作成
	struct engineGraphicObjectTex *obj = (struct engineGraphicObjectTex*)calloc(1, sizeof(struct engineGraphicObjectTex));
	obj->egoId = ++localGlobal.egoIdCount;
	obj->data = texDataCreate(src);
	obj->type = type;
	// リスト登録
	if(localGlobal.egoTexList == NULL){
		localGlobal.egoTexList = obj;
	}else{
		struct engineGraphicObjectTex *temp = localGlobal.egoTexList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->egoId;
}

// ----------------------------------------------------------------

// VBOID取得
bool engineGraphicObjectVBOGetGLId(engineGraphicObjectVBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	struct engineGraphicObjectVBO *temp = localGlobal.egoVBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			*glId = temp->glId;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// IBOID取得
bool engineGraphicObjectIBOGetGLId(engineGraphicObjectIBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	struct engineGraphicObjectIBO *temp = localGlobal.egoIBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			*glId = temp->glId;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// テクスチャID取得
bool engineGraphicObjectTexGetGLId(engineGraphicObjectTexId egoId, GLuint *glId, enum engineGraphicObjectTexType *type){
	if(glId == NULL && type == NULL){return false;}
	struct engineGraphicObjectTex *temp = localGlobal.egoTexList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			if(temp->data == NULL){return false;}
			if(glId != NULL){*glId = temp->data->glId;}
			if(type != NULL){*type = temp->type;}
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO除去
void engineGraphicObjectVBODispose(engineGraphicObjectVBOId egoId){
	struct engineGraphicObjectVBO *prev = NULL;
	struct engineGraphicObjectVBO *temp = localGlobal.egoVBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			// リストから要素を外す
			struct engineGraphicObjectVBO *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.egoVBOList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			egoVBOFree(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 3DオブジェクトIBO除去
void engineGraphicObjectIBODispose(engineGraphicObjectIBOId egoId){
	struct engineGraphicObjectIBO *prev = NULL;
	struct engineGraphicObjectIBO *temp = localGlobal.egoIBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			// リストから要素を外す
			struct engineGraphicObjectIBO *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.egoIBOList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			egoIBOFree(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// テクスチャ情報除去
static void texDataDispose(struct engineGraphicObjectTexData *this){
	// 使用中確認
	struct engineGraphicObjectTex *tempTex = localGlobal.egoTexList;
	while(tempTex != NULL){
		if(tempTex->data == this){return;}
		tempTex = tempTex->next;
	}
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
	struct engineGraphicObjectTex *prev = NULL;
	struct engineGraphicObjectTex *temp = localGlobal.egoTexList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			// リストから要素を外す
			struct engineGraphicObjectTex *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.egoTexList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			texDataDispose(dispose->data);
			egoTexFree(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 全3Dオブジェクト除去
void engineGraphicObjectDispose(){
	struct engineGraphicObjectVBO *tempVBO = localGlobal.egoVBOList;
	while(tempVBO != NULL){
		struct engineGraphicObjectVBO *dispose = tempVBO;
		tempVBO = tempVBO->next;
		// 要素の除去
		egoVBOFree(dispose);
	}
	localGlobal.egoVBOList = NULL;

	struct engineGraphicObjectIBO *tempIBO = localGlobal.egoIBOList;
	while(tempIBO != NULL){
		struct engineGraphicObjectIBO *dispose = tempIBO;
		tempIBO = tempIBO->next;
		// 要素の除去
		egoIBOFree(dispose);
	}
	localGlobal.egoIBOList = NULL;

	struct engineGraphicObjectTex *tempTex = localGlobal.egoTexList;
	while(tempTex != NULL){
		struct engineGraphicObjectTex *dispose = tempTex;
		tempTex = tempTex->next;
		// 要素の除去
		egoTexFree(dispose);
	}
	localGlobal.egoTexList = NULL;

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
void engineGraphicObjectReload(){
	struct engineGraphicObjectVBO *tempVBO = localGlobal.egoVBOList;
	struct engineGraphicObjectIBO *tempIBO = localGlobal.egoIBOList;
	struct engineGraphicObjectTexData *tempTex = localGlobal.texDataList;
	while(tempVBO != NULL){egoVBOLoad(tempVBO); tempVBO = tempVBO->next;}
	while(tempIBO != NULL){egoIBOLoad(tempIBO); tempIBO = tempIBO->next;}
	while(tempTex != NULL){texDataLoad(tempTex); tempTex = tempTex->next;}

	// 読み込み中に使うデフォルトテクスチャ作成
	uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
	glGenTextures(1, &localGlobal.defaultTexture.glId);
	glBindTexture(GL_TEXTURE_2D, localGlobal.defaultTexture.glId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

