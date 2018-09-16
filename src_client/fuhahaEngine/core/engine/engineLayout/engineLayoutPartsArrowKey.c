#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsArrowKeyImplement{
	struct engineLayoutPartsArrowKey super;
};

// ----------------------------------------------------------------

// タッチ判定
static bool isInner(struct engineLayoutPartsArrowKeyImplement *this, double x, double y){
	// ローカル座標変換
	struct engineMathVector3 tempVec1;
	engineMathVec3Set(&tempVec1, x, y, 0);
	engineLayoutViewGearPositionTransformCalcInvert((struct engineLayoutView*)this, &tempVec1);
	// 楕円内部判定
	double rx = this->super.super.position.layout.w * 0.5;
	double ry = this->super.super.position.layout.h * 0.5;
	double x0 = (tempVec1.x - rx) / rx;
	double y0 = (tempVec1.y - ry) / ry;
	return (x0 * x0 + y0 * y0 < 1);
}

// 初期化
static void init(struct engineLayoutPartsArrowKeyImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.position.isInner = (bool(*)(struct engineLayoutView*, double, double))isInner;
	this->super.super.interact.setting.isTouchable = true;

	// サイズ設定
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this, 144);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this, 144);

	// パーツ作成
	struct engineLayoutView *viewUp = engineLayoutViewCreate();
	struct engineLayoutView *viewDn = engineLayoutViewCreate();
	struct engineLayoutView *viewRt = engineLayoutViewCreate();
	struct engineLayoutView *viewLt = engineLayoutViewCreate();
	engineLayoutViewGearPositionSetTop((struct engineLayoutView*)viewUp, 15);
	engineLayoutViewGearPositionSetBottom((struct engineLayoutView*)viewDn, 15);
	engineLayoutViewGearPositionSetRight((struct engineLayoutView*)viewRt, 15);
	engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)viewLt, 15);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)viewUp, 34 );
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)viewDn, 34);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)viewRt, 46);
	engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)viewLt, 46);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)viewUp, 46 + 4);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)viewDn, 46 + 4);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)viewRt, 34 + 4);
	engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)viewLt, 34 + 4);
	engineLayoutViewGearPositionSetPaddingBottom((struct engineLayoutView*)viewUp, 2);
	engineLayoutViewGearPositionSetPaddingBottom((struct engineLayoutView*)viewDn, 2);
	engineLayoutViewGearPositionSetPaddingBottom((struct engineLayoutView*)viewRt, 2);
	engineLayoutViewGearPositionSetPaddingBottom((struct engineLayoutView*)viewLt, 2);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewUp);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewDn);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewRt);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewLt);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewUp);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewDn);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewRt);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewLt);
	this->super.buttonUpNormal = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_XYWH);
	this->super.buttonDnNormal = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_XYWH);
	this->super.buttonRtNormal = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_XYWH);
	this->super.buttonLtNormal = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_XYWH);
	this->super.buttonUpActibve = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLUPACTIVE_XYWH);
	this->super.buttonDnActibve = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLDNACTIVE_XYWH);
	this->super.buttonRtActibve = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLRTACTIVE_XYWH);
	this->super.buttonLtActibve = engineLayoutPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLLTACTIVE_XYWH);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtNormal);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpActibve);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnActibve);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtActibve);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtNormal);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtActibve);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtActibve);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct engineLayoutPartsArrowKeyImplement *this, bool isCancel){
	bool isTouchUp = false;
	bool isTouchDn = false;
	bool isTouchRt = false;
	bool isTouchLt = false;
	if(this->super.super.interact.status.isActive){
		// ローカル座標変換
		struct engineMathVector3 tempVec1;
		engineMathVec3Set(&tempVec1, this->super.super.interact.status.currX, this->super.super.interact.status.currY, 0);
		engineLayoutViewGearPositionTransformCalcInvert((struct engineLayoutView*)this, &tempVec1);
		// 十字キーのタッチ確認
		double x1 = tempVec1.x - this->super.super.position.layout.w * 0.5;
		double y1 = tempVec1.y - this->super.super.position.layout.h * 0.5;
		isTouchUp = this->super.super.interact.status.isHover && (y1 < 0 && x1 < y1 * y1 * 0.18 && x1 > y1 * y1 * -0.18);
		isTouchDn = this->super.super.interact.status.isHover && (y1 > 0 && x1 < y1 * y1 * 0.18 && x1 > y1 * y1 * -0.18);
		isTouchRt = this->super.super.interact.status.isHover && (x1 > 0 && y1 < x1 * x1 * 0.18 && y1 > x1 * x1 * -0.18);
		isTouchLt = this->super.super.interact.status.isHover && (x1 < 0 && y1 < x1 * x1 * 0.18 && y1 > x1 * x1 * -0.18);
	}

	// 十字キーの押下確認
	this->super.isUp = (global.key.up.isActive || isTouchUp);
	this->super.isDn = (global.key.dn.isActive || isTouchDn);
	this->super.isRt = (global.key.rt.isActive || isTouchRt);
	this->super.isLt = (global.key.lt.isActive || isTouchLt);

	// ボタンの見た目を状態に合わせて変える
	this->super.buttonUpNormal->super.family.isInvisible = this->super.isUp;
	this->super.buttonDnNormal->super.family.isInvisible = this->super.isDn;
	this->super.buttonRtNormal->super.family.isInvisible = this->super.isRt;
	this->super.buttonLtNormal->super.family.isInvisible = this->super.isLt;
	this->super.buttonUpActibve->super.family.isInvisible = !this->super.isUp;
	this->super.buttonDnActibve->super.family.isInvisible = !this->super.isDn;
	this->super.buttonRtActibve->super.family.isInvisible = !this->super.isRt;
	this->super.buttonLtActibve->super.family.isInvisible = !this->super.isLt;

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// 十字キー構造体 作成
struct engineLayoutPartsArrowKey *engineLayoutPartsArrowKeyCreate(void){
	struct engineLayoutPartsArrowKeyImplement *this = (struct engineLayoutPartsArrowKeyImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineLayoutPartsArrowKeyImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutPartsArrowKey*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

