#include "../library.h"
#include "pluginUtil.h"
#include "pluginTexture.h"
#include "../engine/engineUtil/engineUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct pluginTextureFontCodeListCage{
		struct pluginTextureFontCode *codeList;
	} *list;
	int length;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フォントテクスチャ用文字リスト取得
struct pluginTextureFontCode *corePluginTextureFontCodeListGet(int codeListIndex){
	if(codeListIndex < 0){return NULL;}
	if(codeListIndex >= localGlobal.length){return NULL;}
	return localGlobal.list[codeListIndex].codeList;
}

// ----------------------------------------------------------------

// フォントテクスチャ用文字リスト作成
int gamePluginTextureFontCodeListCreate(int codeListLength){
	int codeListIndex = -1;
	// 領域リストに空きがないか確認する
	for(int i = 0; i < localGlobal.length; i++){if(localGlobal.list[i].codeList == NULL){codeListIndex =  i;}}
	if(codeListIndex < 0){
		// 空きがなければ領域リストを拡張する
		codeListIndex = localGlobal.length;
		int length = localGlobal.length + 10;
		struct pluginTextureFontCodeListCage *list = (struct pluginTextureFontCodeListCage*)engineUtilMemoryInfoCalloc("(permanent) pluginTextureFontCodeListCage", length, sizeof(struct pluginTextureFontCodeListCage));
		if(localGlobal.length > 0){
			memcpy(list, localGlobal.list, localGlobal.length * sizeof(struct pluginTextureFontCodeListCage));
			engineUtilMemoryInfoFree("(permanent) pluginTextureFontCodeListCage", localGlobal.list);
		}
		localGlobal.length = length;
		localGlobal.list = list;
	}
	// 文字リスト作成
	localGlobal.list[codeListIndex].codeList = (struct pluginTextureFontCode*)engineUtilMemoryInfoCalloc("pluginTextureFontCodeList", codeListLength, sizeof(struct pluginTextureFontCode));
	return codeListIndex;
}

// フォントテクスチャ用文字リストの要素設定
void gamePluginTextureFontCodeListSet(int codeListIndex, int index, int fontSetId, int code, int glId, int imgw, int imgh, int x, int y, int w, int h){
	struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(codeListIndex);
	if(codeList == NULL){return;}
	struct pluginTextureFontCode *data = &codeList[index];
	data->fontSetId = fontSetId;
	data->code = code;
	data->glId = glId;
	data->imgw = imgw;
	data->imgh = imgh;
	data->x = x;
	data->y = y;
	data->w = w;
	data->h = h;
}

// フォントテクスチャ用文字リスト破棄
void gamePluginTextureFontCodeListDispose(int codeListIndex){
	struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(codeListIndex);
	if(codeList == NULL){return;}
	engineUtilMemoryInfoFree("pluginTextureFontCodeList", codeList);
	localGlobal.list[codeListIndex].codeList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

