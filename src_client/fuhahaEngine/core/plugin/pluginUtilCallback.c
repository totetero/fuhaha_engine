#include "fuhahaEngine.h"

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
		new = (struct pluginUtilCallbackUnit*)engineUtilMemoryInfoCalloc("(permanent)", 1, sizeof(struct pluginUtilCallbackUnit));
	}

	// 要素の設定
	new->next = NULL;
	new->callbackId = ++localGlobal.callbackIdCount;
	new->param = param;
	new->callback = callback;
	// 要素をリストに追加
	new->next = localGlobal.list;
	localGlobal.list = new;

	return new->callbackId;
}

// コールバック関数の取得と解放 ひな形
void *corePluginUtilCallbackGet(pluginUtilCallbackId callbackId, void **param, bool isFree){
	struct pluginUtilCallbackUnit *prev = NULL;
	struct pluginUtilCallbackUnit *temp = localGlobal.list;
	while(temp != NULL){
		if(temp->callbackId == callbackId){
			// コールバック取得
			void *callback = temp->callback;
			if(param != NULL){*param = temp->param;}

			if(isFree){
				// リストから要素を外す
				struct pluginUtilCallbackUnit *dispose = temp;
				temp = temp->next;
				if(prev == NULL){localGlobal.list = temp;}
				else{prev->next = temp;}
				// 解放の代わりに要素のプール
				dispose->next = NULL;
				dispose->callbackId = 0;
				dispose->param = NULL;
				dispose->callback = NULL;
				// 要素をプールに追加
				dispose->next = localGlobal.pool;
				localGlobal.pool = dispose;
			}

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
//		engineUtilMemoryInfoFree("(permanent)", dispose->param);
//		dispose->param = NULL;
//		engineUtilMemoryInfoFree("(permanent)", dispose);
//	}
//	localGlobal.list = NULL;
//
//	struct pluginUtilCallbackUnit *tempPool = localGlobal.pool;
//	while(tempPool != NULL){
//		struct pluginUtilCallbackUnit *dispose = tempPool;
//		tempPool = tempPool->next;
//		// 要素の除去
//		engineUtilMemoryInfoFree("(permanent)", dispose);
//	}
//	localGlobal.pool = NULL;
//}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

