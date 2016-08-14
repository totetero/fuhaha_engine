#include "../../library.h"
#include "engineUtil.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct engineUtilMemoryUnit{
		void *ptr;
		size_t size;
		char info[128];
		int count;
		bool isPermanent;
	} **datList;
	int datLength;
	size_t debugSize;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static void* addDatList(char *info, void *ptr, size_t size){
	if(ptr == NULL){trace("mem alloc error %s\n", info); return NULL;}

	// データの挿入先を探す
	int index = -1;
	// リストの空きを探す
	for(int i = 0; i < localGlobal.datLength; i++){
		struct engineUtilMemoryUnit *temp = localGlobal.datList[i];
		if(temp == NULL || temp->ptr == NULL){index = i; break;}
	}
	if(index < 0){
		// リストに空きがなかったらリスト拡張
		index = localGlobal.datLength;
		int addNum = 100;
		int length = index + addNum;
		void *list = calloc(length, sizeof(*localGlobal.datList));
		if(localGlobal.datLength > 0){
			memcpy(list, localGlobal.datList, localGlobal.datLength * sizeof(*localGlobal.datList));
			free(localGlobal.datList);
		}
		localGlobal.datList = list;
		localGlobal.datLength = length;
		localGlobal.debugSize += addNum * sizeof(*localGlobal.datList);
	}

	// データ挿入
	struct engineUtilMemoryUnit *unit = localGlobal.datList[index];
	if(unit == NULL){
		localGlobal.datList[index] = (struct engineUtilMemoryUnit*)calloc(1, sizeof(struct engineUtilMemoryUnit));
		localGlobal.debugSize += sizeof(struct engineUtilMemoryUnit);
		unit = localGlobal.datList[index];
	}
	unit->ptr = ptr;
	unit->size = size;
	strncpy(unit->info, info, sizeof(unit->info) - 1);
	unit->info[sizeof(unit->info) - 1] = '\0';
	unit->count = 0;
	char *key = "(permanent)";
	unit->isPermanent = (strncmp(info, key, strlen(key)) == 0);
	return ptr;
}

// 独自mallocの実装
void* engineUtilMemoryMallocImplement(char *info, size_t size){
	if(size <= 0){trace("warning alloc size zero"); size = 1;}
	return addDatList(info, malloc(size), size);
}

// 独自callocの実装
void* engineUtilMemoryCallocImplement(char *info, size_t n, size_t size){
	if(size <= 0){trace("warning alloc size zero"); size = 1;}
	return addDatList(info, calloc(n, size), n * size);
}

// 独自freeの実装
void engineUtilMemoryFreeImplement(char *info, void *ptr){
	if(ptr == NULL){return;}
	int freeCount = 0;
	for(int i = 0; i < localGlobal.datLength; i++){
		struct engineUtilMemoryUnit *temp = localGlobal.datList[i];
		if(temp == NULL || temp->ptr != ptr){continue;}
		free(temp->ptr);
		temp->ptr = NULL;
		temp->size = 0;
		freeCount++;
	}
	if(freeCount < 1){trace("mem free error -- no such pointer %s\n", info);}
	if(freeCount > 1){trace("mem free error -- too meny pointer %s\n", info);}
}

// 並べ替え関数
static int datList_sort(const void *a, const void *b){
	struct engineUtilMemoryUnit *dat1 = *(struct engineUtilMemoryUnit**)a;
	struct engineUtilMemoryUnit *dat2 = *(struct engineUtilMemoryUnit**)b;

	bool isPermanent1 = (dat1 != NULL) ? dat1->isPermanent : false;
	bool isPermanent2 = (dat2 != NULL) ? dat2->isPermanent : false;
	if(isPermanent1 && !isPermanent2){return -1;}
	if(!isPermanent1 && isPermanent2){return  1;}

	double count1 = (dat1 != NULL) ? dat1->count : 0;
	double count2 = (dat2 != NULL) ? dat2->count : 0;
	int countDiff = count1 - count2;
	if(countDiff != 0){return countDiff;}

	char *info1 = (dat1 != NULL) ? dat1->info : "";
	char *info2 = (dat2 != NULL) ? dat2->info : "";
	int infoDiff = strcmp(info1, info2);
	if(infoDiff != 0){return infoDiff;}

	double size1 = (dat1 != NULL) ? dat1->size : 0;
	double size2 = (dat2 != NULL) ? dat2->size : 0;
	int sizeDiff = size1 - size2;
	if(sizeDiff != 0){return sizeDiff;}

	return 0;
}

// 独自に確保したメモリ領域の確認
void engineUtilMemoryTraceImplement(char *info){
	// メモリサイズ計算
	size_t memorySize = 0;
	size_t permanentSize = 0;
	for(int i = 0; i < localGlobal.datLength; i++){
		struct engineUtilMemoryUnit *temp = localGlobal.datList[i];
		if(temp == NULL || temp->ptr == NULL){continue;}
		if(temp->isPermanent){permanentSize += temp->size;}
		if(!temp->isPermanent){memorySize += temp->size;}
	}
	char dstr[32];
	char mstr[32];
	char pstr[32];
	snprintf(dstr, sizeof(dstr), "%zu.%zukB", localGlobal.debugSize / 1000, localGlobal.debugSize % 1000);
	snprintf(mstr, sizeof(mstr), "%zu.%zukB", memorySize / 1000, memorySize % 1000);
	snprintf(pstr, sizeof(pstr), "%zu.%zukB", permanentSize / 1000, permanentSize % 1000);
	trace("---------------- %s ----------------\n", info);
	trace("mem trace %s %s (+%s) ----------------\n", mstr, pstr, dstr);

	qsort((void*)localGlobal.datList, localGlobal.datLength, sizeof(struct engineUtilMemoryUnit**), datList_sort);
	for(int i = 0; i < localGlobal.datLength; i++){
		struct engineUtilMemoryUnit *temp = localGlobal.datList[i];
		if(temp == NULL || temp->ptr == NULL){continue;}
		trace("mem %2d %8zu %s\n", temp->count++, temp->size, temp->info);
	}

	trace("---------------- ----------------\n");
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

