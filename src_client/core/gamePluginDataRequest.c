#include "engine/engine.h"
#include "gamePluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// リクエストリスト
	uint32_t reqIdCount;
	struct corePluginDataRequestUnit{
		struct corePluginDataRequestUnit *next;
		corePluginDataRequestId reqId;
		void *buff;
		size_t size;
		// ロード状態
		enum corePluginDataRequestUnitStatus{
			COREPLUGINDATAREQUESTUNITSTATUS_LOADING,
			COREPLUGINDATAREQUESTUNITSTATUS_LOADED,
			COREPLUGINDATAREQUESTUNITSTATUS_CANCEL,
		} status;
	} *reqList;
} localGlobal = {0};

// ----------------------------------------------------------------

// リクエストデータ作成
static struct corePluginDataRequestUnit *reqDataCreate(){
	struct corePluginDataRequestUnit *this = (struct corePluginDataRequestUnit*)calloc(1, sizeof(struct corePluginDataRequestUnit));
	// リストにデータ追加
	if(localGlobal.reqList == NULL){
		localGlobal.reqList = this;
	}else{
		struct corePluginDataRequestUnit *temp = localGlobal.reqList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = this;
	}
	this->next = NULL;
	this->reqId = ++localGlobal.reqIdCount;
	this->buff = NULL;
	this->size = 0;
	this->status = COREPLUGINDATAREQUESTUNITSTATUS_LOADING;
	return this;
}

// リクエストデータ解放
static void reqDataFree(struct corePluginDataRequestUnit *this){
	if(this->status == COREPLUGINDATAREQUESTUNITSTATUS_LOADED){
		// 解放
		if(this->buff != NULL){free(this->buff);}
		free(this);
	}else{
		// ロードが完了していないのでコールバックで破棄
		this->status = COREPLUGINDATAREQUESTUNITSTATUS_CANCEL;
	}
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct corePluginDataRequestUnit *this = (struct corePluginDataRequestUnit*)param;

	if(this->status == COREPLUGINDATAREQUESTUNITSTATUS_LOADING){
		// ロード完了
		this->buff = buff;
		this->size = size;
		this->status = COREPLUGINDATAREQUESTUNITSTATUS_LOADED;
	}else if(this->status == COREPLUGINDATAREQUESTUNITSTATUS_CANCEL){
		// ロード中止
		if(this != NULL){free(this);}
		if(buff != NULL){free(buff);}
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 通信リクエスト
corePluginDataRequestId corePluginDataRequestHttp(char *url, char *request){
	struct corePluginDataRequestUnit *this = reqDataCreate();
	platformPluginDataHttp(this, url, request, callback);
	return this->reqId;
}

// ローカルファイルリクエスト
corePluginDataRequestId corePluginDataRequestLocal(char *src){
	struct corePluginDataRequestUnit *this = reqDataCreate();
	platformPluginDataLocal(this, src, callback);
	return this->reqId;
}

// ----------------------------------------------------------------

// リクエスト応答
bool corePluginDataRequestResponse(corePluginDataRequestId reqId, void **buff, size_t *size){
	struct corePluginDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			if(temp->status == COREPLUGINDATAREQUESTUNITSTATUS_LOADED){
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
void corePluginDataRequestFree(corePluginDataRequestId reqId){
	struct corePluginDataRequestUnit *prev = NULL;
	struct corePluginDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			struct corePluginDataRequestUnit *dispose = temp;
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
void corePluginDataRequestFreeAll(){
	struct corePluginDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		struct corePluginDataRequestUnit *dispose = temp;
		temp = temp->next;
		// 解放
		reqDataFree(dispose);
	}
	localGlobal.reqList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

