#include "../../library.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "../engineCtrl/engineCtrl.h"
#include "../engineGraphic/engineGraphic.h"
#include "engineLayout01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 要素の破棄
static void disposeOne(struct engineLayout01TransManager *this, int index, bool isAll){
	struct engineLayout01Trans *trans = this->transList[index];
	if(trans == NULL){return;}
	// 要素をリストから取り除く
	this->transList[index] = NULL;
	if(!isAll){
		// 子要素も再帰にて破棄
		for(int i = 0; i < this->transLength; i++){
			struct engineLayout01Trans *temp = this->transList[i];
			if(temp != NULL && temp->parent == trans){disposeOne(this, i, isAll);}
		}
	}
	// 破棄する
	trans->isExist = false;
	if(trans->dispose != NULL){trans->dispose(trans);}
}

// もう使わない要素の破棄
static void disposeMeny(struct engineLayout01TransManager *this, bool isAll){
	for(int i = 0; i < this->transLength; i++){
		struct engineLayout01Trans *trans = this->transList[i];
		if(trans == NULL){continue;}
		if(isAll || !trans->isExist){disposeOne(this, i, isAll);}
	}
}

// 変形管理構造体 リセット
void engineLayout01TransManagerReset(struct engineLayout01TransManager *this){
	// 要素を全て破棄
	disposeMeny(this, true);
}

// 変形管理構造体 追加
void engineLayout01TransManagerPush(struct engineLayout01TransManager *this, struct engineLayout01Trans *trans){
	// もう使わない要素の破棄
	disposeMeny(this, false);
	// 同じ要素がすでにあるか確認
	for(int i = 0; i < this->transLength; i++){if(this->transList[i] == trans){return;}}
	// リストの空きを探す
	int index = -1;
	for(int i = 0; i < this->transLength; i++){if(this->transList[i] == NULL){index = i; break;}}
	if(index < 0){
		// リストに空きがなかったらリスト拡張
		index = this->transLength;
		int addNum = 10;
		int length = index + addNum;
		void *list = engineUtilMemoryCalloc(length, sizeof(*this->transList));
		if(this->transLength > 0){
			memcpy(list, this->transList, this->transLength * sizeof(*this->transList));
			engineUtilMemoryFree(this->transList);
		}
		this->transLength = length;
		this->transList = list;
	}

	// データ挿入
	trans->isExist = true;
	this->transList[index] = trans;
}

// 並べ替え関数
static int engineLayout01TransList_sort(const void *a, const void *b){
	struct engineLayout01Trans *trans1 = *(struct engineLayout01Trans**)a;
	struct engineLayout01Trans *trans2 = *(struct engineLayout01Trans**)b;
	double zIndex1 = (trans1 != NULL) ? trans1->zIndex : 0;
	double zIndex2 = (trans2 != NULL) ? trans2->zIndex : 0;
	return (zIndex1 - zIndex2);
}

// 変形管理構造体 追加したすべての要素を描画
void engineLayout01TransManagerDraw(struct engineLayout01TransManager *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// もう使わない要素の破棄
	disposeMeny(this, false);
	// 並べ替え
	qsort((void*)this->transList, this->transLength, sizeof(struct engineLayout01Trans*), engineLayout01TransList_sort);
	// 描画
	for(int i = 0; i < this->transLength; i++){
		struct engineLayout01Trans *trans = this->transList[i];
		if(trans == NULL || trans->draw == NULL){continue;}
		// 先祖を遡って表示確認
		bool isVisible = true;
		struct engineLayout01Trans *temp = trans;
		while(temp != NULL && isVisible){isVisible = temp->isVisible; temp = temp->parent;}
		if(!isVisible){continue;}
		// 描画する
		trans->draw(trans, mat, color);
	}
}

// 変形管理構造体 破棄
void engineLayout01TransManagerDispose(struct engineLayout01TransManager *this){
	// 要素を全て破棄
	disposeMeny(this, true);
	// 要素配列を破棄
	if(this->transList != NULL){engineUtilMemoryFree(this->transList);}
	this->transList = NULL;
	this->transLength = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

