#include "../../library.h"
#include "engineUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct engineUtilMemoryUnit{
		struct engineUtilMemoryUnit *next;
		void *ptr;
		char info[64];
	} *datList;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 独自mallocの実装
void* engineUtilMemoryMallocImplement(char *info, size_t size){
	// データ作成
	struct engineUtilMemoryUnit *this = (struct engineUtilMemoryUnit*)calloc(1, sizeof(struct engineUtilMemoryUnit));
	this->ptr = malloc(size);
	strncpy(this->info, info, sizeof(this->info));
	// リストにデータ追加
	this->next = localGlobal.datList;
	localGlobal.datList = this;
	// データを返す
	return this->ptr;
}

// 独自callocの実装
void* engineUtilMemoryCallocImplement(char *info, size_t n, size_t size){
	// データ作成
	struct engineUtilMemoryUnit *this = (struct engineUtilMemoryUnit*)calloc(1, sizeof(struct engineUtilMemoryUnit));
	this->ptr = calloc(n, size);
	strncpy(this->info, info, sizeof(this->info));
	// リストにデータ追加
	this->next = localGlobal.datList;
	localGlobal.datList = this;
	// データを返す
	return this->ptr;
}

// 独自freeの実装
void engineUtilMemoryFreeImplement(void *ptr){
	struct engineUtilMemoryUnit *prev = NULL;
	struct engineUtilMemoryUnit *temp = localGlobal.datList;
	while(temp != NULL){
		if((void*)temp->ptr == ptr){
			struct engineUtilMemoryUnit *dispose = temp;
			// 次のを装填
			if(prev == NULL){localGlobal.datList = temp->next;}
			else{prev->next = temp->next;}
			temp = temp->next;
			// 解放
			free(dispose);
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
}

// 独自に確保したメモリ領域の確認
void engineUtilMemoryTraceImplement(){
	trace("mem trace\n");
	struct engineUtilMemoryUnit *temp = localGlobal.datList;
	while(temp != NULL){
		trace("mem %s\n", temp->info);
		temp = temp->next;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

