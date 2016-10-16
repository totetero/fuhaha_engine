#include "../../library.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 要素の破棄
static void disposeOne(struct engineGraphicTransManager *this, int index, bool isAll){
	struct engineGraphicTrans *trans = this->transList[index];
	if(trans == NULL){return;}
	// 要素をリストから取り除く
	this->transList[index] = NULL;
	if(!isAll){
		// 子要素も再帰にて破棄
		for(int i = 0; i < this->transLength; i++){
			struct engineGraphicTrans *temp = this->transList[i];
			if(temp != NULL && temp->parent == trans){disposeOne(this, i, isAll);}
		}
	}
	// 破棄する
	trans->isExist = false;
	if(trans->dispose != NULL){trans->dispose(trans);}
}

// もう使わない要素の破棄
static void disposeMeny(struct engineGraphicTransManager *this, bool isAll){
	for(int i = 0; i < this->transLength; i++){
		struct engineGraphicTrans *trans = this->transList[i];
		if(trans == NULL){continue;}
		if(isAll || !trans->isExist){disposeOne(this, i, isAll);}
	}
}

// 変形管理構造体 リセット
void engineGraphicTransManagerReset(struct engineGraphicTransManager *this){
	// 要素を全て破棄
	disposeMeny(this, true);
}

// 変形管理構造体 追加
void engineGraphicTransManagerPush(struct engineGraphicTransManager *this, struct engineGraphicTrans *trans){
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
static int engineGraphicTransList_sort(const void *a, const void *b){
	struct engineGraphicTrans *trans1 = *(struct engineGraphicTrans**)a;
	struct engineGraphicTrans *trans2 = *(struct engineGraphicTrans**)b;
	double zIndex1 = (trans1 != NULL) ? trans1->zIndex : 0;
	double zIndex2 = (trans2 != NULL) ? trans2->zIndex : 0;
	return (zIndex1 - zIndex2);
}

// 変形管理構造体 追加したすべての要素を描画
void engineGraphicTransManagerDraw(struct engineGraphicTransManager *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// もう使わない要素の破棄
	disposeMeny(this, false);
	// 並べ替え
	qsort((void*)this->transList, this->transLength, sizeof(struct engineGraphicTrans*), engineGraphicTransList_sort);
	// 描画
	for(int i = 0; i < this->transLength; i++){
		struct engineGraphicTrans *trans = this->transList[i];
		if(trans == NULL || trans->draw == NULL){continue;}
		// 先祖を遡って表示確認
		bool isVisible = true;
		struct engineGraphicTrans *temp = trans;
		while(temp != NULL && isVisible){isVisible = temp->isVisible; temp = temp->parent;}
		if(!isVisible){continue;}
		// 描画する
		trans->draw(trans, mat, color);
	}
}

// 変形管理構造体 破棄
void engineGraphicTransManagerDispose(struct engineGraphicTransManager *this){
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

