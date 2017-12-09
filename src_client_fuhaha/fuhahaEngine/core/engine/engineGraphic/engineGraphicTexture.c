#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャ引数構造体
struct engineGraphicTextureArg{
	enum engineGraphicTextureArgType{
		ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL,
		ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT,
		ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEST,
	} type;
	union{
		struct{char *src;} pluginTextureLocal;
		struct{enum pluginTextureFontSetId fontSetId; char *letterList;} pluginTextureFont;
		struct{} pluginTest;
	};
};

// テクスチャ情報構造体
struct engineGraphicTextureData{
	struct engineGraphicTextureData *next;
	struct engineGraphicTextureArg arg;
	union{
		struct{GLuint glId; int imgw; int imgh;} pluginTextureLocal;
		struct{int codeListIndex; int codeListLength;} pluginTextureFont;
		struct{} pluginTest;
	};
	enum engineGraphicTextureDataStatus{
		ENGINEGRAPHICTEXTUREDATASTATUS_LOADING,
		ENGINEGRAPHICTEXTUREDATASTATUS_LOADED,
		ENGINEGRAPHICTEXTUREDATASTATUS_CANCEL,
	} status;
};

// テクスチャ構造体
struct engineGraphicTextureCage{
	struct engineGraphicTextureData *data;
	enum engineGraphicTextureType type;
};

static struct{
	// デフォルトテクスチャ
	struct{
		GLuint glId;
		int imgw;
		int imgh;
	} defaultTexture;
	// テクスチャリスト
	struct engineGraphicTextureData *texDataList;
	struct engineGraphicTextureCage *egtCageList;
	int egtCageLength;
} localGlobal = {0};

// ----------------------------------------------------------------

static int egtId2Index(engineGraphicTextureId egtId){return (int)(egtId - 1);}
static engineGraphicTextureId egtIndex2Id(int egtIndex){return (engineGraphicTextureId)(egtIndex + 1);}

// ----------------------------------------------------------------

