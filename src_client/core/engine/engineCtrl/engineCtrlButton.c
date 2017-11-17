#include "../../library.h"
#include "engineCtrl.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン状態構造体 計算
void engineCtrlButtonCalc(struct engineCtrlButton *this, double x0, double y0, double x1, double y1){
	struct engineCtrlTouch *t = engineCtrlTouchGet(this->setting.touchId);
	bool isActiveBefore = this->status.isActive;

	// タッチ処理
	if(t != NULL){
		if(t->dn){
			if(this->setting.isInactiveCalc){
				// ボタン無効状態
				this->status.isActive = false;
			}else if(t->mv && this->setting.isInactiveMove){
				// 移動したらボタン無効化
				this->status.isActive = false;
			}else{
				// ボタン押下中
				bool isInner = (x0 < t->screen.x && t->screen.x < x1 && y0 < t->screen.y && t->screen.y < y1);
				this->status.isActive = (isInner != this->setting.isOutside);
			}

			if(!t->active){
				// タッチ開始時
				if(this->status.isActive){engineCtrlTouchOwn();}
			}
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
			this->status.isActive = false;
		}
	}else{
		// タッチ無し
		this->status.isActive = false;
	}

	// 対応キー押下確認
	bool isKeyActive = false;
	isKeyActive = isKeyActive || (this->setting.isZKey && global.key.zb.isActive);
	isKeyActive = isKeyActive || (this->setting.isXKey && global.key.xb.isActive);
	isKeyActive = isKeyActive || (this->setting.isCKey && global.key.cb.isActive);
	isKeyActive = isKeyActive || (this->setting.isVKey && global.key.vb.isActive);
	if(isKeyActive){this->status.isActive = true;}

	// トリガー確認
	if(isActiveBefore && !this->status.isActive){this->status.isTrigger = true;}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

