#include "../engine.h"
#include "../../gamePluginUtil.h"
#include "../../gamePluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// リクエストリスト
	uint32_t reqIdCount;
	struct dataRequestUnit{
		struct dataRequestUnit *next;
		dataRequestId reqId;
		void *buff;
		size_t size;
		// ロード状態
		enum dataRequestUnitStatus{
			DATAREQUESTUNITSTATUS_LOADING,
			DATAREQUESTUNITSTATUS_LOADED,
			DATAREQUESTUNITSTATUS_CANCEL,
		} status;
	} *reqList;
} localGlobal = {0};

// ----------------------------------------------------------------

// リクエストデータ作成
static struct dataRequestUnit *reqDataCreate(){
	struct dataRequestUnit *this = (struct dataRequestUnit*)calloc(1, sizeof(struct dataRequestUnit));
	// リストにデータ追加
	if(localGlobal.reqList == NULL){
		localGlobal.reqList = this;
	}else{
		struct dataRequestUnit *temp = localGlobal.reqList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = this;
	}
	this->next = NULL;
	this->reqId = ++localGlobal.reqIdCount;
	this->buff = NULL;
	this->size = 0;
	this->status = DATAREQUESTUNITSTATUS_LOADING;
	return this;
}

// リクエストデータ解放
static void reqDataFree(struct dataRequestUnit *this){
	if(this->status == DATAREQUESTUNITSTATUS_LOADED){
		// 解放
		if(this->buff != NULL){free(this->buff);}
		free(this);
	}else{
		// ロードが完了していないのでコールバックで破棄
		this->status = DATAREQUESTUNITSTATUS_CANCEL;
	}
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct dataRequestUnit *this = (struct dataRequestUnit*)param;

	if(this->status == DATAREQUESTUNITSTATUS_LOADING){
		// ロード完了
		this->buff = buff;
		this->size = size;
		this->status = DATAREQUESTUNITSTATUS_LOADED;
	}else if(this->status == DATAREQUESTUNITSTATUS_CANCEL){
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
	struct dataRequestUnit *this = reqDataCreate();
	platformPluginDataHttp(this, url, request, callback);
	return this->reqId;
}

// ローカルファイルリクエスト
dataRequestId dataRequestLocal(char *src){
	struct dataRequestUnit *this = reqDataCreate();
	platformPluginDataLocal(this, src, callback);
	return this->reqId;
}

// ----------------------------------------------------------------

// リクエスト応答
bool dataRequestResponse(dataRequestId reqId, void **buff, size_t *size){
	struct dataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			if(temp->status == DATAREQUESTUNITSTATUS_LOADED){
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
	struct dataRequestUnit *prev = NULL;
	struct dataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			struct dataRequestUnit *dispose = temp;
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
	struct dataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		struct dataRequestUnit *dispose = temp;
		temp = temp->next;
		// 解放
		reqDataFree(dispose);
	}
	localGlobal.reqList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

