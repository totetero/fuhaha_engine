#include "../engine.h"

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

static struct{
	// デフォルトテクスチャ
	struct{
		GLuint glId;
	} defaultTexture;
	// 3Dオブジェクトリスト
	uint32_t e3dIdCount;
	struct e3dObjectVBO *e3dObjectVBOList;
	struct e3dObjectIBO *e3dObjectIBOList;
	struct e3dObjectTex *e3dObjectTexList;
	struct e3dObjectTexData *e3dObjectTexDataList;
} localGlobal = {0};

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
	// 読み込み中のデフォルトテクスチャ設定
	this->glId = localGlobal.defaultTexture.glId;
	// テクスチャロード
	//platformTextureLoad(this->glId, this->src); TODO e3dMemoryResetTexも?
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO作成
e3dObjectVBOId e3dObjectVBOCreate(uint32_t size, double *vertices){
	// データ作成
	struct e3dObjectVBO *obj = (struct e3dObjectVBO*)calloc(1, sizeof(struct e3dObjectVBO));
	obj->e3dId = ++localGlobal.e3dIdCount;
	obj->size = size;
	uint32_t memsize = size * sizeof(GLfloat);
	obj->vertices = (GLfloat*)malloc(memsize);
	for(uint32_t i = 0; i < size; i++){obj->vertices[i] = (GLfloat)vertices[i];}
	e3dObjectVBOLoad(obj);
	// リスト登録
	if(localGlobal.e3dObjectVBOList == NULL){
		localGlobal.e3dObjectVBOList = obj;
	}else{
		struct e3dObjectVBO *temp = localGlobal.e3dObjectVBOList;
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
	obj->e3dId = ++localGlobal.e3dIdCount;
	obj->size = size;
	uint32_t memsize = size * sizeof(GLushort);
	obj->indexes = (GLushort*)malloc(memsize);
	for(uint32_t i = 0; i < size; i++){obj->indexes[i] = (GLushort)indexes[i];}
	e3dObjectIBOLoad(obj);
	// リスト登録
	if(localGlobal.e3dObjectIBOList == NULL){
		localGlobal.e3dObjectIBOList = obj;
	}else{
		struct e3dObjectIBO *temp = localGlobal.e3dObjectIBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->e3dId;
}

// テクスチャ情報作成
static struct e3dObjectTexData *e3dObjectTexDataCreate(char *src){
	// 重複確認
	struct e3dObjectTexData *temp = localGlobal.e3dObjectTexDataList;
	while(temp != NULL){
		if(strcmp(temp->src, src) == 0){return temp;}
		temp = temp->next;
	}
	// 重複がなければ新規作成
	struct e3dObjectTexData *obj = (struct e3dObjectTexData*)calloc(1, sizeof(struct e3dObjectTexData));
	uint32_t memsize = ((uint32_t)strlen(src) + 1) * sizeof(char);
	obj->src = (char*)malloc(memsize);
	memmove(obj->src, src, memsize);
	e3dObjectTexDataLoad(obj);
	// リスト登録
	if(localGlobal.e3dObjectTexDataList == NULL){
		localGlobal.e3dObjectTexDataList = obj;
	}else{
		struct e3dObjectTexData *temp = localGlobal.e3dObjectTexDataList;
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
	obj->e3dId = ++localGlobal.e3dIdCount;
	obj->data = e3dObjectTexDataCreate(src);
	obj->type = type;
	// リスト登録
	if(localGlobal.e3dObjectTexList == NULL){
		localGlobal.e3dObjectTexList = obj;
	}else{
		struct e3dObjectTex *temp = localGlobal.e3dObjectTexList;
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
	struct e3dObjectVBO *temp = localGlobal.e3dObjectVBOList;
	while(temp != NULL){
		if(temp->e3dId == e3dId){
			*glId = temp->glId;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// IBOID取得
bool e3dObjectIBOGetGLId(e3dObjectIBOId e3dId, GLuint *glId){
	if(glId == NULL){return false;}
	struct e3dObjectIBO *temp = localGlobal.e3dObjectIBOList;
	while(temp != NULL){
		if(temp->e3dId == e3dId){
			*glId = temp->glId;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// テクスチャID取得
bool e3dObjectTexGetGLId(e3dObjectTexId e3dId, GLuint *glId, enum e3dTexType *type){
	if(glId == NULL && type == NULL){return false;}
	struct e3dObjectTex *temp = localGlobal.e3dObjectTexList;
	while(temp != NULL){
		if(temp->e3dId == e3dId){
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
void e3dObjectVBODispose(e3dObjectVBOId e3dId){
	struct e3dObjectVBO *prev = NULL;
	struct e3dObjectVBO *temp = localGlobal.e3dObjectVBOList;
	while(temp != NULL){
		if(temp->e3dId == e3dId){
			// リストから要素を外す
			struct e3dObjectVBO *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.e3dObjectVBOList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			glDeleteBuffers(1, &dispose->glId);
			free(dispose->vertices);
			free(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 3DオブジェクトIBO除去
void e3dObjectIBODispose(e3dObjectIBOId e3dId){
	struct e3dObjectIBO *prev = NULL;
	struct e3dObjectIBO *temp = localGlobal.e3dObjectIBOList;
	while(temp != NULL){
		if(temp->e3dId == e3dId){
			// リストから要素を外す
			struct e3dObjectIBO *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.e3dObjectIBOList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			glDeleteBuffers(1, &dispose->glId);
			free(dispose->indexes);
			free(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// テクスチャ情報除去
static void e3dObjectTexDataDispose(struct e3dObjectTexData *this){
	// 使用中確認
	struct e3dObjectTex *tempTex = localGlobal.e3dObjectTexList;
	while(tempTex != NULL){
		if(tempTex->data == this){return;}
		tempTex = tempTex->next;
	}
	// 誰も使っていなければ除去
	struct e3dObjectTexData *prevData = NULL;
	struct e3dObjectTexData *tempData = localGlobal.e3dObjectTexDataList;
	while(tempData != NULL){
		if(tempData == this){
			// リストから要素を外す
			tempData = tempData->next;
			if(prevData == NULL){localGlobal.e3dObjectTexDataList = tempData;}
			else{prevData->next = tempData;}
		}else{
			prevData = tempData;
			tempData = tempData->next;
		}
	}
	// 除去
	glDeleteTextures(1, &this->glId);
	free(this->src);
	free(this);
}

// 3DオブジェクトTex除去
void e3dObjectTexDispose(e3dObjectTexId e3dId){
	struct e3dObjectTex *prev = NULL;
	struct e3dObjectTex *temp = localGlobal.e3dObjectTexList;
	while(temp != NULL){
		if(temp->e3dId == e3dId){
			// リストから要素を外す
			struct e3dObjectTex *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.e3dObjectTexList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			e3dObjectTexDataDispose(dispose->data);
			free(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 全3Dオブジェクト除去
void e3dObjectDispose(){
	struct e3dObjectVBO *tempVBO = localGlobal.e3dObjectVBOList;
	while(tempVBO != NULL){
		struct e3dObjectVBO *dispose = tempVBO;
		tempVBO = tempVBO->next;
		// 要素の除去
		glDeleteBuffers(1, &dispose->glId);
		free(dispose->vertices);
		free(dispose);
	}
	localGlobal.e3dObjectVBOList = NULL;

	struct e3dObjectIBO *tempIBO = localGlobal.e3dObjectIBOList;
	while(tempIBO != NULL){
		struct e3dObjectIBO *dispose = tempIBO;
		tempIBO = tempIBO->next;
		// 要素の除去
		glDeleteBuffers(1, &dispose->glId);
		free(dispose->indexes);
		free(dispose);
	}
	localGlobal.e3dObjectIBOList = NULL;

	struct e3dObjectTex *tempTex = localGlobal.e3dObjectTexList;
	while(tempTex != NULL){
		struct e3dObjectTex *dispose = tempTex;
		tempTex = tempTex->next;
		// 要素の除去
		free(dispose);
	}
	localGlobal.e3dObjectTexList = NULL;

	struct e3dObjectTexData *tempData = localGlobal.e3dObjectTexDataList;
	while(tempData != NULL){
		struct e3dObjectTexData *dispose = tempData;
		tempData = tempData->next;
		// 要素の除去
		if(dispose->glId != localGlobal.defaultTexture.glId){glDeleteTextures(1, &dispose->glId);}
		free(dispose->src);
		free(dispose);
	}
	localGlobal.e3dObjectTexDataList = NULL;

	// デフォルトテクスチャ除去
	glDeleteTextures(1, &localGlobal.defaultTexture.glId);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 全データロード再読み込み
void e3dObjectReload(){
	struct e3dObjectVBO *tempVBO = localGlobal.e3dObjectVBOList;
	struct e3dObjectIBO *tempIBO = localGlobal.e3dObjectIBOList;
	struct e3dObjectTexData *tempTex = localGlobal.e3dObjectTexDataList;
	while(tempVBO != NULL){e3dObjectVBOLoad(tempVBO); tempVBO = tempVBO->next;}
	while(tempIBO != NULL){e3dObjectIBOLoad(tempIBO); tempIBO = tempIBO->next;}
	while(tempTex != NULL){e3dObjectTexDataLoad(tempTex); tempTex = tempTex->next;}

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

