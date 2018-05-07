#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体対話型 タッチ処理
bool engineLayoutViewUtilInteractTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	isCancel = (isCancel || !this->interact.setting.isTouchable);
	if(!this->interact.status.isActive){
		if(dn && !mv && !isCancel && this->position.isInner(this, x, y)){
			// タッチ開始
			this->interact.status.touchIndex = touchIndex;
			this->interact.status.isActive = true;
			this->interact.status.isHover = true;
			this->interact.status.isMove = false;
			this->interact.status.isTriggerDn = true;
			this->interact.status.startX = x;
			this->interact.status.startY = y;
			this->interact.status.currX = x;
			this->interact.status.currY = y;
			return true;
		}else{
			// タッチ開始できない
			return false;
		}
	}else if(this->interact.status.touchIndex == touchIndex){
		if(isCancel){
			// タッチキャンセル
			this->interact.status.isActive = false;
			this->interact.status.isHover = false;
			this->interact.status.currX = x;
			this->interact.status.currY = y;
			return false;
		}else if(dn){
			// タッチ途中
			this->interact.status.isHover = this->position.isInner(this, x, y);
			this->interact.status.currX = x;
			this->interact.status.currY = y;
			if(mv && !this->interact.status.isMove){
				int dx = x - this->interact.status.startX;
				int dy = y - this->interact.status.startY;
				if(dx * dx + dy * dy > 5 * 5){
					if(!this->interact.status.isMove && this->interact.setting.isMoveableX && this->interact.setting.isMoveableY){this->interact.status.isMove = true;}
					if(!this->interact.status.isMove && this->interact.setting.isMoveableX && engineMathAbs(dx) > engineMathAbs(dy)){this->interact.status.isMove = true;}
					if(!this->interact.status.isMove && this->interact.setting.isMoveableY && engineMathAbs(dx) < engineMathAbs(dy)){this->interact.status.isMove = true;}
				}
			}
			return true;
		}else{
			// タッチ完了
			this->interact.status.isActive = false;
			this->interact.status.isHover = false;
			this->interact.status.currX = x;
			this->interact.status.currY = y;
			if(this->position.isInner(this, x, y)){this->interact.status.isTriggerUp = true;}
			return true;
		}
	}else{
		// 関係がない
		return false;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体対話型 基盤タッチ処理
void engineLayoutViewUtilInteractTouchRoot(struct engineLayoutView *this, bool isCancel){
	enum engineCtrlTouchId touchIdList[] = {ENGINECTRLTOUCHID_TEST1, ENGINECTRLTOUCHID_TEST2};
	for(int i = 0; i < (int)(sizeof(touchIdList) / sizeof(*touchIdList)); i++){
		struct engineCtrlTouch *t = engineCtrlTouchGet(touchIdList[i]);
		if(t == NULL){continue;}
		if(t->dn && !t->active){engineCtrlTouchOwn();}else if(!t->dn){engineCtrlTouchFree();}
		engineLayoutViewTouch(this, i, t->screen.x, t->screen.y, t->dn, t->mv, isCancel);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体対話型 ボタンタッチ確認
bool engineLayoutViewUtilInteractIsTriggerUp(struct engineLayoutView *this){
	bool isTriggerUp = this->interact.status.isTriggerUp;
	this->interact.status.isTriggerUp = false;
	return isTriggerUp;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

