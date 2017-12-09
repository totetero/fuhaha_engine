#include "../../library.h"
#include "../../plugin/pluginData.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineData/engineData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// リクエストリスト
	int reqIdCount;
	struct engineDataRequestUnit{
		struct engineDataRequestUnit *next;
		engineDataRequestId reqId;
		void *buff;
		size_t size;
		// ロード状態
		enum engineDataRequestUnitStatus{
			ENGINEDATAREQUESTUNITSTATUS_LOADING,
			ENGINEDATAREQUESTUNITSTATUS_LOADED,
			ENGINEDATAREQUESTUNITSTATUS_CANCEL,
		} status;
	} *reqList;
} localGlobal = {0};

// ----------------------------------------------------------------

// リクエストデータ作成
static struct engineDataRequestUnit *reqDataCreate(void){
	struct engineDataRequestUnit *this = (struct engineDataRequestUnit*)engineUtilMemoryInfoCalloc("engineDataRequest body", 1, sizeof(struct engineDataRequestUnit));
	// リストにデータ追加
	if(localGlobal.reqList == NULL){
		localGlobal.reqList = this;
	}else{
		struct engineDataRequestUnit *temp = localGlobal.reqList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = this;
	}
	this->next = NULL;
	this->reqId = ++localGlobal.reqIdCount;
	this->buff = NULL;
	this->size = 0;
	this->status = ENGINEDATAREQUESTUNITSTATUS_LOADING;
	return this;
}

// リクエストデータ解放
static void reqDataFree(struct engineDataRequestUnit *this){
	if(this->status == ENGINEDATAREQUESTUNITSTATUS_LOADED){
		// 解放
		if(this->buff != NULL){engineUtilMemoryInfoFree("engineDataRequest buff", this->buff); this->buff = NULL;}
		engineUtilMemoryInfoFree("engineDataRequest body", this);
	}else{
		// ロードが完了していないのでコールバックで破棄
		this->status = ENGINEDATAREQUESTUNITSTATUS_CANCEL;
	}
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct engineDataRequestUnit *this = (struct engineDataRequestUnit*)param;
	enum engineDataRequestUnitStatus beforeStatus = this->status;
	this->status = ENGINEDATAREQUESTUNITSTATUS_LOADED;
	if(beforeStatus == ENGINEDATAREQUESTUNITSTATUS_LOADING){
		// ロード完了
		this->buff = buff;
		this->size = size;
	}else if(beforeStatus == ENGINEDATAREQUESTUNITSTATUS_CANCEL){
		// ロード中止
		reqDataFree(this);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 通信リクエスト
engineDataRequestId engineDataRequestHttp(char *url, char *request){
	struct engineDataRequestUnit *this = reqDataCreate();
	platformPluginDataHttp(this, url, request, callback);
	return this->reqId;
}

// ローカルファイルリクエスト
engineDataRequestId engineDataRequestLocal(char *src){
	struct engineDataRequestUnit *this = reqDataCreate();
	platformPluginDataLocal(this, src, callback);
	return this->reqId;
}

// ----------------------------------------------------------------

// リクエスト応答
bool engineDataRequestResponse(engineDataRequestId reqId, void **buff, size_t *size){
	struct engineDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			if(temp->status == ENGINEDATAREQUESTUNITSTATUS_LOADED){
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
void engineDataRequestFree(engineDataRequestId reqId){
	struct engineDataRequestUnit *prev = NULL;
	struct engineDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			struct engineDataRequestUnit *dispose = temp;
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
void engineDataRequestFreeAll(void){
	struct engineDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		struct engineDataRequestUnit *dispose = temp;
		temp = temp->next;
		// 解放
		reqDataFree(dispose);
	}
	localGlobal.reqList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

