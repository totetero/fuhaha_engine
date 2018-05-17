#include "./library.h"
#include "./engine/engineMath/engineMath.h"
#include "./game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// グローバル変数作成
struct globalVariable global = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 活性状態更新
// データ使用までの間に1往復以上の更新がかかると取り逃がしが発生する
// データ使用時に非活性トリガーよりも活性トリガーを先に確認してトリガーを下げるようにすれば
// 活性なのに非活性と判断されることはあっても逆はない
static void activatableUpdate(struct globalActivatable *this, bool isActive){
	if(this->isActive != isActive){
		if(isActive){
			this->triggerActive = true;
		}else{
			this->triggerInactive = true;
		}
		this->isActive = isActive;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ユーザーインターフェイスイベント初期化
void gameMainEventInit(void){
	global.screen.w = 320;
	global.screen.h = 480;
	global.acc.x = 0.0;
	global.acc.y = 1.0;
	global.acc.z = 0.0;
}

// ----------------------------------------------------------------

// 画面サイズの状態を報告する
void gameMainEventScreen(int width, int height, double pixelRatio){
	global.window.w = width;
	global.window.h = height;
	glViewport(0, 0, width * pixelRatio, height * pixelRatio);
	if(DEFINESETTING_SCREEN_W > 0 && DEFINESETTING_SCREEN_H > 0){
		// スクリーンサイズ固定 
		global.screen.w = DEFINESETTING_SCREEN_W;
		global.screen.h = DEFINESETTING_SCREEN_H;
	}else if(DEFINESETTING_SCREEN_W > 0){
		// スクリーン横幅固定
		global.screen.w = DEFINESETTING_SCREEN_W;
		global.screen.h = DEFINESETTING_SCREEN_W * global.window.h / global.window.w;
		int minh = engineMathFloor(DEFINESETTING_SCREEN_W * DEFINESETTING_SCREEN_ASPECTMIN);
		int maxh = engineMathFloor(DEFINESETTING_SCREEN_W * DEFINESETTING_SCREEN_ASPECTMAX);
		if(global.screen.h < minh){global.screen.h = minh;}
		if(global.screen.h > maxh){global.screen.h = maxh;}
	}else if(DEFINESETTING_SCREEN_H > 0){
		// スクリーン縦幅固定
		global.screen.h = DEFINESETTING_SCREEN_H;
		global.screen.w = DEFINESETTING_SCREEN_H * global.window.w / global.window.h;
		int minw = engineMathFloor(DEFINESETTING_SCREEN_H * DEFINESETTING_SCREEN_ASPECTMIN);
		int maxw = engineMathFloor(DEFINESETTING_SCREEN_H * DEFINESETTING_SCREEN_ASPECTMAX);
		if(global.screen.w < minw){global.screen.w = minw;}
		if(global.screen.w > maxw){global.screen.w = maxw;}
	}else{
		// スクリーンサイズをウインドウサイズに合わせる
		global.screen.w = width;
		global.screen.h = height;
	}
	// スクリーン座標系でのタッチ座標
	global.screen.aspect.x = (double)(global.window.w * global.screen.h) / (global.window.h * global.screen.w);
	global.screen.aspect.y = (double)(global.window.h * global.screen.w) / (global.window.w * global.screen.h);
	if(global.screen.aspect.x < 1){global.screen.aspect.x = 1;}
	if(global.screen.aspect.y < 1){global.screen.aspect.y = 1;}
	global.screen.offset.x = global.screen.w * (global.screen.aspect.x - 1) * 0.5;
	global.screen.offset.y = global.screen.h * (global.screen.aspect.y - 1) * 0.5;
}

// ----------------------------------------------------------------

// タッチの状態を報告する
void gameMainEventTouch(int index, int tx, int ty, bool tdn){
	if(index >= (int)(sizeof(global.touch) / sizeof(*global.touch))){return;}
	struct globalTouch *touch = &global.touch[index];
	touch->window.x = tx;
	touch->window.y = ty;
	touch->screen.x = (int)(global.screen.aspect.x * global.screen.w * tx / global.window.w - global.screen.offset.x);
	touch->screen.y = (int)(global.screen.aspect.y * global.screen.h * ty / global.window.h - global.screen.offset.y);
	activatableUpdate(&touch->dn, tdn);
}

// ----------------------------------------------------------------

// キーの押下状態変化を報告する
void gameMainEventKeyBack(bool kbk){
	activatableUpdate(&global.key.bk, kbk);
}
void gameMainEventKeyArrow(bool kup, bool kdn, bool krt, bool klt){
	activatableUpdate(&global.key.up, kup);
	activatableUpdate(&global.key.dn, kdn);
	activatableUpdate(&global.key.rt, krt);
	activatableUpdate(&global.key.lt, klt);
}
void gameMainEventKeyZxcv(bool kzb, bool kxb, bool kcb, bool kvb){
	activatableUpdate(&global.key.zb, kzb);
	activatableUpdate(&global.key.xb, kxb);
	activatableUpdate(&global.key.cb, kcb);
	activatableUpdate(&global.key.vb, kvb);
}

// ----------------------------------------------------------------

// 傾きの状態を報告する
void gameMainEventAcceleration(double accx, double accy, double accz){
	global.acc.x = accx;
	global.acc.y = accy;
	global.acc.z = accz;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

