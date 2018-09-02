#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsButtonBoxImplement{
	struct engineLayoutPartsButtonBox super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsButtonBoxImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.interact.setting.isTouchable = true;

	// パーツ作成
	this->super.frameNormal = engineLayoutPartsFrameCreate();
	this->super.frameSelect = engineLayoutPartsFrameCreate();
	this->super.frameActibve = engineLayoutPartsFrameCreate();
	this->super.frameInactive = engineLayoutPartsFrameCreate();
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
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameSelect);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameActibve);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameInactive);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameSelect);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameInactive);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	this->super.viewInner->family.zIndex = 1;
}

// ----------------------------------------------------------------

// 計算
static void calc(struct engineLayoutPartsButtonBoxImplement *this, bool isCancel){
	// ボタンの見た目を状態に合わせて変える
	this->super.frameNormal->super.family.isInvisible = true;
	this->super.frameSelect->super.family.isInvisible = true;
	this->super.frameActibve->super.family.isInvisible = true;
	this->super.frameInactive->super.family.isInvisible = true;
	if(this->super.super.interact.status.isHover){
		// 押下状態
		this->super.frameActibve->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 2);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 0);
	}else if(this->super.isSelect){
		// 選択状態
		this->super.frameSelect->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 2);
	}else if(this->super.isInactiveDraw){
		// 非活性状態
		this->super.frameInactive->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 2);
	}else{
		// 通常状態
		this->super.frameNormal->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 2);
	}

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// 標準ボタン構造体 作成
struct engineLayoutPartsButtonBox *engineLayoutPartsButtonBoxCreate(){
	struct engineLayoutPartsButtonBoxImplement *this = (struct engineLayoutPartsButtonBoxImplement*)engineUtilMemoryInfoCalloc("engineLayoutPartsButtonBox", 1, sizeof(struct engineLayoutPartsButtonBoxImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutPartsButtonBox*)this;
}

// 標準ボタン構造体 作成 テキスト付き
struct engineLayoutPartsButtonBox *engineLayoutPartsButtonBoxCreateText(char *text){
	struct engineLayoutPartsButtonBox *this = engineLayoutPartsButtonBoxCreate();
	struct engineLayoutPartsFontText *viewText = engineLayoutPartsFontTextCreateDefault(text);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewInner, (struct engineLayoutView*)viewText);
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

