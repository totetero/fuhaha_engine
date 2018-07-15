#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsArrowKeyImplement{
	struct engineLayoutViewPartsArrowKey super;
	bool isTouchUp;
	bool isTouchDn;
	bool isTouchRt;
	bool isTouchLt;
};

// ----------------------------------------------------------------

static bool isInner(struct engineLayoutViewPartsArrowKeyImplement *this, double x, double y){
	// ローカル座標変換
	struct engineMathVector3 tempVec1;
	engineMathVec3Set(&tempVec1, x, y, 0);
	engineLayoutViewUtilPositionTransformCalcInvert((struct engineLayoutView*)this, &tempVec1);
	// 楕円内部判定
	double rx = this->super.super.position.layout.w * 0.5;
	double ry = this->super.super.position.layout.h * 0.5;
	double x0 = (tempVec1.x - rx) / rx;
	double y0 = (tempVec1.y - ry) / ry;
	return (x0 * x0 + y0 * y0 < 1);
}

// 初期化
static void init(struct engineLayoutViewPartsArrowKeyImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	this->super.super.position.isInner = (bool(*)(struct engineLayoutView*, double, double))isInner;
	this->super.super.interact.setting.isTouchable = true;

	// サイズ設定
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)this, 144);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)this, 144);

	// パーツ作成
	struct engineLayoutView *viewUp = engineLayoutViewCreate();
	struct engineLayoutView *viewDn = engineLayoutViewCreate();
	struct engineLayoutView *viewRt = engineLayoutViewCreate();
	struct engineLayoutView *viewLt = engineLayoutViewCreate();
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)viewUp, 15);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)viewDn, 15);
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)viewRt, 15);
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)viewLt, 15);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewUp, 34 );
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewDn, 34);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewRt, 46);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewLt, 46);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewUp, 46 + 4);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewDn, 46 + 4);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewRt, 34 + 4);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewLt, 34 + 4);
	engineLayoutViewUtilPositionSetPaddingBottom((struct engineLayoutView*)viewUp, 2);
	engineLayoutViewUtilPositionSetPaddingBottom((struct engineLayoutView*)viewDn, 2);
	engineLayoutViewUtilPositionSetPaddingBottom((struct engineLayoutView*)viewRt, 2);
	engineLayoutViewUtilPositionSetPaddingBottom((struct engineLayoutView*)viewLt, 2);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewUp);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewDn);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewRt);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewLt);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewUp);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewDn);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewRt);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewLt);
	this->super.buttonUpNormal = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_XYWH);
	this->super.buttonDnNormal = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_XYWH);
	this->super.buttonRtNormal = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_XYWH);
	this->super.buttonLtNormal = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_XYWH);
	this->super.buttonUpActibve = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLUPACTIVE_XYWH);
	this->super.buttonDnActibve = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLDNACTIVE_XYWH);
	this->super.buttonRtActibve = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLRTACTIVE_XYWH);
	this->super.buttonLtActibve = engineLayoutViewPartsRectCreate("img/system.png", TEXSIZ_SYSTEM_WH, TEXPOS_SYSTEM_BUTTONCTRLLTACTIVE_XYWH);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpNormal);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnNormal);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtNormal);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtNormal);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpActibve);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnActibve);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtActibve);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtActibve);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpNormal);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnNormal);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtNormal);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtNormal);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewUp, (struct engineLayoutView*)this->super.buttonUpActibve);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewDn, (struct engineLayoutView*)this->super.buttonDnActibve);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewRt, (struct engineLayoutView*)this->super.buttonRtActibve);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewLt, (struct engineLayoutView*)this->super.buttonLtActibve);

	// パラメータ初期化
	this->super.buttonUpNormal->super.family.isInvisible = true;
	this->super.buttonDnNormal->super.family.isInvisible = true;
	this->super.buttonRtNormal->super.family.isInvisible = true;
	this->super.buttonLtNormal->super.family.isInvisible = true;
	this->super.buttonUpActibve->super.family.isInvisible = false;
	this->super.buttonDnActibve->super.family.isInvisible = false;
	this->super.buttonRtActibve->super.family.isInvisible = false;
	this->super.buttonLtActibve->super.family.isInvisible = false;
}

// ----------------------------------------------------------------


// タッチ処理
static bool touch(struct engineLayoutViewPartsArrowKeyImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;

	if(isActiveLocal){
		// ローカル座標変換
		struct engineMathVector3 tempVec1;
		engineMathVec3Set(&tempVec1, x, y, 0);
		engineLayoutViewUtilPositionTransformCalcInvert((struct engineLayoutView*)this, &tempVec1);
		// 十字キーのタッチ確認
		double x1 = tempVec1.x - this->super.super.position.layout.w * 0.5;
		double y1 = tempVec1.y - this->super.super.position.layout.h * 0.5;
		this->isTouchUp = this->super.super.interact.status.isHover && (y1 < 0 && x1 < y1 * y1 * 0.18 && x1 > y1 * y1 * -0.18);
		this->isTouchDn = this->super.super.interact.status.isHover && (y1 > 0 && x1 < y1 * y1 * 0.18 && x1 > y1 * y1 * -0.18);
		this->isTouchRt = this->super.super.interact.status.isHover && (x1 > 0 && y1 < x1 * x1 * 0.18 && y1 > x1 * x1 * -0.18);
		this->isTouchLt = this->super.super.interact.status.isHover && (x1 < 0 && y1 < x1 * x1 * 0.18 && y1 > x1 * x1 * -0.18);
	}else if(this->super.super.interact.status.touchIndex == touchIndex){
		// タッチしていない時
		this->isTouchUp = false;
		this->isTouchDn = false;
		this->isTouchRt = false;
		this->isTouchLt = false;
	}

	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsArrowKeyImplement *this){
	// 十字キーの押下確認
	this->super.isUp = (global.key.up.isActive || this->isTouchUp);
	this->super.isDn = (global.key.dn.isActive || this->isTouchDn);
	this->super.isRt = (global.key.rt.isActive || this->isTouchRt);
	this->super.isLt = (global.key.lt.isActive || this->isTouchLt);

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
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsArrowKeyImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsArrowKeyImplement *this){
}

// 描画
static void draw(struct engineLayoutViewPartsArrowKeyImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsArrowKeyImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsArrowKeyImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsArrowKey", this);
}

// ----------------------------------------------------------------

// 画像描画構造体 作成
struct engineLayoutViewPartsArrowKey *engineLayoutViewPartsArrowKeyCreate(){
	struct engineLayoutViewPartsArrowKeyImplement *this = (struct engineLayoutViewPartsArrowKeyImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsArrowKey", 1, sizeof(struct engineLayoutViewPartsArrowKeyImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsArrowKey*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

