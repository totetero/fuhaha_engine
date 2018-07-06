#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsButtonBoxImplement{
	struct engineLayoutViewPartsButtonBox super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsButtonBoxImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	engineLayoutViewUtilGraphicObjectInit((struct engineLayoutView*)this);
	this->super.super.interact.setting.isTouchable = true;

	// パーツ作成
	this->super.frameNormal = engineLayoutViewPartsFrameCreate();
	this->super.frameSelect = engineLayoutViewPartsFrameCreate();
	this->super.frameActibve = engineLayoutViewPartsFrameCreate();
	this->super.frameInactive = engineLayoutViewPartsFrameCreate();
	this->super.viewInner = engineLayoutViewCreate();
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
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)this->super.frameNormal, 0, 0, 0, 0);
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)this->super.frameSelect, 0, 0, 0, 0);
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)this->super.frameActibve, 0, 0, 0, 0);
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)this->super.frameInactive, 0, 0, 0, 0);
	engineLayoutViewUtilPositionSetLtRtTpBm(this->super.viewInner, 0, 0, 0, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameNormal);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameSelect);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameActibve);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameInactive);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameNormal);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameSelect);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameActibve);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameInactive);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	this->super.viewInner->family.zIndex = 1;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsButtonBoxImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsButtonBoxImplement *this){
	// ボタンの見た目を状態に合わせて変える
	this->super.frameNormal->super.family.isInvisible = true;
	this->super.frameSelect->super.family.isInvisible = true;
	this->super.frameActibve->super.family.isInvisible = true;
	this->super.frameInactive->super.family.isInvisible = true;
	if(this->super.super.interact.status.isHover){
		// 押下状態
		this->super.frameActibve->super.family.isInvisible = false;
		engineLayoutViewUtilPositionSetPaddingTop(this->super.viewInner, 2);
		engineLayoutViewUtilPositionSetPaddingBottom(this->super.viewInner, 0);
	}else if(this->super.isSelect){
		// 選択状態
		this->super.frameSelect->super.family.isInvisible = false;
		engineLayoutViewUtilPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewUtilPositionSetPaddingBottom(this->super.viewInner, 2);
	}else if(this->super.isInactiveDraw){
		// 非活性状態
		this->super.frameInactive->super.family.isInvisible = false;
		engineLayoutViewUtilPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewUtilPositionSetPaddingBottom(this->super.viewInner, 2);
	}else{
		// 通常状態
		this->super.frameNormal->super.family.isInvisible = false;
		engineLayoutViewUtilPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewUtilPositionSetPaddingBottom(this->super.viewInner, 2);
	}

	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsButtonBoxImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsButtonBoxImplement *this){
}

// 描画
static void draw(struct engineLayoutViewPartsButtonBoxImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsButtonBoxImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsButtonBoxImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilGraphicObjectDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsButtonBox", this);
}

// ----------------------------------------------------------------

// 標準ボタン構造体 作成
struct engineLayoutViewPartsButtonBox *engineLayoutViewPartsButtonBoxCreate(){
	struct engineLayoutViewPartsButtonBoxImplement *this = (struct engineLayoutViewPartsButtonBoxImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsButtonBox", 1, sizeof(struct engineLayoutViewPartsButtonBoxImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsButtonBox*)this;
}

// 標準ボタン構造体 作成 テキスト付き
struct engineLayoutViewPartsButtonBox *engineLayoutViewPartsButtonBoxCreateText(char *text){
	struct engineLayoutViewPartsButtonBox *this = engineLayoutViewPartsButtonBoxCreate();
	struct engineLayoutViewPartsFontText *viewText = engineLayoutViewPartsFontTextCreateDefault(text);
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewText, 0, 0, 0, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewInner, (struct engineLayoutView*)viewText);
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

