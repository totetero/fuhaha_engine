#include "../../library.h"
#include "./engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 双方向リストにおいて境界条件を無視するためのダミー設定
static void dummySet(struct engineLayout02View *this, struct engineLayout02View *dummy){
	if(this->children.childrenHead != NULL && this->children.childrenTail != NULL){
		dummy->children.next = this->children.childrenHead;
		dummy->children.prev = this->children.childrenTail;
		dummy->children.next->children.prev = dummy;
		dummy->children.prev->children.next = dummy;
	}else{
		// 要素がない場合
		dummy->children.next = dummy;
		dummy->children.prev = dummy;
	}
}

// ダミー設定を本体に反映
static void dummyUnset(struct engineLayout02View *this, struct engineLayout02View *dummy){
	if(dummy != dummy->children.next && dummy != dummy->children.prev){
		dummy->children.next->children.prev = NULL;
		dummy->children.prev->children.next = NULL;
		this->children.childrenHead = dummy->children.next;
		this->children.childrenTail = dummy->children.prev;
	}else{
		// 要素がない場合
		this->children.childrenHead = NULL;
		this->children.childrenTail = NULL;
	}
}

// 表示要素構造体子要素 子要素追加
void engineLayout02ViewUtilChildrenAdd(struct engineLayout02View *this, struct engineLayout02View *child){
	if(child->children.parent == this){return;}
	if(child->children.parent != NULL){engineLayout02ViewUtilChildrenRemove(child->children.parent, child);}

	struct engineLayout02View dummy;
	dummySet(this, &dummy);

	// 子要素の親設定
	child->children.parent = this;

	// childをdummyの前に挿入
	child->children.prev = dummy.children.prev;
	child->children.next = &dummy;
	dummy.children.prev->children.next = child;
	dummy.children.prev = child;

	dummyUnset(this, &dummy);
}

// 表示要素構造体子要素 子要素排除
void engineLayout02ViewUtilChildrenRemove(struct engineLayout02View *this, struct engineLayout02View *child){
	if(child->children.parent != this){return;}

	struct engineLayout02View dummy;
	dummySet(this, &dummy);

	// childをリストから外す
	child->children.prev->children.next = child->children.next;
	child->children.next->children.prev = child->children.prev;
	child->children.prev = NULL;
	child->children.next = NULL;

	// 子要素の親設定
	child->children.parent = NULL;

	dummyUnset(this, &dummy);
}

// ----------------------------------------------------------------

static int compare(struct engineLayout02View *insert, struct engineLayout02View *find){
	return insert->children.zIndex - find->children.zIndex;
}

// 子要素を並び替える 挿入ソート
static void sortChildren(struct engineLayout02View *this){
	struct engineLayout02View dummy;
	dummySet(this, &dummy);

	struct engineLayout02View *insert = dummy.children.next;
	while(insert != &dummy){
		struct engineLayout02View *keepNext = insert->children.next;
		struct engineLayout02View *find = dummy.children.next;
		while(find != insert){
			if(compare(insert, find) > 0){
				// insertをリストから外す
				insert->children.prev->children.next = insert->children.next;
				insert->children.next->children.prev = insert->children.prev;
				// insertをfindの前に挿入
				insert->children.prev = find->children.prev;
				insert->children.next = find;
				find->children.prev->children.next = insert;
				find->children.prev = insert;
				break;
			}
			find = find->children.next;
		}
		insert = keepNext;
	}

	dummyUnset(this, &dummy);
}

// 表示要素構造体子要素 計算
void engineLayout02ViewUtilChildrenCalc(struct engineLayout02View *this){
	// 子要素の並べ替え
	sortChildren(this);
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
		if(!temp->children.isInvisible){temp->draw(temp, mat, color);}
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
		dispose->children.parent = NULL;
		dispose->children.prev = NULL;
		dispose->children.next = NULL;
		dispose->dispose(dispose);
	}
	this->children.childrenHead = NULL;
	this->children.childrenTail = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

