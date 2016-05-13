#include "engine/engine.h"
#include "gamePluginUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック構造体
struct gamePluginUtilCallback{
	struct gamePluginUtilCallback *next;
	int callbackId;
	void *param;
	void *callback;
};

static struct{
	int callbackIdCount;
	struct gamePluginUtilCallback *list;
	struct gamePluginUtilCallback *pool;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数の登録 ひな形
int corePluginUtilCallbackSet(void *param, void *callback){
	struct gamePluginUtilCallback *new = NULL;

	if(localGlobal.pool != NULL){
		// プールから要素をとってくる
		new = localGlobal.pool;
		localGlobal.pool = new->next;
	}else{
		// 要素を新しく作成
		new = (struct gamePluginUtilCallback*)calloc(1, sizeof(struct gamePluginUtilCallback));
	}

	// 要素の設定
	new->next = NULL;
	new->callbackId = ++localGlobal.callbackIdCount;
	new->param = param;
	new->callback = callback;

	// 要素をリストに追加
	if(localGlobal.list != NULL){
		struct gamePluginUtilCallback *temp = localGlobal.list;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = new;
	}else{
		localGlobal.list = new;
	}

	return new->callbackId;
}

// コールバック関数の取得と解放 ひな形
void *corePluginUtilCallbackGet(int callbackId, void **param){
	struct gamePluginUtilCallback *prev = NULL;
	struct gamePluginUtilCallback *temp = localGlobal.list;
	while(temp != NULL){
		if(temp->callbackId == callbackId){
			// リストから要素を外す
			struct gamePluginUtilCallback *use = temp;
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
//void corePluginUtilCallbackDispose(){
//	struct gamePluginUtilCallback *tempList = localGlobal.list;
//	while(tempList != NULL){
//		struct gamePluginUtilCallback *dispose = tempList;
//		tempList = tempList->next;
//		// 要素の除去
//		free(dispose->param);
//		free(dispose);
//	}
//	localGlobal.list = NULL;
//
//	struct gamePluginUtilCallback *tempPool = localGlobal.pool;
//	while(tempPool != NULL){
//		struct gamePluginUtilCallback *dispose = tempPool;
//		tempPool = tempPool->next;
//		// 要素の除去
//		free(dispose);
//	}
//	localGlobal.pool = NULL;
//}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

