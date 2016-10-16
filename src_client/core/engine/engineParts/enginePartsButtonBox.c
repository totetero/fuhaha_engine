#include "../../library.h"
#include "../../texpos.h"
#include "../engineMath/engineMath.h"
#include "../engineCtrl/engineCtrl.h"
#include "../engineGraphic/engineGraphic.h"
#include "engineParts.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineGraphicTrans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct enginePartsButtonBox *this = (struct enginePartsButtonBox*)((char*)that - offsetof(struct enginePartsButtonBox, trans));
	enginePartsButtonBoxDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineGraphicTrans *that){
	struct enginePartsButtonBox *this = (struct enginePartsButtonBox*)((char*)that - offsetof(struct enginePartsButtonBox, trans));
	enginePartsButtonBoxDispose(this);
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 初期化
void enginePartsButtonBoxInit(struct enginePartsButtonBox *this){
	enginePartsButtonInit(&this->super);
	engineGraphicTransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	engineGraphicImageFrameInit(&this->imgNormal);
	engineGraphicImageFrameInit(&this->imgSelect);
	engineGraphicImageFrameInit(&this->imgActibve);
	engineGraphicImageFrameInit(&this->imgInactive);
	engineGraphicImageTextInit(&this->imgText);
	this->imgNormal.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICNORMAL_X;
	this->imgNormal.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y;
	this->imgNormal.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICNORMAL_W;
	this->imgNormal.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICNORMAL_H;
	this->imgSelect.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICSELECT_X;
	this->imgSelect.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICSELECT_Y;
	this->imgSelect.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICSELECT_W;
	this->imgSelect.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICSELECT_H;
	this->imgActibve.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICACTIVE_X;
	this->imgActibve.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y;
	this->imgActibve.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICACTIVE_W;
	this->imgActibve.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICACTIVE_H;
	this->imgInactive.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X;
	this->imgInactive.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y;
	this->imgInactive.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W;
	this->imgInactive.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H;
	this->imgNormal.createArrayInfo.border.bt = 24;
	this->imgNormal.createArrayInfo.border.bb = 24;
	this->imgSelect.createArrayInfo.border.bt = 24;
	this->imgSelect.createArrayInfo.border.bb = 24;
	this->imgActibve.createArrayInfo.border.bt = 24;
	this->imgActibve.createArrayInfo.border.bb = 24;
	this->imgInactive.createArrayInfo.border.bt = 24;
	this->imgInactive.createArrayInfo.border.bb = 24;
	this->imgText.createArrayInfo.xalign = 0;
	this->imgText.createArrayInfo.yalign = 0;
	this->select = false;
}

// 表示箱付きボタン構造体 静的位置設定
void enginePartsButtonBoxSetPosition(struct enginePartsButtonBox *this, double x, double y, double w, double h){
	enginePartsButtonSetPosition(&this->super, x, y, w, h);
	this->imgNormal.x = x;
	this->imgNormal.y = y;
	this->imgNormal.w = w;
	this->imgNormal.h = h;
	this->imgSelect.x = x;
	this->imgSelect.y = y;
	this->imgSelect.w = w;
	this->imgSelect.h = h;
	this->imgActibve.x = x;
	this->imgActibve.y = y;
	this->imgActibve.w = w;
	this->imgActibve.h = h;
	this->imgInactive.x = x;
	this->imgInactive.y = y;
	this->imgInactive.w = w;
	this->imgInactive.h = h;
}

// 表示箱付きボタン構造体 計算
void enginePartsButtonBoxCalc(struct enginePartsButtonBox *this){
	enginePartsButtonCalc(&this->super);
}

// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void enginePartsButtonBoxSubCalc(struct enginePartsButtonBox *this, struct engineCtrlTouch *t, bool clickable){
	enginePartsButtonSubCalc(&this->super, t, clickable);
}

// 配列に表示箱付きボタンの描画情報を追加
void enginePartsButtonBoxCreateArray(struct enginePartsButtonBox *this, char *text){
	engineGraphicImageFrameCreateArray(&this->imgNormal, this->super.x, this->super.y, this->super.w, this->super.h);
	engineGraphicImageFrameCreateArray(&this->imgSelect, this->super.x, this->super.y, this->super.w, this->super.h);
	engineGraphicImageFrameCreateArray(&this->imgActibve, this->super.x, this->super.y, this->super.w, this->super.h);
	engineGraphicImageFrameCreateArray(&this->imgInactive, this->super.x, this->super.y, this->super.w, this->super.h);
	engineGraphicImageTextCreateArray(&this->imgText, 0, 0, text);
	this->imgNormal.trans.parent = &this->trans;
	this->imgSelect.trans.parent = &this->trans;
	this->imgActibve.trans.parent = &this->trans;
	this->imgInactive.trans.parent = &this->trans;
	this->imgText.trans.parent = &this->trans;
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 描画
void enginePartsButtonBoxDraw(struct enginePartsButtonBox *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	bool isInactive = (this->super.inactive && !this->super.active && !this->select);
	// 領域内確認行列読み込み
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineGraphicTransMultMatrix(&this->trans, &tempMat1);
	enginePartsButtonSetMatrix(&this->super, &tempMat1);
	// ボタン枠描画
	if(isInactive){engineGraphicImageFrameDraw(&this->imgInactive, mat, color);}
	else if(this->super.active){engineGraphicImageFrameDraw(&this->imgActibve, mat, color);}
	else if(this->select){engineGraphicImageFrameDraw(&this->imgSelect, mat, color);}
	else{engineGraphicImageFrameDraw(&this->imgNormal, mat, color);}
	// 文字列描画
	double x = this->super.x + this->super.w * 0.5;
	double y = this->super.y + this->super.h * 0.5 + (this->super.active ? 1 : -1);
	double bright = isInactive ? 0.5 : 0.0;
	engineGraphicTransSetTranslate(&this->imgText.trans, x, y, 0.0);
	engineGraphicTransSetColorRgba(&this->imgText.trans, bright, bright, bright, 1.0);
	engineGraphicImageTextDraw(&this->imgText, mat, color);
}

// 表示箱付きボタン構造体 破棄
void enginePartsButtonBoxDispose(struct enginePartsButtonBox *this){
	engineGraphicImageFrameDispose(&this->imgNormal);
	engineGraphicImageFrameDispose(&this->imgSelect);
	engineGraphicImageFrameDispose(&this->imgActibve);
	engineGraphicImageFrameDispose(&this->imgInactive);
	engineGraphicImageTextDispose(&this->imgText);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

