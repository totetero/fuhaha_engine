#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3DオブジェクトVBO構造体
struct e3dObjectVBO{
	struct e3dObjectVBO *next;
	e3dObjectVBOId e3dId;
	// VBOデータ
	GLuint glId;
	uint32_t size;
	GLfloat *vertices;
};

// 3DオブジェクトIBO構造体
struct e3dObjectIBO{
	struct e3dObjectIBO *next;
	e3dObjectIBOId e3dId;
	// IBOデータ
	GLuint glId;
	uint32_t size;
	GLushort *indexes;
};

// 3DオブジェクトTex構造体
struct e3dObjectTex{
	struct e3dObjectTex *next;
	e3dObjectTexId e3dId;
	// テクスチャデータ
	struct e3dObjectTexData *data;
	enum e3dTexType type;
};

// テクスチャ情報構造体
struct e3dObjectTexData{
	struct e3dObjectTexData *next;
	GLuint glId;
	char *src;
};

// 3Dオブジェクトリスト
static uint32_t e3dIdCount = 1;
static struct e3dObjectVBO *e3dObjectVBOList = NULL;
static struct e3dObjectIBO *e3dObjectIBOList = NULL;
static struct e3dObjectTex *e3dObjectTexList = NULL;
static struct e3dObjectTexData *e3dObjectTexDataList = NULL;

// ----------------------------------------------------------------

// VBO作成
static void e3dObjectVBOLoad(struct e3dObjectVBO *this){
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->size, this->vertices, GL_STATIC_DRAW);
	e3dMemoryResetVBO();
}

// IBO作成
static void e3dObjectIBOLoad(struct e3dObjectIBO *this){
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->size, this->indexes, GL_STATIC_DRAW);
	e3dMemoryResetIBO();
}

