#include "engine/engine.h"
#include "gamePluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック構造体
struct gamePluginDataCallback{
	struct gamePluginDataCallback *next;
	int callbackId;
	void *param;
	void (*callback)(void *param, void *buff, size_t size);
};

static struct{
	// コールバック
	struct{
		int idCount;
		struct gamePluginDataCallback *list;
		struct gamePluginDataCallback *pool;
	} callback;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コールバック関数ポインタの登録
int gamePluginDataCallbackSet(void *param, void(*callback)(void *param, void *buff, size_t size)){
	struct gamePluginDataCallback *new = NULL;

	if(localGlobal.callback.pool != NULL){
		// プールから要素をとってくる
		new = localGlobal.callback.pool;
		localGlobal.callback.pool = new->next;
	}else{
		// 要素を新しく作成
		new = (struct gamePluginDataCallback*)calloc(1, sizeof(struct gamePluginDataCallback));
	}

	// 要素の設定
	new->next = NULL;
	new->callbackId = ++localGlobal.callback.idCount;
	new->param = param;
	new->callback = callback;

	// 要素をリストに追加
	if(localGlobal.callback.list != NULL){
		struct gamePluginDataCallback *temp = localGlobal.callback.list;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = new;
	}else{
		localGlobal.callback.list = new;
	}

	return new->callbackId;
}

// コールバック関数の実行
bool gamePluginDataCallbackCall(int callbackId, void *buff, size_t size){
	struct gamePluginDataCallback *prev = NULL;
	struct gamePluginDataCallback *temp = localGlobal.callback.list;
	while(temp != NULL){
		if(temp->callbackId == callbackId){
			// リストから要素を外す
			struct gamePluginDataCallback *use = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.callback.list = temp;}
			else{prev->next = temp;}
			// コールバック実行
			use->callback(use->param, buff, size);
			use->callbackId = 0;
			use->param = NULL;
			use->callback = NULL;
			// 要素のプール
			use->next = localGlobal.callback.pool;
			localGlobal.callback.pool = use;
			return true;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
	return false;
}

//// コールバック関数の全解放
//void gamePluginDataCallbackDispose(){
//	struct gamePluginDataCallback *tempList = localGlobal.callback.list;
//	while(tempList != NULL){
//		struct gamePluginDataCallback *dispose = tempList;
//		tempList = tempList->next;
//		// 要素の除去
//		free(dispose->param);
//		free(dispose);
//	}
//	localGlobal.callback.list = NULL;
//
//	struct gamePluginDataCallback *tempPool = localGlobal.callback.pool;
//	while(tempPool != NULL){
//		struct gamePluginDataCallback *dispose = tempPool;
//		tempPool = tempPool->next;
//		// 要素の除去
//		free(dispose);
//	}
//	localGlobal.callback.pool = NULL;
//}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

