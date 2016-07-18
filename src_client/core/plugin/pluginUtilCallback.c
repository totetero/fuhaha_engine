#include "../library.h"
#include "pluginUtil.h"
#include "../engine/engineUtil/engineUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック構造体
struct pluginUtilCallbackUnit{
	struct pluginUtilCallbackUnit *next;
	pluginUtilCallbackId callbackId;
	void *param;
	void *callback;
};

static struct{
	int callbackIdCount;
	struct pluginUtilCallbackUnit *list;
	struct pluginUtilCallbackUnit *pool;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録 ひな形
pluginUtilCallbackId corePluginUtilCallbackSet(void *param, void *callback){
	struct pluginUtilCallbackUnit *new = NULL;

	if(localGlobal.pool != NULL){
		// プールから要素をとってくる
		new = localGlobal.pool;
		localGlobal.pool = new->next;
	}else{
		// 要素を新しく作成
		new = (struct pluginUtilCallbackUnit*)engineUtilMemoryCalloc(1, sizeof(struct pluginUtilCallbackUnit));
	}

	// 要素の設定
	new->next = NULL;
	new->callbackId = ++localGlobal.callbackIdCount;
	new->param = param;
	new->callback = callback;

	// 要素をリストに追加
	if(localGlobal.list != NULL){
		struct pluginUtilCallbackUnit *temp = localGlobal.list;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = new;
	}else{
		localGlobal.list = new;
	}

	return new->callbackId;
}

// コールバック関数の取得と解放 ひな形
void *corePluginUtilCallbackGet(pluginUtilCallbackId callbackId, void **param){
	struct pluginUtilCallbackUnit *prev = NULL;
	struct pluginUtilCallbackUnit *temp = localGlobal.list;
	while(temp != NULL){
		if(temp->callbackId == callbackId){
			// リストから要素を外す
			struct pluginUtilCallbackUnit *use = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.list = temp;}
			else{prev->next = temp;}
			// コールバック取得
			void *callback = use->callback;
			if(param != NULL){*param = use->param;}
			// 解放の代わりに要素のプール
			use->callbackId = 0;
			use->param = NULL;
			use->callback = NULL;
			use->next = localGlobal.pool;
			localGlobal.pool = use;

			return callback;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
	return NULL;
}

//// コールバック関数の全解放
//void corePluginUtilCallbackDispose(void){
//	struct pluginUtilCallbackUnit *tempList = localGlobal.list;
//	while(tempList != NULL){
//		struct pluginUtilCallbackUnit *dispose = tempList;
//		tempList = tempList->next;
//		// 要素の除去
//		engineUtilMemoryFree(dispose->param);
//		engineUtilMemoryFree(dispose);
//	}
//	localGlobal.list = NULL;
//
//	struct pluginUtilCallbackUnit *tempPool = localGlobal.pool;
//	while(tempPool != NULL){
//		struct pluginUtilCallbackUnit *dispose = tempPool;
//		tempPool = tempPool->next;
//		// 要素の除去
//		engineUtilMemoryFree(dispose);
//	}
//	localGlobal.pool = NULL;
//}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

