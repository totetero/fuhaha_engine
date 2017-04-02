#include "../library.h"
#include "pluginUtil.h"
#include "pluginTexture.h"
#include "../engine/engineUtil/engineUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フォントテクスチャ用文字リスト作成
struct pluginTextureFontCode *gamePluginTextureFontCodeListCreate(int letterLength){
	return (struct pluginTextureFontCode*)engineUtilMemoryInfoCalloc("pluginTextureFontCodeList", letterLength, sizeof(struct pluginTextureFontCode));
}

// フォントテクスチャ用文字リストの要素設定
void gamePluginTextureFontCodeListSet(struct pluginTextureFontCode *codeList, int index, int code, int x, int y, int w, int h){
	codeList[index].code = code;
	codeList[index].x = x;
	codeList[index].y = y;
	codeList[index].w = w;
	codeList[index].h = h;
}

// フォントテクスチャ用文字リスト破棄
void gamePluginTextureFontCodeListDispose(struct pluginTextureFontCode *codeList){
	engineUtilMemoryInfoFree("pluginTextureFontCodeList", codeList);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