// テクスチャ作成
static void e3dObjectTexDataLoad(struct e3dObjectTexData *this){
	// 読み込み中用のデフォルトテクスチャ作成
	uint8_t data[4] = {0xff, 0xff, 0xff, 0xff};
	glGenTextures(1, &this->glId);
	glBindTexture(GL_TEXTURE_2D, this->glId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	e3dMemoryResetTex();
	// テクスチャロード
	//platformTextureLoad(this->glId, this->src); TODO e3dMemoryResetTexも?
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO作成
e3dObjectVBOId e3dObjectVBOCreate(uint32_t size, double *vertices){
	// データ作成
	struct e3dObjectVBO *obj = (struct e3dObjectVBO*)calloc(1, sizeof(struct e3dObjectVBO));
	obj->e3dId = e3dIdCount++;
	obj->size = size;
	uint32_t memsize = size * sizeof(GLfloat);
	obj->vertices = (GLfloat*)malloc(memsize);
	for(uint32_t i = 0; i < size; i++){obj->vertices[i] = (GLfloat)vertices[i];}
	e3dObjectVBOLoad(obj);
	// リスト登録
	if(e3dObjectVBOList == NULL){
		e3dObjectVBOList = obj;
	}else{
		struct e3dObjectVBO *temp = e3dObjectVBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->e3dId;
}

// 3DオブジェクトIBO作成
e3dObjectIBOId e3dObjectIBOCreate(uint32_t size, uint16_t *indexes){
	// データ作成
	struct e3dObjectIBO *obj = (struct e3dObjectIBO*)calloc(1, sizeof(struct e3dObjectIBO));
	obj->e3dId = e3dIdCount++;
	obj->size = size;
	uint32_t memsize = size * sizeof(GLushort);
	obj->indexes = (GLushort*)malloc(memsize);
	for(uint32_t i = 0; i < size; i++){obj->indexes[i] = (GLushort)indexes[i];}
	e3dObjectIBOLoad(obj);
	// リスト登録
	if(e3dObjectIBOList == NULL){
		e3dObjectIBOList = obj;
	}else{
		struct e3dObjectIBO *temp = e3dObjectIBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->e3dId;
}

// テクスチャ情報作成
static struct e3dObjectTexData *e3dObjectTexDataCreate(char *src){
	// 重複確認
	struct e3dObjectTexData *temp = e3dObjectTexDataList;
	while(temp->next != NULL){
		if(!strncmp(src, temp->src, 256)){return temp;}
		temp = temp->next;
	}
	// 重複がなければ新規作成
	struct e3dObjectTexData *obj = (struct e3dObjectTexData*)calloc(1, sizeof(struct e3dObjectTexData));
	uint32_t memsize = ((uint32_t)strlen(src) + 1) * sizeof(char);
	obj->src = (char*)malloc(memsize);
	memmove(obj->src, src, memsize);
	e3dObjectTexDataLoad(obj);
	// リスト登録
	if(e3dObjectTexDataList == NULL){
		e3dObjectTexDataList = obj;
	}else{
		struct e3dObjectTexData *temp = e3dObjectTexDataList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj;
}

// 3DオブジェクトTex作成
e3dObjectTexId e3dObjectTexCreate(char *src, enum e3dTexType type){
	// データ作成
	struct e3dObjectTex *obj = (struct e3dObjectTex*)calloc(1, sizeof(struct e3dObjectTex));
	obj->e3dId = e3dIdCount++;
	obj->data = e3dObjectTexDataCreate(src);
	obj->type = type;
	// リスト登録
	if(e3dObjectTexList == NULL){
		e3dObjectTexList = obj;
	}else{
		struct e3dObjectTex *temp = e3dObjectTexList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->e3dId;
}

// ----------------------------------------------------------------

// VBOID取得
bool e3dObjectVBOGetGLId(e3dObjectVBOId e3dId, GLuint *glId){
	if(glId == NULL){return false;}
	struct e3dObjectVBO *obj = e3dObjectVBOList;
	while(obj != NULL){
		if(obj->e3dId == e3dId){
			*glId = obj->glId;
			return true;
		}
		obj = obj->next;
	}
	return false;
}

// IBOID取得
bool e3dObjectIBOGetGLId(e3dObjectIBOId e3dId, GLuint *glId){
	if(glId == NULL){return false;}
	struct e3dObjectIBO *obj = e3dObjectIBOList;
	while(obj != NULL){
		if(obj->e3dId == e3dId){
			*glId = obj->glId;
			return true;
		}
		obj = obj->next;
	}
	return false;
}

// テクスチャID取得
bool e3dObjectTexGetGLId(e3dObjectTexId e3dId, GLuint *glId, enum e3dTexType *type){
	if(glId == NULL && type == NULL){return false;}
	struct e3dObjectTex *obj = e3dObjectTexList;
	while(obj != NULL){
		if(obj->e3dId == e3dId){
			if(obj->data == NULL){return false;}
			if(glId != NULL){*glId = obj->data->glId;}
			if(type != NULL){*type = obj->type;}
			return true;
		}
		obj = obj->next;
	}
	return false;
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO除去
void e3dObjectVBODispose(e3dObjectVBOId e3dId){
	struct e3dObjectVBO *prevObj = NULL;
	struct e3dObjectVBO *tempObj = e3dObjectVBOList;
	while(tempObj != NULL){
		if(tempObj->e3dId == e3dId){
			// リストから要素を外す
			struct e3dObjectVBO *disposeObj = tempObj;
			tempObj = tempObj->next;
			if(prevObj == NULL){e3dObjectVBOList = tempObj;}
			else{prevObj->next = tempObj;}
			// 要素の除去
			glDeleteBuffers(1, &disposeObj->glId);
			free(disposeObj->vertices);
			free(disposeObj);
		}else{
			prevObj = tempObj;
			tempObj = tempObj->next;
		}
	}
}

// 3DオブジェクトIBO除去
void e3dObjectIBODispose(e3dObjectIBOId e3dId){
	struct e3dObjectIBO *prevObj = NULL;
	struct e3dObjectIBO *tempObj = e3dObjectIBOList;
	while(tempObj != NULL){
		if(tempObj->e3dId == e3dId){
			// リストから要素を外す
			struct e3dObjectIBO *disposeObj = tempObj;
			tempObj = tempObj->next;
			if(prevObj == NULL){e3dObjectIBOList = tempObj;}
			else{prevObj->next = tempObj;}
			// 要素の除去
			glDeleteBuffers(1, &disposeObj->glId);
			free(disposeObj->indexes);
			free(disposeObj);
		}else{
			prevObj = tempObj;
			tempObj = tempObj->next;
		}
	}
}

// テクスチャ情報除去
static void e3dObjectTexDataDispose(struct e3dObjectTexData *this){
	// 使用中確認
	struct e3dObjectTex *obj = e3dObjectTexList;
	while(obj != NULL){
		if(obj->data == this){return;}
		obj = obj->next;
	}
	// 誰も使っていなければ除去
	struct e3dObjectTexData *prevObj = NULL;
	struct e3dObjectTexData *tempObj = e3dObjectTexDataList;
	while(tempObj != NULL){
		if(tempObj == this){
			// リストから要素を外す
			tempObj = tempObj->next;
			if(prevObj == NULL){e3dObjectTexDataList = tempObj;}
			else{prevObj->next = tempObj;}
			break;
		}else{
			prevObj = tempObj;
			tempObj = tempObj->next;
		}
	}
	// 除去
	glDeleteTextures(1, &this->glId);
	free(this->src);
	free(this);
}

// 3DオブジェクトTex除去
void e3dObjectTexDispose(e3dObjectTexId e3dId){
	struct e3dObjectTex *prevObj = NULL;
	struct e3dObjectTex *tempObj = e3dObjectTexList;
	while(tempObj != NULL){
		if(tempObj->e3dId == e3dId){
			// リストから要素を外す
			struct e3dObjectTex *disposeObj = tempObj;
			tempObj = tempObj->next;
			if(prevObj == NULL){e3dObjectTexList = tempObj;}
			else{prevObj->next = tempObj;}
			// 要素の除去
			e3dObjectTexDataDispose(disposeObj->data);
			free(disposeObj);
		}else{
			prevObj = tempObj;
			tempObj = tempObj->next;
		}
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 全データロード再読み込み
void e3dObjectReload(){
	struct e3dObjectVBO *objVBO = e3dObjectVBOList;
	struct e3dObjectIBO *objIBO = e3dObjectIBOList;
	struct e3dObjectTexData *objTex = e3dObjectTexDataList;
	while(objVBO != NULL){e3dObjectVBOLoad(objVBO); objVBO = objVBO->next;}
	while(objIBO != NULL){e3dObjectIBOLoad(objIBO); objIBO = objIBO->next;}
	while(objTex != NULL){e3dObjectTexDataLoad(objTex); objTex = objTex->next;}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

