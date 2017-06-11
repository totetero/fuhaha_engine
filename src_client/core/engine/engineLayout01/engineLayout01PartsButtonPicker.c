#include "../../library.h"
#include "../../define/texpos.h"
#include "../engineMath/engineMath.h"
#include "../engineCtrl/engineCtrl.h"
#include "../engineGraphic/engineGraphic.h"
#include "engineLayout01.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayout01Trans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineLayout01ButtonPicker *this = (struct engineLayout01ButtonPicker*)((char*)that - offsetof(struct engineLayout01ButtonPicker, trans));
	engineLayout01ButtonPickerDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayout01Trans *that){
	struct engineLayout01ButtonPicker *this = (struct engineLayout01ButtonPicker*)((char*)that - offsetof(struct engineLayout01ButtonPicker, trans));
	engineLayout01ButtonPickerDispose(this);
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 初期化
void engineLayout01ButtonPickerInit(struct engineLayout01ButtonPicker *this, int picked){
	engineLayout01ButtonInit(&this->super);
	engineLayout01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	engineLayout01ImageRectInit(&this->imgRectNormal);
	engineLayout01ImageRectInit(&this->imgRectSelect);
	engineLayout01ImageRectInit(&this->imgRectActibve);
	engineLayout01ImageRectInit(&this->imgRectInactive);
	engineLayout01ImageFrameInit(&this->imgFrameNormal);
	engineLayout01ImageFrameInit(&this->imgFrameSelect);
	engineLayout01ImageFrameInit(&this->imgFrameActibve);
	engineLayout01ImageFrameInit(&this->imgFrameInactive);
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
		engineLayout01ImageTextInit(&this->imgText[i]);
		this->imgText[i].createArrayInfo.xalign = 1;
		this->imgText[i].createArrayInfo.yalign = 0;
	}
	this->picked = picked;
	this->select = false;
}

// 表示箱付きボタン構造体 静的位置設定
void engineLayout01ButtonPickerSetPosition(struct engineLayout01ButtonPicker *this, double x, double y, double w){
	double h = 30;
	engineLayout01ButtonSetPosition(&this->super, x, y, w, h);
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
void engineLayout01ButtonPickerCalc(struct engineLayout01ButtonPicker *this){
	engineLayout01ButtonCalc(&this->super);
}

// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void engineLayout01ButtonPickerSubCalc(struct engineLayout01ButtonPicker *this, struct engineCtrlTouch *t, bool clickable){
	engineLayout01ButtonSubCalc(&this->super, t, clickable);
}

// 配列に表示箱付きボタンの描画情報を追加
void engineLayout01ButtonPickerCreateArray(struct engineLayout01ButtonPicker *this, char **textList, int textListLength){
	int fx = this->super.x + 28;
	int fy = this->super.y +  3;
	int fw = this->super.w - 28;
	int fh = this->super.h -  6;
	engineLayout01ImageRectCreateArray(&this->imgRectNormal, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_XYWH);
	engineLayout01ImageRectCreateArray(&this->imgRectSelect, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERSELECT_XYWH);
	engineLayout01ImageRectCreateArray(&this->imgRectActibve, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_XYWH);
	engineLayout01ImageRectCreateArray(&this->imgRectInactive, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_XYWH);
	engineLayout01ImageFrameCreateArray(&this->imgFrameNormal, fx, fy, fw, fh);
	engineLayout01ImageFrameCreateArray(&this->imgFrameSelect, fx, fy, fw, fh);
	engineLayout01ImageFrameCreateArray(&this->imgFrameActibve, fx, fy, fw, fh);
	engineLayout01ImageFrameCreateArray(&this->imgFrameInactive, fx, fy, fw, fh);
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
		engineLayout01ImageTextCreateArray(&this->imgText[i], 0, 0, textList[i]);
		this->imgText[i].trans.parent = &this->trans;
	}
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 描画
void engineLayout01ButtonPickerDraw(struct engineLayout01ButtonPicker *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	bool isInactive = (this->super.inactive && !this->super.active && !this->select);
	// 領域内確認行列読み込み
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineLayout01TransMultMatrix(&this->trans, &tempMat1);
	engineLayout01ButtonSetMatrix(&this->super, &tempMat1);
	// ボタン枠描画
	if(isInactive){
		engineLayout01TransSetTranslate(&this->imgRectInactive.trans, this->super.x, this->super.y, 0.0);
		engineLayout01ImageRectDraw(&this->imgRectInactive, mat, color);
		engineLayout01ImageFrameDraw(&this->imgFrameInactive, mat, color);
	}else if(this->super.active){
		engineLayout01TransSetTranslate(&this->imgRectActibve.trans, this->super.x, this->super.y, 0.0);
		engineLayout01ImageRectDraw(&this->imgRectActibve, mat, color);
		engineLayout01ImageFrameDraw(&this->imgFrameActibve, mat, color);
	}else if(this->select){
		engineLayout01TransSetTranslate(&this->imgRectSelect.trans, this->super.x, this->super.y, 0.0);
		engineLayout01ImageRectDraw(&this->imgRectSelect, mat, color);
		engineLayout01ImageFrameDraw(&this->imgFrameSelect, mat, color);
	}else{
		engineLayout01TransSetTranslate(&this->imgRectNormal.trans, this->super.x, this->super.y, 0.0);
		engineLayout01ImageRectDraw(&this->imgRectNormal, mat, color);
		engineLayout01ImageFrameDraw(&this->imgFrameNormal, mat, color);
	}
	if(0 <= this->picked && this->picked < (int)(sizeof(this->imgText) / sizeof(*this->imgText))){
		// 文字列描画
		double x = this->super.x + 28 + 5;
		double y = this->super.y + 15 + (this->super.active ? 1 : -1);
		double bright = isInactive ? 0.5 : 0.0;
		engineLayout01TransSetTranslate(&this->imgText[this->picked].trans, x, y, 0.0);
		engineLayout01TransSetColorRgba(&this->imgText[this->picked].trans, bright, bright, bright, 1.0);
		engineLayout01ImageTextDraw(&this->imgText[this->picked], mat, color);
	}
}

// 表示箱付きボタン構造体 破棄
void engineLayout01ButtonPickerDispose(struct engineLayout01ButtonPicker *this){
	engineLayout01ImageRectDispose(&this->imgRectNormal);
	engineLayout01ImageRectDispose(&this->imgRectSelect);
	engineLayout01ImageRectDispose(&this->imgRectActibve);
	engineLayout01ImageRectDispose(&this->imgRectInactive);
	engineLayout01ImageFrameDispose(&this->imgFrameNormal);
	engineLayout01ImageFrameDispose(&this->imgFrameSelect);
	engineLayout01ImageFrameDispose(&this->imgFrameActibve);
	engineLayout01ImageFrameDispose(&this->imgFrameInactive);
	for(int i = 0; i < (int)(sizeof(this->imgText) / sizeof(*this->imgText)); i++){
		engineLayout01ImageTextDispose(&this->imgText[i]);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