// テクスチャ情報解放
static void texDataFree(struct engineGraphicTextureData *this){
	if(this->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL){
		if(this->status == ENGINEGRAPHICTEXTUREDATASTATUS_LOADED){
			// 解放
			if(this->pluginTextureLocal.glId != localGlobal.defaultTexture.glId){glDeleteTextures(1, &this->pluginTextureLocal.glId);}
			engineUtilMemoryInfoFree("engineGraphicTexture tex3", this->arg.pluginTextureLocal.src);
			this->arg.pluginTextureLocal.src = NULL;
			engineUtilMemoryInfoFree("engineGraphicTexture tex2", this);
		}else{
			// ロードが完了していないのでコールバックで破棄
			this->status = ENGINEGRAPHICTEXTUREDATASTATUS_CANCEL;
		}
	}else if(this->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT){
		if(this->status == ENGINEGRAPHICTEXTUREDATASTATUS_LOADED){
			// 解放
			platformPluginTextureFontDispose(this->pluginTextureFont.codeListIndex);
			engineUtilMemoryInfoFree("engineGraphicTexture tex3", this->arg.pluginTextureFont.letterList);
			this->arg.pluginTextureFont.letterList = NULL;
			engineUtilMemoryInfoFree("engineGraphicTexture tex2", this);
		}else{
			// ロードが完了していないのでコールバックで破棄
			this->status = ENGINEGRAPHICTEXTUREDATASTATUS_CANCEL;
		}
	}else if(this->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEST){
		// 解放
		engineUtilMemoryInfoFree("engineGraphicTexture tex2", this);
	}
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void texDataLocalCallback(void *param, PLUGINTEXTURE_LOCAL_CALLBACKPARAMS){
	struct engineGraphicTextureData *this = (struct engineGraphicTextureData*)param;
	enum engineGraphicTextureDataStatus beforeStatus = this->status;
	this->status = ENGINEGRAPHICTEXTUREDATASTATUS_LOADED;
	if(beforeStatus == ENGINEGRAPHICTEXTUREDATASTATUS_LOADING){
		// ロード完了
		this->pluginTextureLocal.glId = glId;
		this->pluginTextureLocal.imgw = imgw;
		this->pluginTextureLocal.imgh = imgh;
	}else if(beforeStatus == ENGINEGRAPHICTEXTUREDATASTATUS_CANCEL){
		// ロード中止
		texDataFree(this);
	}
}

// ロード完了時コールバック
static void texDataFontCallback(void *param, PLUGINTEXTURE_FONT_CALLBACKPARAMS){
	struct engineGraphicTextureData *this = (struct engineGraphicTextureData*)param;
	enum engineGraphicTextureDataStatus beforeStatus = this->status;
	this->status = ENGINEGRAPHICTEXTUREDATASTATUS_LOADED;
	if(beforeStatus == ENGINEGRAPHICTEXTUREDATASTATUS_LOADING){
		// ロード完了
		this->pluginTextureFont.codeListIndex = codeListIndex;
		this->pluginTextureFont.codeListLength = codeListLength;
	}else if(beforeStatus == ENGINEGRAPHICTEXTUREDATASTATUS_CANCEL){
		// ロード中止
		texDataFree(this);
	}
}

// ----------------------------------------------------------------

// テクスチャ作成
static void texDataLoad(struct engineGraphicTextureData *this){
	if(this->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL){
		// 読み込み中のデフォルトテクスチャ設定
		this->pluginTextureLocal.glId = localGlobal.defaultTexture.glId;
		this->pluginTextureLocal.imgw = localGlobal.defaultTexture.imgw;
		this->pluginTextureLocal.imgh = localGlobal.defaultTexture.imgh;
		// テクスチャロード
		this->status = ENGINEGRAPHICTEXTUREDATASTATUS_LOADING;
		platformPluginTextureLocal(this, this->arg.pluginTextureLocal.src, texDataLocalCallback);
	}else if(this->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT){
		// フォント未ロード値設定
		this->pluginTextureFont.codeListIndex = -1;
		this->pluginTextureFont.codeListLength = 0;
		// テクスチャロード
		this->status = ENGINEGRAPHICTEXTUREDATASTATUS_LOADING;
		platformPluginTextureFont(this, this->arg.pluginTextureFont.fontSetId, this->arg.pluginTextureFont.letterList, texDataFontCallback);
	}else if(this->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEST){
	}
}

// ----------------------------------------------------------------

// テクスチャ情報作成
static struct engineGraphicTextureData *texDataCreate(struct engineGraphicTextureArg *arg){
	// 重複確認
	struct engineGraphicTextureData *temp = localGlobal.texDataList;
	while(temp != NULL){
		if(arg->type == temp->arg.type){
			if(arg->type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL){
				if(strcmp(temp->arg.pluginTextureLocal.src, arg->pluginTextureLocal.src) == 0){return temp;}
			}else if(arg->type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT){
				bool isSame = true;
				if(isSame && temp->arg.pluginTextureFont.fontSetId != arg->pluginTextureFont.fontSetId){isSame = false;}
				if(isSame && strcmp(temp->arg.pluginTextureFont.letterList, arg->pluginTextureFont.letterList) != 0){isSame = false;}
				if(isSame){return temp;}
			}else if(arg->type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEST){
				return temp;
			}
		}
		temp = temp->next;
	}
	// 重複がなければ新規作成
	struct engineGraphicTextureData *obj = (struct engineGraphicTextureData*)engineUtilMemoryInfoCalloc("engineGraphicTexture tex2", 1, sizeof(struct engineGraphicTextureData));
	obj->arg.type = arg->type;
	if(arg->type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL){
		obj->arg.pluginTextureLocal.src = engineUtilMemoryInfoStrdup("engineGraphicTexture tex3", arg->pluginTextureLocal.src);
	}else if(arg->type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT){
		obj->arg.pluginTextureFont.fontSetId = arg->pluginTextureFont.fontSetId;
		obj->arg.pluginTextureFont.letterList = engineUtilMemoryInfoStrdup("engineGraphicTexture tex3", arg->pluginTextureFont.letterList);
	}else if(arg->type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEST){
	}
	texDataLoad(obj);
	// リスト登録
	if(localGlobal.texDataList == NULL){
		localGlobal.texDataList = obj;
	}else{
		struct engineGraphicTextureData *temp = localGlobal.texDataList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj;
}

// テクスチャ作成
static engineGraphicTextureId egtCageCreate(struct engineGraphicTextureArg *arg, enum engineGraphicTextureType type){
	int egtIndex = -1;
	// 領域リストに空きがないか確認する
	for(int i = 0; i < localGlobal.egtCageLength; i++){if(localGlobal.egtCageList[i].data == NULL){egtIndex =  i;}}
	if(egtIndex < 0){
		// 空きがなければ領域リストを拡張する
		egtIndex = localGlobal.egtCageLength;
		int egtCageLength = localGlobal.egtCageLength + 10;
		struct engineGraphicTextureCage *egtCageList = (struct engineGraphicTextureCage*)engineUtilMemoryInfoCalloc("(permanent) engineGraphicTexture tex1", egtCageLength, sizeof(struct engineGraphicTextureCage));
		if(localGlobal.egtCageLength > 0){
			memcpy(egtCageList, localGlobal.egtCageList, localGlobal.egtCageLength * sizeof(struct engineGraphicTextureCage));
			engineUtilMemoryInfoFree("(permanent) engineGraphicTexture tex1", localGlobal.egtCageList);
		}
		localGlobal.egtCageLength = egtCageLength;
		localGlobal.egtCageList = egtCageList;
	}

	// データ作成
	struct engineGraphicTextureCage *obj = &localGlobal.egtCageList[egtIndex];
	obj->data = texDataCreate(arg);
	obj->type = type;

	return egtIndex2Id(egtIndex);
}

// テクスチャ作成
engineGraphicTextureId engineGraphicTextureCreateLocal(char *src, enum engineGraphicTextureType type){
	struct engineGraphicTextureArg arg;
	arg.type = ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL;
	arg.pluginTextureLocal.src = src;
	return egtCageCreate(&arg, type);
}

// テクスチャ作成
engineGraphicTextureId engineGraphicTextureCreateFont(enum pluginTextureFontSetId fontSetId, char *letterList, enum engineGraphicTextureType type){
	struct engineGraphicTextureArg arg;
	arg.type = ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT;
	arg.pluginTextureFont.fontSetId = fontSetId;
	arg.pluginTextureFont.letterList = letterList;
	return egtCageCreate(&arg, type);
}

// ----------------------------------------------------------------

// テクスチャ読み込み完了確認
bool engineGraphicTextureIsComplete(engineGraphicTextureId egtId){
	int egtIndex = egtId2Index(egtId);
	if(egtIndex < 0 || localGlobal.egtCageLength <= egtIndex){return true;}
	struct engineGraphicTextureCage *temp = &localGlobal.egtCageList[egtIndex];
	return (temp->data != NULL && temp->data->status == ENGINEGRAPHICTEXTUREDATASTATUS_LOADED);
}

// ----------------------------------------------------------------

// テクスチャID取得
bool engineGraphicTextureGetGLId(engineGraphicTextureId egtId, GLuint *glId, enum engineGraphicTextureType *type){
	if(glId == NULL && type == NULL){return false;}
	int egtIndex = egtId2Index(egtId);
	if(egtIndex < 0 || localGlobal.egtCageLength <= egtIndex){return false;}
	struct engineGraphicTextureCage *temp = &localGlobal.egtCageList[egtIndex];
	if(temp->data == NULL){return false;}
	if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTURELOCAL){*glId = temp->data->pluginTextureLocal.glId;}
	if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT){*glId = localGlobal.defaultTexture.glId;}
	if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEST){*glId = localGlobal.defaultTexture.glId;}
	if(type != NULL){*type = temp->type;}
	return true;
}

