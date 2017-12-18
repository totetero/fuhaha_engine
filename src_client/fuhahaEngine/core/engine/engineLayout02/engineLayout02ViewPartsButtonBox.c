#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineLayout02/engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayout02ViewPartsButtonBoxImplement{
	struct engineLayout02ViewPartsButtonBox super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02ViewPartsButtonBoxImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);
	this->super.super.interact.setting.isTouchable = true;

	this->super.frameNormal = engineLayout02ViewPartsFrameCreate();
	this->super.frameSelect = engineLayout02ViewPartsFrameCreate();
	this->super.frameActibve = engineLayout02ViewPartsFrameCreate();
	this->super.frameInactive = engineLayout02ViewPartsFrameCreate();
	this->super.viewInner = engineLayout02ViewCreate();
	this->super.frameNormal->texture.tu = TEXPOS_SYSTEM_BUTTONBASICNORMAL_X;
	this->super.frameNormal->texture.tv = TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y;
	this->super.frameNormal->texture.tw = TEXPOS_SYSTEM_BUTTONBASICNORMAL_W;
	this->super.frameNormal->texture.th = TEXPOS_SYSTEM_BUTTONBASICNORMAL_H;
	this->super.frameSelect->texture.tu = TEXPOS_SYSTEM_BUTTONBASICSELECT_X;
	this->super.frameSelect->texture.tv = TEXPOS_SYSTEM_BUTTONBASICSELECT_Y;
	this->super.frameSelect->texture.tw = TEXPOS_SYSTEM_BUTTONBASICSELECT_W;
	this->super.frameSelect->texture.th = TEXPOS_SYSTEM_BUTTONBASICSELECT_H;
	this->super.frameActibve->texture.tu = TEXPOS_SYSTEM_BUTTONBASICACTIVE_X;
	this->super.frameActibve->texture.tv = TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y;
	this->super.frameActibve->texture.tw = TEXPOS_SYSTEM_BUTTONBASICACTIVE_W;
	this->super.frameActibve->texture.th = TEXPOS_SYSTEM_BUTTONBASICACTIVE_H;
	this->super.frameInactive->texture.tu = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X;
	this->super.frameInactive->texture.tv = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y;
	this->super.frameInactive->texture.tw = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W;
	this->super.frameInactive->texture.th = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H;
	this->super.frameNormal->border.bt = 24;
	this->super.frameNormal->border.bb = 24;
	this->super.frameSelect->border.bt = 24;
	this->super.frameSelect->border.bb = 24;
	this->super.frameActibve->border.bt = 24;
	this->super.frameActibve->border.bb = 24;
	this->super.frameInactive->border.bt = 24;
	this->super.frameInactive->border.bb = 24;
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->super.frameNormal, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->super.frameSelect, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->super.frameActibve, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)this->super.frameInactive, 0, 0, 0, 0);
	engineLayout02ViewUtilPositionSetLtRtTpBm(this->super.viewInner, 0, 0, 0, 0);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.frameNormal);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.frameSelect);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.frameActibve);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.frameInactive);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.viewInner);
	this->super.viewInner->children.zIndex = 1;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsButtonBoxImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsButtonBoxImplement *this){
	// ボタンの見た目を状態に合わせて変える
	this->super.frameNormal->super.children.isInvisible = true;
	this->super.frameSelect->super.children.isInvisible = true;
	this->super.frameActibve->super.children.isInvisible = true;
	this->super.frameInactive->super.children.isInvisible = true;
	if(this->super.super.interact.status.isHover){
		// 押下状態
		this->super.frameActibve->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->super.viewInner, 2);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->super.viewInner, 0);
	}else if(this->super.isSelect){
		// 選択状態
		this->super.frameSelect->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->super.viewInner, 2);
	}else if(this->super.isInactiveDraw){
		// 非活性状態
		this->super.frameInactive->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->super.viewInner, 2);
	}else{
		// 通常状態
		this->super.frameNormal->super.children.isInvisible = false;
		engineLayout02ViewUtilPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayout02ViewUtilPositionSetPaddingBottom(this->super.viewInner, 2);
	}

	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayout02ViewPartsButtonBoxImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsButtonBoxImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsButtonBoxImplement *this){
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
	struct engineLayout02ViewPartsButtonBoxImplement *this = (struct engineLayout02ViewPartsButtonBoxImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsButtonBox", 1, sizeof(struct engineLayout02ViewPartsButtonBoxImplement));
	init(this);

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsButtonBox*)this;
}

// 標準ボタン構造体 作成 テキスト付き
struct engineLayout02ViewPartsButtonBox *engineLayout02ViewPartsButtonBoxCreateText(char *text){
	struct engineLayout02ViewPartsButtonBox *this = engineLayout02ViewPartsButtonBoxCreate();
	struct engineLayout02ViewPartsFontText *viewText = engineLayout02ViewPartsFontTextCreateDefault(text);
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)viewText, 0, 0, 0, 0);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this->viewInner, (struct engineLayout02View*)viewText);
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

