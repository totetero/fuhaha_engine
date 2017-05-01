#include "../../library.h"
#include "../../define/texpos.h"
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
	struct enginePartsButtonPicker *this = (struct enginePartsButtonPicker*)((char*)that - offsetof(struct enginePartsButtonPicker, trans));
	enginePartsButtonPickerDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineGraphicTrans *that){
	struct enginePartsButtonPicker *this = (struct enginePartsButtonPicker*)((char*)that - offsetof(struct enginePartsButtonPicker, trans));
	enginePartsButtonPickerDispose(this);
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 初期化
void enginePartsButtonPickerInit(struct enginePartsButtonPicker *this, int picked){
	enginePartsButtonInit(&this->super);
	engineGraphicTransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	engineGraphicImageRectInit(&this->imgRectNormal);
	engineGraphicImageRectInit(&this->imgRectSelect);
	engineGraphicImageRectInit(&this->imgRectActibve);
	engineGraphicImageRectInit(&this->imgRectInactive);
	engineGraphicImageFrameInit(&this->imgFrameNormal);
	engineGraphicImageFrameInit(&this->imgFrameSelect);
	engineGraphicImageFrameInit(&this->imgFrameActibve);
	engineGraphicImageFrameInit(&this->imgFrameInactive);
	this->imgFrameNormal.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICNORMAL_X + 20;
	this->imgFrameNormal.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y;
	this->imgFrameNormal.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICNORMAL_W - 20;
	this->imgFrameNormal.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICNORMAL_H;
	this->imgFrameSelect.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICSELECT_X + 20;
	this->imgFrameSelect.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICSELECT_Y;
	this->imgFrameSelect.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICSELECT_W - 20;
	this->imgFrameSelect.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICSELECT_H;
	this->imgFrameActibve.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICACTIVE_X + 20;
	this->imgFrameActibve.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y;
	this->imgFrameActibve.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICACTIVE_W - 20;
	this->imgFrameActibve.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICACTIVE_H;
	this->imgFrameInactive.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X + 20;
	this->imgFrameInactive.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y;
	this->imgFrameInactive.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W - 20;
	this->imgFrameInactive.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H;
	this->imgFrameNormal.createArrayInfo.border.bt = 24;
	this->imgFrameNormal.createArrayInfo.border.bb = 24;
	this->imgFrameNormal.createArrayInfo.border.bl = 0;
	this->imgFrameSelect.createArrayInfo.border.bt = 24;
	this->imgFrameSelect.createArrayInfo.border.bb = 24;
	this->imgFrameSelect.createArrayInfo.border.bl = 0;
	this->imgFrameActibve.createArrayInfo.border.bt = 24;
	this->imgFrameActibve.createArrayInfo.border.bb = 24;
	this->imgFrameActibve.createArrayInfo.border.bl = 0;
	this->imgFrameInactive.createArrayInfo.border.bt = 24;
	this->imgFrameInactive.createArrayInfo.border.bb = 24;
	this->imgFrameInactive.createArrayInfo.border.bl = 0;
	for(int i = 0; i < (int)(sizeof(this->imgText) / sizeof(*this->imgText)); i++){
		engineGraphicImageTextInit(&this->imgText[i]);
		this->imgText[i].createArrayInfo.xalign = 1;
		this->imgText[i].createArrayInfo.yalign = 0;
	}
	this->picked = picked;
	this->select = false;
}

// 表示箱付きボタン構造体 静的位置設定
void enginePartsButtonPickerSetPosition(struct enginePartsButtonPicker *this, double x, double y, double w){
	double h = 30;
	enginePartsButtonSetPosition(&this->super, x, y, w, h);
	this->imgFrameNormal.x = x + 28;
	this->imgFrameNormal.y = y +  3;
	this->imgFrameNormal.w = w - 28;
	this->imgFrameNormal.h = h -  6;
	this->imgFrameSelect.x = x + 28;
	this->imgFrameSelect.y = y +  3;
	this->imgFrameSelect.w = w - 28;
	this->imgFrameSelect.h = h -  6;
	this->imgFrameActibve.x = x + 28;
	this->imgFrameActibve.y = y +  3;
	this->imgFrameActibve.w = w - 28;
	this->imgFrameActibve.h = h -  6;
	this->imgFrameInactive.x = x + 28;
	this->imgFrameInactive.y = y +  3;
	this->imgFrameInactive.w = w - 28;
	this->imgFrameInactive.h = h -  6;
}

// 表示箱付きボタン構造体 計算
void enginePartsButtonPickerCalc(struct enginePartsButtonPicker *this){
	enginePartsButtonCalc(&this->super);
}

// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void enginePartsButtonPickerSubCalc(struct enginePartsButtonPicker *this, struct engineCtrlTouch *t, bool clickable){
	enginePartsButtonSubCalc(&this->super, t, clickable);
}

