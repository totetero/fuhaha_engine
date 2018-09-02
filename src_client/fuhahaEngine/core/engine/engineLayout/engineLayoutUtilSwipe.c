#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// スワイプ構造体 タッチ処理
bool engineLayoutUtilSwipeTouch(struct engineLayoutUtilSwipe *this, struct engineLayoutView *view, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	// タッチ開始時に親要素の範囲外ならば子要素はタッチ開始できない
	if(dn && !mv && !view->position.isInner(view, x, y)){isCancel = true;}

	// スクロール開始したら子要素のタッチ処理をキャンセルする
	// ただし子要素がスクロール開始したら自身はスクロール開始できない
	this->isChildrenMove = false;
	bool isActive = false;
	bool isMove = (view->interact.status.isActive && view->interact.status.isMove);
	bool isActiveChild = engineLayoutViewGearChildrenTouch((struct engineLayoutView*)view, touchIndex, x, y, dn, mv, isCancel || isActive || isMove); isActive = isActiveChild || isActive;
	bool isChildrenMove = this->isChildrenMove;
	bool isActiveLocal = engineLayoutViewGearInteractTouch((struct engineLayoutView*)view, touchIndex, x, y, dn, mv, isCancel || isChildrenMove); isActive = isActiveLocal || isActive;

	// 必要ならば親要素に子要素がスクロールしたことを伝える
	if(this->parentSwipe != NULL){
		isMove = (view->interact.status.isActive && view->interact.status.isMove);
		if(isMove || isChildrenMove){this->parentSwipe->isChildrenMove = true;}
	}

	if(isActiveLocal){
		// ローカル座標変換
		struct engineMathVector3 tempVec1;
		engineMathVec3Set(&tempVec1, x, y, 0);
		engineLayoutViewGearPositionTransformCalcInvert((struct engineLayoutView*)view, &tempVec1);
		// 新タッチ位置更新
		this->x = tempVec1.x;
		this->y = tempVec1.y;
	}

	return isActive;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

