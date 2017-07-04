#include "../../library.h"
#include "./engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素 子要素追加
void engineLayout02ViewUtilChildrenAdd(struct engineLayout02View *this, struct engineLayout02View *child){
	// 別の子要素でないか確認
	if(child->children.prev != NULL || this->children.childrenHead == child){return;}
	if(child->children.next != NULL || this->children.childrenTail == child){return;}
	if(child->children.parent != NULL){return;}
	if(this->children.childrenHead == NULL && this->children.childrenTail == NULL){
		// 最初の要素追加
		child->children.prev = NULL;
		child->children.next = NULL;
		child->children.parent = this;
		this->children.childrenHead = child;
		this->children.childrenTail = child;
	}else if(this->children.childrenHead == NULL){
		// ここには来ない
	}else if(this->children.childrenTail == NULL){
		// ここには来ない
	}else{
		// リストの最後に要素追加
		child->children.prev = this->children.childrenTail;
		child->children.next = NULL;
		child->children.parent = this;
		this->children.childrenTail->children.next = child;
		this->children.childrenTail = child;
	}
}

// 表示要素構造体子要素 子要素排除
void engineLayout02ViewUtilChildrenRemove(struct engineLayout02View *this, struct engineLayout02View *child){
	if(this->children.childrenHead == child && this->children.childrenTail == child){
		// 最後の要素排除
		this->children.childrenHead = NULL;
		this->children.childrenTail = NULL;
	}else if(this->children.childrenHead == child){
		// リストの最初から要素除去
		this->children.childrenHead = child->children.next;
		this->children.childrenHead->children.prev = NULL;
		child->children.next = NULL;
	}else if(this->children.childrenTail == child){
		// リストの最後から要素除去
		this->children.childrenTail = child->children.prev;
		this->children.childrenTail->children.next = NULL;
		child->children.prev = NULL;
	}else if(child->children.prev != NULL && child->children.next != NULL){
		// リストの途中から要素除去
		child->children.prev->children.next = child->children.next;
		child->children.next->children.prev = child->children.prev;
	}else{
		// ここには来ない
	}
	// 子要素でなくする
	child->children.parent = NULL;
	child->children.prev = NULL;
	child->children.next = NULL;
}

// ----------------------------------------------------------------

// 子要素を並び替える 挿入ソート
static void sortChildren(struct engineLayout02View *this){
//	struct engineLayout02View *list = this->childrenHead;
//	while(list != NULL){
//		struct engineLayout02View *next = list->next;
//		struct engineLayout02View *temp = this->childrenHead;
//		while(temp != list && temp != NULL){
//			if(isSwap(temp, list)){
//				list->prev->next = list->next;
//				break;
//			}
//			temp = temp->next;
//		}
//		list = next;
//	}
}

// 表示要素構造体子要素 計算
void engineLayout02ViewUtilChildrenCalc(struct engineLayout02View *this){
	// 子要素の計算
	struct engineLayout02View *temp = this->children.childrenHead;
	while(temp != NULL){
		temp->calc(temp);
		temp = temp->children.next;
	}
}

// 表示要素構造体子要素 描画
void engineLayout02ViewUtilChildrenDraw(struct engineLayout02View *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素の描画
	struct engineLayout02View *temp = this->children.childrenHead;
	while(temp != NULL){
		temp->draw(temp, mat, color);
		temp = temp->children.next;
	}
}

// 表示要素構造体子要素 一時停止
void engineLayout02ViewUtilChildrenPause(struct engineLayout02View *this){
	// 子要素の一時停止
	struct engineLayout02View *temp = this->children.childrenHead;
	while(temp != NULL){
		temp->pause(temp);
		temp = temp->children.next;
	}
}

// 表示要素構造体子要素 破棄
void engineLayout02ViewUtilChildrenDispose(struct engineLayout02View *this){
	// 子要素の破棄
	struct engineLayout02View *temp = this->children.childrenHead;
	while(temp != NULL){
		struct engineLayout02View *dispose = temp;
		temp = temp->children.next;
		engineLayout02ViewUtilChildrenRemove(this, dispose);
		dispose->dispose(dispose);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

