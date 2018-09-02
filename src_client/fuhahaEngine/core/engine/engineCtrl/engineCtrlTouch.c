#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct engineCtrlTouchLocal{
		struct engineCtrlTouch touch;
		struct{
			int tempx;
			int tempy;
		} screen;
		bool trigger;
		bool free;
	} list[sizeof(global.touch) / sizeof(*global.touch)];
	struct engineCtrlTouchLocal *tempTouch;
	enum engineCtrlTouchId tempTouchId;
} localGlobal = {0};

// ----------------------------------------------------------------

// コントローラ状態メインループ計算
void engineCtrlTouchCalc(void){
	for(int i = 0; i < (int)(sizeof(localGlobal.list) / sizeof(*localGlobal.list)); i++){
		struct engineCtrlTouchLocal *localTouch = &localGlobal.list[i];
		struct globalTouch *globalTouch = &global.touch[i];
		if(globalTouch->dn.triggerActive){
			// 押した直後
			globalTouch->dn.triggerActive = false;
			localTouch->touch.dn = true;
			localTouch->touch.mv = false;
			localTouch->touch.window.x = globalTouch->window.x;
			localTouch->touch.window.y = globalTouch->window.y;
			localTouch->touch.screen.x = localTouch->screen.tempx = globalTouch->screen.x;
			localTouch->touch.screen.y = localTouch->screen.tempy = globalTouch->screen.y;
			localTouch->trigger = true;
		}else if(globalTouch->dn.isActive){
			// 押下中
			localTouch->touch.window.x = globalTouch->window.x;
			localTouch->touch.window.y = globalTouch->window.y;
			localTouch->touch.screen.x = globalTouch->screen.x;
			localTouch->touch.screen.y = globalTouch->screen.y;
			if(!localTouch->touch.mv){
				// タッチ移動の確認
				int x = localTouch->touch.screen.x - localTouch->screen.tempx;
				int y = localTouch->touch.screen.y - localTouch->screen.tempy;
				localTouch->touch.mv = (x * x + y * y > 5 * 5);
			}
		}else if(globalTouch->dn.triggerInactive){
			// 離した直後
			globalTouch->dn.triggerInactive = false;
			localTouch->touch.dn = false;
			localTouch->trigger = false;
		}

		if(localTouch->free){
			// タッチ解放実行
			localTouch->touch.touchId = ENGINECTRLTOUCHID_NONE;
			localTouch->free = false;
		}
	}
}

// タッチ情報取得
struct engineCtrlTouch *engineCtrlTouchGet(enum engineCtrlTouchId touchId){
	localGlobal.tempTouchId = touchId;
	// 自分で占有済みのタッチ情報を探す
	for(int i = 0; i < (int)(sizeof(localGlobal.list) / sizeof(*localGlobal.list)); i++){
		localGlobal.tempTouch = &localGlobal.list[i];
		if(!localGlobal.tempTouch->trigger && localGlobal.tempTouch->touch.touchId == touchId){
			localGlobal.tempTouch->touch.active = true;
			return &localGlobal.tempTouch->touch;
		}
	}
	// 占有されていないタッチ情報を探す
	for(int i = 0; i < (int)(sizeof(localGlobal.list) / sizeof(*localGlobal.list)); i++){
		localGlobal.tempTouch = &localGlobal.list[i];
		if(localGlobal.tempTouch->trigger){
			localGlobal.tempTouch->touch.active = false;
			return &localGlobal.tempTouch->touch;
		}
	}
	// タッチ情報が見つからなかった
	localGlobal.tempTouch = NULL;
	return NULL;
}

// 取得したタッチ情報占有
void engineCtrlTouchOwn(void){
	if(localGlobal.tempTouch != NULL){
		localGlobal.tempTouch->trigger = false;
		localGlobal.tempTouch->touch.touchId = localGlobal.tempTouchId;
	}
}

// 取得して占有したタッチ情報の解放
void engineCtrlTouchFree(void){
	if(localGlobal.tempTouch != NULL){
		localGlobal.tempTouch->free = true;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

