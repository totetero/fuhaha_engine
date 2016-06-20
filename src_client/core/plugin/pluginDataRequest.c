#include "../library.h"
#include "pluginData.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	// リクエストリスト
	int reqIdCount;
	struct pluginDataRequestUnit{
		struct pluginDataRequestUnit *next;
		pluginDataRequestId reqId;
		void *buff;
		size_t size;
		// ロード状態
		enum pluginDataRequestUnitStatus{
			PLUGINDATAREQUESTUNITSTATUS_LOADING,
			PLUGINDATAREQUESTUNITSTATUS_LOADED,
			PLUGINDATAREQUESTUNITSTATUS_CANCEL,
		} status;
	} *reqList;
} localGlobal = {0};

// ----------------------------------------------------------------

// リクエストデータ作成
static struct pluginDataRequestUnit *reqDataCreate(void){
	struct pluginDataRequestUnit *this = (struct pluginDataRequestUnit*)calloc(1, sizeof(struct pluginDataRequestUnit));
	// リストにデータ追加
	if(localGlobal.reqList == NULL){
		localGlobal.reqList = this;
	}else{
		struct pluginDataRequestUnit *temp = localGlobal.reqList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = this;
	}
	this->next = NULL;
	this->reqId = ++localGlobal.reqIdCount;
	this->buff = NULL;
	this->size = 0;
	this->status = PLUGINDATAREQUESTUNITSTATUS_LOADING;
	return this;
}

// リクエストデータ解放
static void reqDataFree(struct pluginDataRequestUnit *this){
	if(this->status == PLUGINDATAREQUESTUNITSTATUS_LOADED){
		// 解放
		if(this->buff != NULL){free(this->buff);}
		free(this);
	}else{
		// ロードが完了していないのでコールバックで破棄
		this->status = PLUGINDATAREQUESTUNITSTATUS_CANCEL;
	}
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct pluginDataRequestUnit *this = (struct pluginDataRequestUnit*)param;

	if(this->status == PLUGINDATAREQUESTUNITSTATUS_LOADING){
		// ロード完了
		this->buff = buff;
		this->size = size;
		this->status = PLUGINDATAREQUESTUNITSTATUS_LOADED;
	}else if(this->status == PLUGINDATAREQUESTUNITSTATUS_CANCEL){
		// ロード中止
		if(this != NULL){free(this);}
		if(buff != NULL){free(buff);}
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 通信リクエスト
pluginDataRequestId corePluginDataRequestHttp(char *url, char *request){
	struct pluginDataRequestUnit *this = reqDataCreate();
	platformPluginDataHttp(this, url, request, callback);
	return this->reqId;
}

// ローカルファイルリクエスト
pluginDataRequestId corePluginDataRequestLocal(char *src){
	struct pluginDataRequestUnit *this = reqDataCreate();
	platformPluginDataLocal(this, src, callback);
	return this->reqId;
}

// ----------------------------------------------------------------

// リクエスト応答
bool corePluginDataRequestResponse(pluginDataRequestId reqId, void **buff, size_t *size){
	struct pluginDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			if(temp->status == PLUGINDATAREQUESTUNITSTATUS_LOADED){
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
void corePluginDataRequestFree(pluginDataRequestId reqId){
	struct pluginDataRequestUnit *prev = NULL;
	struct pluginDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		if(temp->reqId == reqId){
			struct pluginDataRequestUnit *dispose = temp;
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
void corePluginDataRequestFreeAll(void){
	struct pluginDataRequestUnit *temp = localGlobal.reqList;
	while(temp != NULL){
		struct pluginDataRequestUnit *dispose = temp;
		temp = temp->next;
		// 解放
		reqDataFree(dispose);
	}
	localGlobal.reqList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