// 配列に表示箱付きボタンの描画情報を追加
void enginePartsButtonPickerCreateArray(struct enginePartsButtonPicker *this, char **textList, int textListLength){
	int fx = this->super.x + 28;
	int fy = this->super.y +  3;
	int fw = this->super.w - 28;
	int fh = this->super.h -  6;
	engineGraphicImageRectCreateArray(&this->imgRectNormal, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_XYWH);
	engineGraphicImageRectCreateArray(&this->imgRectSelect, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERSELECT_XYWH);
	engineGraphicImageRectCreateArray(&this->imgRectActibve, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_XYWH);
	engineGraphicImageRectCreateArray(&this->imgRectInactive, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_XYWH);
	engineGraphicImageFrameCreateArray(&this->imgFrameNormal, fx, fy, fw, fh);
	engineGraphicImageFrameCreateArray(&this->imgFrameSelect, fx, fy, fw, fh);
	engineGraphicImageFrameCreateArray(&this->imgFrameActibve, fx, fy, fw, fh);
	engineGraphicImageFrameCreateArray(&this->imgFrameInactive, fx, fy, fw, fh);
	this->imgRectNormal.trans.parent = &this->trans;
	this->imgRectSelect.trans.parent = &this->trans;
	this->imgRectActibve.trans.parent = &this->trans;
	this->imgRectInactive.trans.parent = &this->trans;
	this->imgFrameNormal.trans.parent = &this->trans;
	this->imgFrameSelect.trans.parent = &this->trans;
	this->imgFrameActibve.trans.parent = &this->trans;
	this->imgFrameInactive.trans.parent = &this->trans;
	for(int i = 0; i < (int)(sizeof(this->imgText) / sizeof(*this->imgText)); i++){
		if(i >= textListLength){break;}
		engineGraphicImageTextCreateArray(&this->imgText[i], 0, 0, textList[i]);
		this->imgText[i].trans.parent = &this->trans;
	}
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 描画
void enginePartsButtonPickerDraw(struct enginePartsButtonPicker *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	bool isInactive = (this->super.inactive && !this->super.active && !this->select);
	// 領域内確認行列読み込み
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineGraphicTransMultMatrix(&this->trans, &tempMat1);
	enginePartsButtonSetMatrix(&this->super, &tempMat1);
	// ボタン枠描画
	if(isInactive){
		engineGraphicTransSetTranslate(&this->imgRectInactive.trans, this->super.x, this->super.y, 0.0);
		engineGraphicImageRectDraw(&this->imgRectInactive, mat, color);
		engineGraphicImageFrameDraw(&this->imgFrameInactive, mat, color);
	}else if(this->super.active){
		engineGraphicTransSetTranslate(&this->imgRectActibve.trans, this->super.x, this->super.y, 0.0);
		engineGraphicImageRectDraw(&this->imgRectActibve, mat, color);
		engineGraphicImageFrameDraw(&this->imgFrameActibve, mat, color);
	}else if(this->select){
		engineGraphicTransSetTranslate(&this->imgRectSelect.trans, this->super.x, this->super.y, 0.0);
		engineGraphicImageRectDraw(&this->imgRectSelect, mat, color);
		engineGraphicImageFrameDraw(&this->imgFrameSelect, mat, color);
	}else{
		engineGraphicTransSetTranslate(&this->imgRectNormal.trans, this->super.x, this->super.y, 0.0);
		engineGraphicImageRectDraw(&this->imgRectNormal, mat, color);
		engineGraphicImageFrameDraw(&this->imgFrameNormal, mat, color);
	}
	if(0 <= this->picked && this->picked < (int)(sizeof(this->imgText) / sizeof(*this->imgText))){
		// 文字列描画
		double x = this->super.x + 28 + 5;
		double y = this->super.y + 15 + (this->super.active ? 1 : -1);
		double bright = isInactive ? 0.5 : 0.0;
		engineGraphicTransSetTranslate(&this->imgText[this->picked].trans, x, y, 0.0);
		engineGraphicTransSetColorRgba(&this->imgText[this->picked].trans, bright, bright, bright, 1.0);
		engineGraphicImageTextDraw(&this->imgText[this->picked], mat, color);
	}
}

// 表示箱付きボタン構造体 破棄
void enginePartsButtonPickerDispose(struct enginePartsButtonPicker *this){
	engineGraphicImageRectDispose(&this->imgRectNormal);
	engineGraphicImageRectDispose(&this->imgRectSelect);
	engineGraphicImageRectDispose(&this->imgRectActibve);
	engineGraphicImageRectDispose(&this->imgRectInactive);
	engineGraphicImageFrameDispose(&this->imgFrameNormal);
	engineGraphicImageFrameDispose(&this->imgFrameSelect);
	engineGraphicImageFrameDispose(&this->imgFrameActibve);
	engineGraphicImageFrameDispose(&this->imgFrameInactive);
	for(int i = 0; i < (int)(sizeof(this->imgText) / sizeof(*this->imgText)); i++){
		engineGraphicImageTextDispose(&this->imgText[i]);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

