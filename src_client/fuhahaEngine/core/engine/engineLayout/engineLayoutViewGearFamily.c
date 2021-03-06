#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct engineLayoutView viewRoot;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素 全ての要素の親となる要素を取得
static struct engineLayoutView *viewRootGet(void){
	return &localGlobal.viewRoot;
}

// ----------------------------------------------------------------

// 双方向リストにおいて境界条件を無視するためのダミー設定
static void dummySet(struct engineLayoutView *this, struct engineLayoutView *dummy){
	if(this->family.childrenHead != NULL && this->family.childrenTail != NULL){
		dummy->family.next = this->family.childrenHead;
		dummy->family.prev = this->family.childrenTail;
		dummy->family.next->family.prev = dummy;
		dummy->family.prev->family.next = dummy;
	}else{
		// 要素がない場合
		dummy->family.next = dummy;
		dummy->family.prev = dummy;
	}
}

// ダミー設定を本体に反映
static void dummyUnset(struct engineLayoutView *this, struct engineLayoutView *dummy){
	if(dummy != dummy->family.next && dummy != dummy->family.prev){
		dummy->family.next->family.prev = NULL;
		dummy->family.prev->family.next = NULL;
		this->family.childrenHead = dummy->family.next;
		this->family.childrenTail = dummy->family.prev;
	}else{
		// 要素がない場合
		this->family.childrenHead = NULL;
		this->family.childrenTail = NULL;
	}
}

// ----------------------------------------------------------------

// 子要素追加
static void familyAdd(struct engineLayoutView *this, struct engineLayoutView *child){
	if(this == NULL){return;}
	if(child->family.parent == this){return;}
	if(child == viewRootGet()){return;}

	struct engineLayoutView dummy;
	dummySet(this, &dummy);

	// 子要素の親設定
	child->family.parent = this;

	// childをdummyの前に挿入
	child->family.prev = dummy.family.prev;
	child->family.next = &dummy;
	dummy.family.prev->family.next = child;
	dummy.family.prev = child;

	dummyUnset(this, &dummy);
}

// 子要素除外
static void familyRemove(struct engineLayoutView *this, struct engineLayoutView *child){
	if(this == NULL){return;}
	if(child->family.parent != this){return;}

	struct engineLayoutView dummy;
	dummySet(this, &dummy);

	// childをリストから外す
	child->family.prev->family.next = child->family.next;
	child->family.next->family.prev = child->family.prev;
	child->family.prev = NULL;
	child->family.next = NULL;

	// 子要素の親設定
	child->family.parent = NULL;

	dummyUnset(this, &dummy);
}

// ----------------------------------------------------------------

// 表示要素構造体子要素 初期化
void engineLayoutViewGearFamilyInit(struct engineLayoutView *this){
	// ルートに貼り付ける
	familyAdd(viewRootGet(), this);
}

// 表示要素構造体子要素 子要素追加
void engineLayoutViewGearFamilyAdd(struct engineLayoutView *this, struct engineLayoutView *child){
	if(child->family.parent == this){return;}
	familyRemove(child->family.parent, child);
	familyAdd(this, child);

	// 要素を追加された時の情報を記録しておく
	// 要素が追加されたフレームでは計算を行わず、次のフレームから計算を行う
	// 要素が追加されてから一度でも計算を行わない限り描画は行わない
	child->family.addInfo.frameCount = engineUtilFrameCountGet();
	child->family.addInfo.isFirstCalc = false;
}

// 表示要素構造体子要素 子要素除外
void engineLayoutViewGearFamilyRemove(struct engineLayoutView *this, struct engineLayoutView *child, bool isDispose){
	if(child->family.parent != this){return;}
	familyRemove(this, child);
	if(isDispose){
		// 子要素破棄
		child->pause(child);
		child->dispose(child);
	}else{
		// ルートに張り替える
		familyAdd(viewRootGet(), child);
	}
}

