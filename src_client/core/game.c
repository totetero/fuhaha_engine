#include "library.h"
#include "platform.h"
#include "plugin/pluginData.h"
#include "engine/engineMath/engineMath.h"
#include "engine/engineGraphic/engineGraphic.h"
#include "game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// グローバル変数作成
struct global global = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameSurfaceCreated(void){
	engineGraphicGameInit();

	global.screen.w = 320;
	global.screen.h = 480;
	global.acc.x = 0.0;
	global.acc.y = 1.0;
	global.acc.z = 0.0;
}

// opengl画面サイズ変更
void gameSurfaceChanged(int width, int height, double pixelRatio){
	global.window.w = width;
	global.window.h = height;
	glViewport(0, 0, width * pixelRatio, height * pixelRatio);
	if(false){
		// スクリーンサイズ固定 
		global.screen.w = 320;
		global.screen.h = 480;
	}else if(true){
		// スクリーン横幅固定
		global.screen.w = 320;
		global.screen.h = global.screen.w * global.window.h / global.window.w;
		if(global.screen.h < global.screen.w){global.screen.h = global.screen.w;}
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

// opengl描画
void gameSurfaceDrawFrame(void){
	engineGraphicSampleCode();
}

// opengl中断
void gameSurfacePause(void){
}

// opengl終了
void gameSurfaceDestroy(void){
	engineGraphicGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 押下状態更新
// データ使用までの間に1往復以上の更新がかかると取り逃がしが発生する
// データ使用時に非活性トリガーよりも活性トリガーを先に確認してトリガーを下げるようにすれば
// 活性なのに非活性と判断されることはあっても逆はない
void coreEventActivatableUpdate(struct activatable *this, bool isActive){
	if(this->isActive != isActive){
		if(isActive){
			this->triggerActive = true;
		}else{
			this->triggerInactive = true;
		}
		this->isActive = isActive;
	}
}

// タッチの状態を報告する
void gameEvenTouch(int index, int tx, int ty, bool tdn){
	if(index >= (int)(sizeof(global.touch) / sizeof(*global.touch))){return;}
	struct touch *touch = &global.touch[index];
	touch->window.x = tx;
	touch->window.y = ty;
	touch->screen.x = (int)(global.screen.aspect.x * global.screen.w * tx / global.window.w - global.screen.offset.x);
	touch->screen.y = (int)(global.screen.aspect.y * global.screen.h * ty / global.window.h - global.screen.offset.y);
	coreEventActivatableUpdate(&touch->dn, tdn);
}

// キーの押下状態変化を報告する
void gameEventKeyBack(bool kbk){
	coreEventActivatableUpdate(&global.key.bk, kbk);
}
void gameEventKeyArrow(bool kup, bool kdn, bool krt, bool klt){
	coreEventActivatableUpdate(&global.key.up, kup);
	coreEventActivatableUpdate(&global.key.dn, kdn);
	coreEventActivatableUpdate(&global.key.rt, krt);
	coreEventActivatableUpdate(&global.key.lt, klt);
}
void gameEventKeyZxcv(bool kzb, bool kxb, bool kcb, bool kvb){
	coreEventActivatableUpdate(&global.key.zb, kzb);
	coreEventActivatableUpdate(&global.key.xb, kxb);
	coreEventActivatableUpdate(&global.key.cb, kcb);
	coreEventActivatableUpdate(&global.key.vb, kvb);
}

// 傾きの状態を報告する
void gameEventAcceleration(double accx, double accy, double accz){
	global.acc.x = accx;
	global.acc.y = accy;
	global.acc.z = accz;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

