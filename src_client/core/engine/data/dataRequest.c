#include "../engine.h"
#include "../../gamePluginUtil.h"
#include "../../gamePluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// リクエストリスト
	uint32_t reqIdCount;
	struct reqData{
		struct reqData *next;
		dataRequestId reqId;
		void *buff;
		size_t size;
		// ロード状態
		enum{
			REQDATA_LOADING_LOADING,
			REQDATA_LOADING_LOADED,
			REQDATA_LOADING_CANCEL,
		} loading;
	} *reqList;
} localGlobal = {0};

// ----------------------------------------------------------------

// リクエストデータ作成
static struct reqData *reqDataCreate(){
	struct reqData *this = (struct reqData*)calloc(1, sizeof(struct reqData));
	// リストにデータ追加
	if(localGlobal.reqList == NULL){
		localGlobal.reqList = this;
	}else{
		struct reqData *temp = localGlobal.reqList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = this;
	}
	this->next = NULL;
	this->reqId = ++localGlobal.reqIdCount;
	this->buff = NULL;
	this->size = 0;
	this->loading = REQDATA_LOADING_LOADING;
	return this;
}

// リクエストデータ解放
static void reqDataFree(struct reqData *this){
	if(this->loading == REQDATA_LOADING_LOADED){
		// 解放
		if(this->buff != NULL){free(this->buff);}
		free(this);
	}else{
		// ロードが完了していないのでコールバックで破棄
		this->loading = REQDATA_LOADING_CANCEL;
	}
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct reqData *this = (struct reqData*)param;

	if(this->loading == REQDATA_LOADING_LOADING){
		// ロード完了
		this->buff = buff;
		this->size = size;
		this->loading = REQDATA_LOADING_LOADED;
	}else if(this->loading == REQDATA_LOADING_CANCEL){
		// ロード中止
		if(this != NULL){free(this);}
		if(buff != NULL){free(buff);}
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 読み込み完了確認
bool dataRequestIsLoading(){
	return platformPluginUtilIsLoading();
}

// ----------------------------------------------------------------

// 通信リクエスト
dataRequestId dataRequestHttp(char *url, char *request){
	struct reqData *this = reqDataCreate();
	platformPluginDataHttp(this, url, request, callback);
	return this->reqId;
}

// ローカルファイルリクエスト
dataRequestId dataRequestLocal(char *src){
	struct reqData *this = reqDataCreate();
	platformPluginDataLocal(this, src, callback);
	return this->reqId;
}

// ----------------------------------------------------------------

// リクエスト応答
bool dataRequestResponse(dataRequestId reqId, void **buff, size_t *size){
	struct reqData *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			if(temp->loading == REQDATA_LOADING_LOADED){
				// データが存在した場合
				if(buff != NULL){*buff = temp->buff;}
				if(size != NULL){*size = temp->size;}
				return true;
			}else{
				// データロード中
				return false;
			}
		}
		temp = temp->next;
	}
	// データなし
	return false;
}

// ----------------------------------------------------------------

// リクエスト解放
void dataRequestFree(dataRequestId reqId){
	struct reqData *prev = NULL;
	struct reqData *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			struct reqData *dispose = temp;
			// 次のを装填
			if(prev == NULL){localGlobal.reqList = temp->next;}
			else{prev->next = temp->next;}
			temp = temp->next;
			// 解放
			reqDataFree(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 全リクエスト解放
void dataRequestFreeAll(){
	struct reqData *temp = localGlobal.reqList;
	while(temp != NULL){
		struct reqData *dispose = temp;
		temp = temp->next;
		// 解放
		reqDataFree(dispose);
	}
	localGlobal.reqList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