// 表示要素構造体子要素 破棄
void engineLayoutViewGearFamilyDispose(struct engineLayoutView *this){
	// 完全に取り除く
	familyRemove(this->family.parent, this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static int compare(struct engineLayoutView *insert, struct engineLayoutView *find){
	return insert->family.zIndex - find->family.zIndex;
}

// 子要素を並び替える 挿入ソート
static void sortChildren(struct engineLayoutView *this){
	struct engineLayoutView dummy;
	dummySet(this, &dummy);

	struct engineLayoutView *insert = dummy.family.next;
	while(insert != &dummy){
		struct engineLayoutView *keepNext = insert->family.next;
		struct engineLayoutView *find = dummy.family.next;
		while(find != insert){
			if(compare(insert, find) < 0){
				// insertをリストから外す
				insert->family.prev->family.next = insert->family.next;
				insert->family.next->family.prev = insert->family.prev;
				// insertをfindの前に挿入
				insert->family.prev = find->family.prev;
				insert->family.next = find;
				find->family.prev->family.next = insert;
				find->family.prev = insert;
				break;
			}
			find = find->family.next;
		}
		insert = keepNext;
	}

	dummyUnset(this, &dummy);
}

// 表示要素構造体子要素 タッチ処理
bool engineLayoutViewGearChildrenTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	if(this == viewRootGet()){return false;}
	bool isActive = false;
	// 子要素のタッチ処理 表示手前から先に処理する
	struct engineLayoutView *temp = this->family.childrenTail;
	while(temp != NULL){
		isCancel = isCancel || isActive || temp->family.isInactive;
		isActive = temp->touch(temp, touchIndex, x, y, dn, mv, isCancel) || isActive;
		temp = temp->family.prev;
	}
	return isActive;
}

// 表示要素構造体子要素 計算
void engineLayoutViewGearChildrenCalc(struct engineLayoutView *this, bool isCancel){
	if(this == viewRootGet()){return;}
	// 子要素の並べ替え
	sortChildren(this);
	// 子要素の計算
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		bool isCalc = !temp->family.isInactive;
		isCalc = (isCalc && temp->family.addInfo.frameCount != engineUtilFrameCountGet());
		if(isCalc){
			temp->family.addInfo.isFirstCalc = true;
			temp->calc(temp, isCancel);
		}
		temp = temp->family.next;
	}
}

// 表示要素構造体子要素 描画
void engineLayoutViewGearChildrenDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	if(this == viewRootGet()){return;}
	// 子要素の描画 表示奥から先に処理する
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		bool isDraw = !temp->family.isInvisible;
		isDraw = (isDraw && temp->family.addInfo.isFirstCalc);
		if(isDraw){
			struct engineMathVector4 tempColor1;
			tempColor1.r = temp->color.r * color->r;
			tempColor1.g = temp->color.g * color->g;
			tempColor1.b = temp->color.b * color->b;
			tempColor1.a = temp->color.a * color->a;
			temp->draw(temp, mat, &tempColor1);
		}
		temp = temp->family.next;
	}
}

// 表示要素構造体子要素 一時停止
void engineLayoutViewGearChildrenPause(struct engineLayoutView *this){
	if(this == viewRootGet()){return;}
	// 子要素の一時停止
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		temp->pause(temp);
		temp = temp->family.next;
	}
}

// 表示要素構造体子要素 破棄
void engineLayoutViewGearChildrenDispose(struct engineLayoutView *this){
	if(this == viewRootGet()){return;}
	// 子要素の破棄
	struct engineLayoutView *temp = this->family.childrenHead;
	while(temp != NULL){
		struct engineLayoutView *dispose = temp;
		temp = temp->family.next;
		dispose->family.parent = NULL;
		dispose->family.prev = NULL;
		dispose->family.next = NULL;
		dispose->dispose(dispose);
	}
	this->family.childrenHead = NULL;
	this->family.childrenTail = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

