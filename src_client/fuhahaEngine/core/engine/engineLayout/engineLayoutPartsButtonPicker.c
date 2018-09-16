#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsButtonPickerImplement{
	struct engineLayoutPartsButtonPicker super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsButtonPickerImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.interact.setting.isTouchable = true;

	// パーツ作成
	this->super.rectNormal = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_XYWH);
	this->super.rectSelect = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONPICKERSELECT_XYWH);
	this->super.rectActibve = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_XYWH);
	this->super.rectInactive = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_XYWH);
	this->super.frameNormal = engineLayoutPartsFrameCreate();
	this->super.frameSelect = engineLayoutPartsFrameCreate();
	this->super.frameActibve = engineLayoutPartsFrameCreate();
	this->super.frameInactive = engineLayoutPartsFrameCreate();
	this->super.viewInner = engineLayoutViewCreate();
	this->super.frameNormal->texture.tu = TEXPOS_SYSTEM_BUTTONBASICNORMAL_X + 20;
	this->super.frameNormal->texture.tv = TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y;
	this->super.frameNormal->texture.tw = TEXPOS_SYSTEM_BUTTONBASICNORMAL_W - 20;
	this->super.frameNormal->texture.th = TEXPOS_SYSTEM_BUTTONBASICNORMAL_H;
	this->super.frameSelect->texture.tu = TEXPOS_SYSTEM_BUTTONBASICSELECT_X + 20;
	this->super.frameSelect->texture.tv = TEXPOS_SYSTEM_BUTTONBASICSELECT_Y;
	this->super.frameSelect->texture.tw = TEXPOS_SYSTEM_BUTTONBASICSELECT_W - 20;
	this->super.frameSelect->texture.th = TEXPOS_SYSTEM_BUTTONBASICSELECT_H;
	this->super.frameActibve->texture.tu = TEXPOS_SYSTEM_BUTTONBASICACTIVE_X + 20;
	this->super.frameActibve->texture.tv = TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y;
	this->super.frameActibve->texture.tw = TEXPOS_SYSTEM_BUTTONBASICACTIVE_W - 20;
	this->super.frameActibve->texture.th = TEXPOS_SYSTEM_BUTTONBASICACTIVE_H;
	this->super.frameInactive->texture.tu = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X + 20;
	this->super.frameInactive->texture.tv = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y;
	this->super.frameInactive->texture.tw = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W - 20;
	this->super.frameInactive->texture.th = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H;
	this->super.frameNormal->border.bt = 24;
	this->super.frameNormal->border.bb = 24;
	this->super.frameNormal->border.bl = 0;
	this->super.frameSelect->border.bt = 24;
	this->super.frameSelect->border.bb = 24;
	this->super.frameSelect->border.bl = 0;
	this->super.frameActibve->border.bt = 24;
	this->super.frameActibve->border.bb = 24;
	this->super.frameActibve->border.bl = 0;
	this->super.frameInactive->border.bt = 24;
	this->super.frameInactive->border.bb = 24;
	this->super.frameInactive->border.bl = 0;
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.rectNormal, 0);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.rectSelect, 0);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.rectActibve, 0);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.rectInactive, 0);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->super.rectNormal, 28);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->super.rectSelect, 28);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->super.rectActibve, 28);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->super.rectInactive, 28);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.rectNormal, 30);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.rectSelect, 30);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.rectActibve, 30);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.rectInactive, 30);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.frameNormal, 28);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.frameSelect, 28);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.frameActibve, 28);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.frameInactive, 28);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->super.viewInner, 28);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->super.frameNormal, 0);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->super.frameSelect, 0);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->super.frameActibve, 0);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->super.frameInactive, 0);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)this->super.viewInner, 0);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.frameNormal, 24);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.frameSelect, 24);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.frameActibve, 24);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.frameInactive, 24);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->super.viewInner, 24);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectSelect);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectActibve);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectInactive);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameSelect);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameActibve);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameInactive);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectSelect);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.rectInactive);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameSelect);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.frameInactive);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	this->super.viewInner->family.zIndex = 1;
}

// ----------------------------------------------------------------

// 計算
static void calc(struct engineLayoutPartsButtonPickerImplement *this, bool isCancel){
	// ボタンの見た目を状態に合わせて変える
	this->super.rectNormal->super.family.isInvisible = true;
	this->super.rectSelect->super.family.isInvisible = true;
	this->super.rectActibve->super.family.isInvisible = true;
	this->super.rectInactive->super.family.isInvisible = true;
	this->super.frameNormal->super.family.isInvisible = true;
	this->super.frameSelect->super.family.isInvisible = true;
	this->super.frameActibve->super.family.isInvisible = true;
	this->super.frameInactive->super.family.isInvisible = true;
	if(this->super.super.interact.status.isHover){
		// 押下状態
		this->super.rectActibve->super.family.isInvisible = false;
		this->super.frameActibve->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 2);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 0);
	}else if(this->super.isSelect){
		// 選択状態
		this->super.rectSelect->super.family.isInvisible = false;
		this->super.frameSelect->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 2);
	}else if(this->super.isInactiveDraw){
		// 非活性状態
		this->super.rectInactive->super.family.isInvisible = false;
		this->super.frameInactive->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 2);
	}else{
		// 通常状態
		this->super.rectNormal->super.family.isInvisible = false;
		this->super.frameNormal->super.family.isInvisible = false;
		engineLayoutViewGearPositionSetPaddingTop(this->super.viewInner, 0);
		engineLayoutViewGearPositionSetPaddingBottom(this->super.viewInner, 2);
	}

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// ピッカーボタン構造体 作成
struct engineLayoutPartsButtonPicker *engineLayoutPartsButtonPickerCreate(void){
	struct engineLayoutPartsButtonPickerImplement *this = (struct engineLayoutPartsButtonPickerImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineLayoutPartsButtonPickerImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutPartsButtonPicker*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

