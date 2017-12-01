#include "../../library.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "../engineCtrl/engineCtrl.h"
#include "./engineLayout02.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02ViewPartsButtonBox *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);

	this->frameNormal = engineLayout02ViewPartsFrameCreate();
	this->frameSelect = engineLayout02ViewPartsFrameCreate();
	this->frameActibve = engineLayout02ViewPartsFrameCreate();
	this->frameInactive = engineLayout02ViewPartsFrameCreate();
	this->viewInner = engineLayout02ViewCreate();
	this->frameNormal->texture.tu = TEXPOS_SYSTEM_BUTTONBASICNORMAL_X;
	this->frameNormal->texture.tv = TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y;
	this->frameNormal->texture.tw = TEXPOS_SYSTEM_BUTTONBASICNORMAL_W;
	this->frameNormal->texture.th = TEXPOS_SYSTEM_BUTTONBASICNORMAL_H;
	this->frameSelect->texture.tu = TEXPOS_SYSTEM_BUTTONBASICSELECT_X;
	this->frameSelect->texture.tv = TEXPOS_SYSTEM_BUTTONBASICSELECT_Y;
	this->frameSelect->texture.tw = TEXPOS_SYSTEM_BUTTONBASICSELECT_W;
	this->frameSelect->texture.th = TEXPOS_SYSTEM_BUTTONBASICSELECT_H;
	this->frameActibve->texture.tu = TEXPOS_SYSTEM_BUTTONBASICACTIVE_X;
	this->frameActibve->texture.tv = TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y;
	this->frameActibve->texture.tw = TEXPOS_SYSTEM_BUTTONBASICACTIVE_W;
	this->frameActibve->texture.th = TEXPOS_SYSTEM_BUTTONBASICACTIVE_H;
	this->frameInactive->texture.tu = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X;
	this->frameInactive->texture.tv = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y;
	this->frameInactive->texture.tw = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W;
	this->frameInactive->texture.th = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H;
	this->frameNormal->border.bt = 24;
	this->frameNormal->border.bb = 24;
	this->frameSelect->border.bt = 24;
	this->frameSelect->border.bb = 24;
	this->frameActibve->border.bt = 24;
	this->frameActibve->border.bb = 24;
	this->frameInactive->border.bt = 24;
	this->frameInactive->border.bb = 24;
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->frameNormal, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->frameSelect, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->frameActibve, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->frameInactive, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm(this->viewInner, 0, 0, 0, 0);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->frameNormal);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->frameSelect);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->frameActibve);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->frameInactive);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->viewInner);
	this->viewInner->children.zIndex = 1;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsButtonBox *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	isCancel = (isCancel || ((struct engineLayout02View*)this)->interact.setting.isDisable);

	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsButtonBox *this){
	// ボタンの見た目を状態に合わせて変える
	this->frameNormal->super.children.isInvisible = true;
	this->frameSelect->super.children.isInvisible = true;
	this->frameActibve->super.children.isInvisible = true;
	this->frameInactive->super.children.isInvisible = true;
	if(this->super.interact.status.isHover){
		// 押下状態
		this->frameActibve->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->viewInner, 2);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->viewInner, 0);
	}else if(this->isSelect){
		// 選択状態
		this->frameSelect->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->viewInner, 0);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->viewInner, 2);
	}else if(this->isInactiveDraw){
		// 非活性状態
		this->frameInactive->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->viewInner, 0);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->viewInner, 2);
	}else{
		// 通常状態
		this->frameNormal->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->viewInner, 0);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->viewInner, 2);
	}

	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayout02ViewPartsButtonBox *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsButtonBox *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsButtonBox *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewDetouch((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsButtonBox", this);
}

// ----------------------------------------------------------------

// 標準ボタン構造体 作成
struct engineLayout02ViewPartsButtonBox *engineLayout02ViewPartsButtonBoxCreate(){
	struct engineLayout02ViewPartsButtonBox *this = (struct engineLayout02ViewPartsButtonBox*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsButtonBox", 1, sizeof(struct engineLayout02ViewPartsButtonBox));
	init(this);

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

