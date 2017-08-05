#include "../../library.h"
#include "../engineUtil/engineUtil.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3DオブジェクトVBO構造体
struct engineGraphicObjectVBO{
	GLuint glId;
	int length;
	GLfloat *vertices;
};

// 3DオブジェクトIBO構造体
struct engineGraphicObjectIBO{
	GLuint glId;
	int length;
	GLushort *indexes;
};

static struct{
	// 3Dオブジェクトリスト
	struct engineGraphicObjectVBO *egoVBOList;
	struct engineGraphicObjectIBO *egoIBOList;
	int egoVBOLength;
	int egoIBOLength;
} localGlobal = {0};

// ----------------------------------------------------------------

static int egoVBOId2Index(engineGraphicObjectVBOId egoId){return (int)(egoId - 1);}
static int egoIBOId2Index(engineGraphicObjectIBOId egoId){return (int)(egoId - 1);}
static engineGraphicObjectVBOId egoVBOIndex2Id(int egoIndex){return (engineGraphicObjectVBOId)(egoIndex + 1);}
static engineGraphicObjectIBOId egoIBOIndex2Id(int egoIndex){return (engineGraphicObjectIBOId)(egoIndex + 1);}

// ----------------------------------------------------------------

// VBO作成
static void egoVBOLoad(struct engineGraphicObjectVBO *this){
	if(this->vertices == NULL){return;}
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->length, this->vertices, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetVBO();
}

// IBO作成
static void egoIBOLoad(struct engineGraphicObjectIBO *this){
	if(this->indexes == NULL){return;}
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->length, this->indexes, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetIBO();
}

// 3DオブジェクトVBO作成
engineGraphicObjectVBOId engineGraphicObjectVBOCreate(int length, GLfloat *vertices){
	int egoIndex = -1;
	// 領域リストに空きがないか確認する
	for(int i = 0; i < localGlobal.egoVBOLength; i++){if(localGlobal.egoVBOList[i].vertices == NULL){egoIndex =  i;}}
	if(egoIndex < 0){
		// 空きがなければ領域リストを拡張する
		egoIndex = localGlobal.egoVBOLength;
		int egoVBOLength = localGlobal.egoVBOLength + 10;
		struct engineGraphicObjectVBO *egoVBOList = (struct engineGraphicObjectVBO*)engineUtilMemoryInfoCalloc("(permanent) engineGraphicObject vbo1", egoVBOLength, sizeof(struct engineGraphicObjectVBO));
		if(localGlobal.egoVBOLength > 0){
			memcpy(egoVBOList, localGlobal.egoVBOList, localGlobal.egoVBOLength * sizeof(struct engineGraphicObjectVBO));
			engineUtilMemoryInfoFree("(permanent) engineGraphicObject vbo1", localGlobal.egoVBOList);
		}
		localGlobal.egoVBOLength = egoVBOLength;
		localGlobal.egoVBOList = egoVBOList;
	}

	// データ作成
	struct engineGraphicObjectVBO *obj = &localGlobal.egoVBOList[egoIndex];
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
		struct engineGraphicObjectIBO *egoIBOList = (struct engineGraphicObjectIBO*)engineUtilMemoryInfoCalloc("(permanent) engineGraphicObject ibo1", egoIBOLength, sizeof(struct engineGraphicObjectIBO));
		if(localGlobal.egoIBOLength > 0){
			memcpy(egoIBOList, localGlobal.egoIBOList, localGlobal.egoIBOLength * sizeof(struct engineGraphicObjectIBO));
			engineUtilMemoryInfoFree("(permanent) engineGraphicObject ibo1", localGlobal.egoIBOList);
		}
		localGlobal.egoIBOLength = egoIBOLength;
		localGlobal.egoIBOList = egoIBOList;
	}

	// データ作成
	struct engineGraphicObjectIBO *obj = &localGlobal.egoIBOList[egoIndex];
	obj->length = length;
	size_t size = length * sizeof(GLushort);
	obj->indexes = (GLushort*)engineUtilMemoryInfoMalloc("engineGraphicObject ibo2", size);
	memcpy(obj->indexes, indexes, size);
	//for(int i = 0; i < length; i++){obj->indexes[i] = (GLushort)indexes[i];}
	egoIBOLoad(obj);

	return egoIBOIndex2Id(egoIndex);
}

// ----------------------------------------------------------------

// TODO 必要ならばGL_DYNAMIC_DRAWとglBufferSubDataを使う仕組みを作る

// ----------------------------------------------------------------

// VBOID取得
bool engineGraphicObjectVBOGetGLId(engineGraphicObjectVBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	int egoIndex = egoVBOId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoVBOLength <= egoIndex){return false;}
	struct engineGraphicObjectVBO *temp = &localGlobal.egoVBOList[egoIndex];
	if(temp->vertices == NULL){return false;}
	*glId = temp->glId;
	return true;
}

// IBOID取得
bool engineGraphicObjectIBOGetGLId(engineGraphicObjectIBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	int egoIndex = egoIBOId2Index(egoId);
	if(egoIndex < 0 || localGlobal.egoIBOLength <= egoIndex){return false;}
	struct engineGraphicObjectIBO *temp = &localGlobal.egoIBOList[egoIndex];
	if(temp->indexes == NULL){return false;}
	*glId = temp->glId;
	return true;
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO解放
static void egoVBOFree(struct engineGraphicObjectVBO *this){
	if(this->vertices == NULL){return;}
	glDeleteBuffers(1, &this->glId);
	engineUtilMemoryInfoFree("engineGraphicObject vbo2", this->vertices);
	this->vertices = NULL;
}

// 3DオブジェクトIBO解放
static void egoIBOFree(struct engineGraphicObjectIBO *this){
	if(this->indexes == NULL){return;}
	glDeleteBuffers(1, &this->glId);
	engineUtilMemoryInfoFree("engineGraphicObject ibo2", this->indexes);
	this->indexes = NULL;
}

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

// 全3Dオブジェクト除去
void engineGraphicObjectDisposeAll(void){
	for(int i = 0; i < localGlobal.egoVBOLength; i++){egoVBOFree(&localGlobal.egoVBOList[i]);}
	for(int i = 0; i < localGlobal.egoIBOLength; i++){egoIBOFree(&localGlobal.egoIBOList[i]);}
	engineUtilMemoryInfoFree("(permanent) engineGraphicObject vbo1", localGlobal.egoVBOList);
	engineUtilMemoryInfoFree("(permanent) engineGraphicObject ibo1", localGlobal.egoIBOList);
	localGlobal.egoVBOList = NULL;
	localGlobal.egoIBOList = NULL;
	localGlobal.egoVBOLength = 0;
	localGlobal.egoIBOLength = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 全データロード再読み込み
void engineGraphicObjectReload(void){
	for(int i = 0; i < localGlobal.egoVBOLength; i++){egoVBOLoad(&localGlobal.egoVBOList[i]);}
	for(int i = 0; i < localGlobal.egoIBOLength; i++){egoIBOLoad(&localGlobal.egoIBOList[i]);}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