// テクスチャID取得
bool engineGraphicTextureGetCodeList(engineGraphicTextureId egtId, int *codeListIndex, int *codeListLength, enum engineGraphicTextureType *type){
	if(codeListIndex == NULL && codeListLength == NULL && type == NULL){return false;}
	int egtIndex = egtId2Index(egtId);
	if(egtIndex < 0 || localGlobal.egtCageLength <= egtIndex){return false;}
	struct engineGraphicTextureCage *temp = &localGlobal.egtCageList[egtIndex];
	if(temp->data == NULL){return false;}
	if(temp->data->arg.type != ENGINEGRAPHICTEXTUREARGTYPE_PLUGINTEXTUREFONT){return false;}
	if(codeListIndex != NULL){*codeListIndex = temp->data->pluginTextureFont.codeListIndex;}
	if(codeListLength != NULL){*codeListLength = temp->data->pluginTextureFont.codeListLength;}
	if(type != NULL){*type = temp->type;}
	return true;
}

// ----------------------------------------------------------------

// テクスチャ解放
static void egtCageFree(struct engineGraphicTextureCage *this){
	if(this->data == NULL){return;}
	this->data = NULL;
}

// テクスチャ除去
void engineGraphicTextureDispose(engineGraphicTextureId egtId){
	int egtIndex = egtId2Index(egtId);
	if(egtIndex < 0 || localGlobal.egtCageLength <= egtIndex){return;}
	struct engineGraphicTextureCage *cage = &localGlobal.egtCageList[egtIndex];
	struct engineGraphicTextureData *data = cage->data;
	egtCageFree(cage);
	// テクスチャ情報の使用中確認
	for(int i = 0; i < localGlobal.egtCageLength; i++){if(localGlobal.egtCageList[i].data == data){data = NULL; break;}}
	if(data != NULL){
		// 誰も使っていなければ除去
		struct engineGraphicTextureData *prevData = NULL;
		struct engineGraphicTextureData *tempData = localGlobal.texDataList;
		while(tempData != NULL){
			if(tempData == data){
				// リストから要素を外す
				tempData = tempData->next;
				if(prevData == NULL){localGlobal.texDataList = tempData;}
				else{prevData->next = tempData;}
			}else{
				prevData = tempData;
				tempData = tempData->next;
			}
		}
		// テクスチャ情報除去
		texDataFree(data);
	}
}

// 全テクスチャ除去
void engineGraphicTextureDisposeAll(void){
	for(int i = 0; i < localGlobal.egtCageLength; i++){egtCageFree(&localGlobal.egtCageList[i]);}
	engineUtilMemoryInfoFree("(permanent) engineGraphicTexture tex1", localGlobal.egtCageList);
	localGlobal.egtCageList = NULL;
	localGlobal.egtCageLength = 0;

	struct engineGraphicTextureData *tempData = localGlobal.texDataList;
	while(tempData != NULL){
		struct engineGraphicTextureData *dispose = tempData;
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

// 全テクスチャロード再読み込み
void engineGraphicTextureReload(void){
	struct engineGraphicTextureData *tempTex = localGlobal.texDataList;
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

