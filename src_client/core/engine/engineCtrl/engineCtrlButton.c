#include "../../library.h"
#include "engineCtrl.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン状態構造体 計算
void engineCtrlButtonCalc(struct engineCtrlButton *this, void *innerParam, bool(*isInner)(void *innerParam, void *touchParam)){
	// タッチ処理
	struct engineCtrlTouch *t = engineCtrlTouchGet(this->setting.touchId);
	if(t != NULL){
		if(t->dn){
			if(this->setting.isInactiveCalc){
				// ボタン無効状態
				this->status.isTouchActive = false;
			}else if(t->mv && this->setting.isInactiveMove){
				// 移動したらボタン無効化
				this->status.isTouchActive = false;
			}else{
				// ボタン押下中
				this->status.isTouchActive = (isInner(innerParam, t) != this->setting.isOutside);
			}

			if(!t->active){
				// タッチ開始時
				if(this->status.isTouchActive){engineCtrlTouchOwn();}
			}
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
			if(this->status.isTouchActive){this->status.isTrigger = true;}
			this->status.isTouchActive = false;
		}
	}else{
		// タッチ無し
		this->status.isTouchActive = false;
	}

	// 対応キー押下確認
	bool isKeyActive = false;
	isKeyActive = isKeyActive || (this->setting.isZKey && global.key.zb.isActive);
	isKeyActive = isKeyActive || (this->setting.isXKey && global.key.xb.isActive);
	isKeyActive = isKeyActive || (this->setting.isCKey && global.key.cb.isActive);
	isKeyActive = isKeyActive || (this->setting.isVKey && global.key.vb.isActive);
	if(!isKeyActive && this->status.isKeyActive){this->status.isTrigger = true;}
	this->status.isKeyActive = isKeyActive;

	// ボタン押下状態
	this->status.isActive = (this->status.isTouchActive || this->status.isKeyActive);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

