#include "../../library.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体描画情報 初期化
void engineLayoutViewUtilGraphicObjectInit(struct engineLayoutView *this){
	// 要素が一つの双方向循環リストを作成する
	this->graphicObject.prev = this;
	this->graphicObject.next = this;
}

// 表示要素構造体描画情報 破棄
void engineLayoutViewUtilGraphicObjectDispose(struct engineLayoutView *this){
	if(this->graphicObject.next == this){
		// 双方向循環リストに自分以外が存在しなければバッファを削除する
		engineGraphicObjectVBODispose(this->graphicObject.egoIdVert);
		engineGraphicObjectVBODispose(this->graphicObject.egoIdTexc);
		engineGraphicObjectIBODispose(this->graphicObject.egoIdFace);
		// バッファリセット
		this->graphicObject.isBufferChecked = false;
		this->graphicObject.egoIdVert = 0;
		this->graphicObject.egoIdTexc = 0;
		this->graphicObject.egoIdFace = 0;
	}else{
		// 双方向循環リストから外す
		engineLayoutViewUtilGraphicObjectDisconnect(this);
	}

	// 関数リセット
	this->graphicObject.shouldBufferCreate = NULL;
	this->graphicObject.bufferCreate = NULL;
}

// ----------------------------------------------------------------

// 表示要素構造体描画情報 関連づける
void engineLayoutViewUtilGraphicObjectConnect(struct engineLayoutView *this, struct engineLayoutView *view){
	// すでに関連づけられているか確認
	struct engineLayoutView *temp1 = this;
	do{temp1 = temp1->graphicObject.next;}while(temp1 != this && temp1 != view);
	if(temp1 == view){return;}

	// 既存のバッファを削除する
	struct engineLayoutView *tempList[2];
	tempList[0] = this;
	tempList[1] = view;
	for(int i = 0; i < (int)(sizeof(tempList) / sizeof(*tempList)); i++){
		struct engineLayoutView *temp2 = tempList[i];
		// バッファを削除する
		engineGraphicObjectVBODispose(temp2->graphicObject.egoIdVert);
		engineGraphicObjectVBODispose(temp2->graphicObject.egoIdTexc);
		engineGraphicObjectIBODispose(temp2->graphicObject.egoIdFace);
		// 双方向循環リストの全てに対してバッファリセットを行う
		struct engineLayoutView *temp3 = temp2;
		do{
			// バッファリセット
			temp3->graphicObject.isBufferChecked = false;
			temp3->graphicObject.egoIdVert = 0;
			temp3->graphicObject.egoIdTexc = 0;
			temp3->graphicObject.egoIdFace = 0;
			// 次の要素へ
			temp3 = temp3->graphicObject.next;
		}while(temp3 != temp2);
	}

	// 二つの双方向循環リストを繋げる
	struct engineLayoutView *thisNext = this->graphicObject.next;
	struct engineLayoutView *viewNext = view->graphicObject.next;
	thisNext->graphicObject.prev = view;
	viewNext->graphicObject.prev = this;
	this->graphicObject.next = viewNext;
	view->graphicObject.next = thisNext;
}

// 表示要素構造体描画情報 関連を外す
void engineLayoutViewUtilGraphicObjectDisconnect(struct engineLayoutView *this){
	// すでに双方向循環リストに自身以外が存在しなければ何もしない
	if(this->graphicObject.next == this){return;}

	// 双方向循環リストから外す
	struct engineLayoutView *thisPrev = this->graphicObject.prev;
	struct engineLayoutView *thisNext = this->graphicObject.next;
	thisPrev->graphicObject.next = thisNext;
	thisNext->graphicObject.prev = thisPrev;
	// 要素が一つの双方向循環リストを作成する
	this->graphicObject.prev = this;
	this->graphicObject.next = this;

	// バッファリセット
	this->graphicObject.isBufferChecked = false;
	this->graphicObject.egoIdVert = 0;
	this->graphicObject.egoIdTexc = 0;
	this->graphicObject.egoIdFace = 0;
}

// ----------------------------------------------------------------

// 表示要素構造体描画情報 バッファ作成
void engineLayoutViewUtilGraphicObjectBufferCreate(struct engineLayoutView *this){
	// バッファ作成が確認済みかどうかを確認
	bool isBufferChecked = true;
	struct engineLayoutView *temp1 = this;
	do{
		isBufferChecked = temp1->graphicObject.isBufferChecked && isBufferChecked;
		temp1 = temp1->graphicObject.next;
	}while(temp1 != this);
	if(isBufferChecked){return;}

	// バッファ作成の必要性を確認
	bool shouldBufferCreate = false;
	struct engineLayoutView *temp2 = this;
	do{
		shouldBufferCreate = (!temp2->graphicObject.isBufferChecked && temp2->graphicObject.shouldBufferCreate(temp2)) || shouldBufferCreate;
		temp2 = temp2->graphicObject.next;
	}while(temp2 != this);

	if(shouldBufferCreate){
		// バッファ作成開始
		engineGraphicBufferBegin();

		// 全要素のバッファ作成
		struct engineLayoutView *temp3 = this;
		do{
			temp3->graphicObject.bufferCreate(temp3);
			temp3 = temp3->graphicObject.next;
		}while(temp3 != this);

		// バッファ作成完了
		engineGraphicObjectVBOId egoIdVert = this->graphicObject.egoIdVert;
		engineGraphicObjectVBOId egoIdTexc = this->graphicObject.egoIdTexc;
		engineGraphicObjectIBOId egoIdFace = this->graphicObject.egoIdFace;
		engineGraphicBufferEnd(&egoIdVert, NULL, NULL, &egoIdTexc, &egoIdFace);

		// 全要素のバッファ記録
		struct engineLayoutView *temp4 = this;
		do{
			temp4->graphicObject.isBufferChecked = true;
			temp4->graphicObject.egoIdVert = egoIdVert;
			temp4->graphicObject.egoIdTexc = egoIdTexc;
			temp4->graphicObject.egoIdFace = egoIdFace;
			temp4 = temp4->graphicObject.next;
		}while(temp4 != this);
	}else{
		// バッファ確認済みフラグを立てる
		struct engineLayoutView *temp4 = this;
		do{
			temp4->graphicObject.isBufferChecked = true;
			temp4 = temp4->graphicObject.next;
		}while(temp4 != this);
	}
}

// 表示要素構造体描画情報 バッファ作成
void engineLayoutViewUtilGraphicObjectBufferCreateAll(struct engineLayoutView *this){
	// バッファを作成する
	engineLayoutViewUtilGraphicObjectBufferCreate(this);
	// 子要素に対して処理を適用する
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		engineLayoutViewUtilGraphicObjectBufferCreateAll(temp);
		temp = temp->family.next;
	}
}

// 表示要素構造体描画情報 バッファ作成
void engineLayoutViewUtilGraphicObjectBufferActiveAll(struct engineLayoutView *this){
	// バッファ確認済みフラグを寝かす
	this->graphicObject.isBufferChecked = false;
	// 子要素に対して処理を適用する
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		engineLayoutViewUtilGraphicObjectBufferActiveAll(temp);
		temp = temp->family.next;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

