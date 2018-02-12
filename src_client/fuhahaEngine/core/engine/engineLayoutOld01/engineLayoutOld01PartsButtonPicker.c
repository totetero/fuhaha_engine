#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayoutOld01/engineLayoutOld01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutOld01Trans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineLayoutOld01ButtonPicker *this = (struct engineLayoutOld01ButtonPicker*)((char*)that - offsetof(struct engineLayoutOld01ButtonPicker, trans));
	engineLayoutOld01ButtonPickerDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayoutOld01Trans *that){
	struct engineLayoutOld01ButtonPicker *this = (struct engineLayoutOld01ButtonPicker*)((char*)that - offsetof(struct engineLayoutOld01ButtonPicker, trans));
	engineLayoutOld01ButtonPickerDispose(this);
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 初期化
void engineLayoutOld01ButtonPickerInit(struct engineLayoutOld01ButtonPicker *this, int picked){
	engineLayoutOld01ButtonInit(&this->super);
	engineLayoutOld01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	engineLayoutOld01ImageRectInit(&this->imgRectNormal);
	engineLayoutOld01ImageRectInit(&this->imgRectSelect);
	engineLayoutOld01ImageRectInit(&this->imgRectActibve);
	engineLayoutOld01ImageRectInit(&this->imgRectInactive);
	engineLayoutOld01ImageFrameInit(&this->imgFrameNormal);
	engineLayoutOld01ImageFrameInit(&this->imgFrameSelect);
	engineLayoutOld01ImageFrameInit(&this->imgFrameActibve);
	engineLayoutOld01ImageFrameInit(&this->imgFrameInactive);
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
		engineLayoutOld01ImageTextInit(&this->imgText[i]);
		this->imgText[i].createArrayInfo.xalign = 1;
		this->imgText[i].createArrayInfo.yalign = 0;
	}
	this->picked = picked;
	this->select = false;
}

// 表示箱付きボタン構造体 静的位置設定
void engineLayoutOld01ButtonPickerSetPosition(struct engineLayoutOld01ButtonPicker *this, double x, double y, double w){
	double h = 30;
	engineLayoutOld01ButtonSetPosition(&this->super, x, y, w, h);
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
void engineLayoutOld01ButtonPickerCalc(struct engineLayoutOld01ButtonPicker *this){
	engineLayoutOld01ButtonCalc(&this->super);
}

// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void engineLayoutOld01ButtonPickerSubCalc(struct engineLayoutOld01ButtonPicker *this, struct engineCtrlTouch *t, bool clickable){
	engineLayoutOld01ButtonSubCalc(&this->super, t, clickable);
}

// 配列に表示箱付きボタンの描画情報を追加
void engineLayoutOld01ButtonPickerCreateArray(struct engineLayoutOld01ButtonPicker *this, char **textList, int textListLength){
	int fx = this->super.x + 28;
	int fy = this->super.y +  3;
	int fw = this->super.w - 28;
	int fh = this->super.h -  6;
	engineLayoutOld01ImageRectCreateArray(&this->imgRectNormal, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_XYWH);
	engineLayoutOld01ImageRectCreateArray(&this->imgRectSelect, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERSELECT_XYWH);
	engineLayoutOld01ImageRectCreateArray(&this->imgRectActibve, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_XYWH);
	engineLayoutOld01ImageRectCreateArray(&this->imgRectInactive, TEXSIZ_SYSTEM_WH, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_XYWH);
	engineLayoutOld01ImageFrameCreateArray(&this->imgFrameNormal, fx, fy, fw, fh);
	engineLayoutOld01ImageFrameCreateArray(&this->imgFrameSelect, fx, fy, fw, fh);
	engineLayoutOld01ImageFrameCreateArray(&this->imgFrameActibve, fx, fy, fw, fh);
	engineLayoutOld01ImageFrameCreateArray(&this->imgFrameInactive, fx, fy, fw, fh);
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
		engineLayoutOld01ImageTextCreateArray(&this->imgText[i], 0, 0, textList[i]);
		this->imgText[i].trans.parent = &this->trans;
	}
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 描画
void engineLayoutOld01ButtonPickerDraw(struct engineLayoutOld01ButtonPicker *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	bool isInactive = (this->super.inactive && !this->super.active && !this->select);
	// 領域内確認行列読み込み
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineLayoutOld01TransMultMatrix(&this->trans, &tempMat1);
	engineLayoutOld01ButtonSetMatrix(&this->super, &tempMat1);
	// ボタン枠描画
	if(isInactive){
		engineLayoutOld01TransSetTranslate(&this->imgRectInactive.trans, this->super.x, this->super.y, 0.0);
		engineLayoutOld01ImageRectDraw(&this->imgRectInactive, mat, color);
		engineLayoutOld01ImageFrameDraw(&this->imgFrameInactive, mat, color);
	}else if(this->super.active){
		engineLayoutOld01TransSetTranslate(&this->imgRectActibve.trans, this->super.x, this->super.y, 0.0);
		engineLayoutOld01ImageRectDraw(&this->imgRectActibve, mat, color);
		engineLayoutOld01ImageFrameDraw(&this->imgFrameActibve, mat, color);
	}else if(this->select){
		engineLayoutOld01TransSetTranslate(&this->imgRectSelect.trans, this->super.x, this->super.y, 0.0);
		engineLayoutOld01ImageRectDraw(&this->imgRectSelect, mat, color);
		engineLayoutOld01ImageFrameDraw(&this->imgFrameSelect, mat, color);
	}else{
		engineLayoutOld01TransSetTranslate(&this->imgRectNormal.trans, this->super.x, this->super.y, 0.0);
		engineLayoutOld01ImageRectDraw(&this->imgRectNormal, mat, color);
		engineLayoutOld01ImageFrameDraw(&this->imgFrameNormal, mat, color);
	}
	if(0 <= this->picked && this->picked < (int)(sizeof(this->imgText) / sizeof(*this->imgText))){
		// 文字列描画
		double x = this->super.x + 28 + 5;
		double y = this->super.y + 15 + (this->super.active ? 1 : -1);
		double bright = isInactive ? 0.5 : 0.0;
		engineLayoutOld01TransSetTranslate(&this->imgText[this->picked].trans, x, y, 0.0);
		engineLayoutOld01TransSetColorRgba(&this->imgText[this->picked].trans, bright, bright, bright, 1.0);
		engineLayoutOld01ImageTextDraw(&this->imgText[this->picked], mat, color);
	}
}

// 表示箱付きボタン構造体 破棄
void engineLayoutOld01ButtonPickerDispose(struct engineLayoutOld01ButtonPicker *this){
	engineLayoutOld01ImageRectDispose(&this->imgRectNormal);
	engineLayoutOld01ImageRectDispose(&this->imgRectSelect);
	engineLayoutOld01ImageRectDispose(&this->imgRectActibve);
	engineLayoutOld01ImageRectDispose(&this->imgRectInactive);
	engineLayoutOld01ImageFrameDispose(&this->imgFrameNormal);
	engineLayoutOld01ImageFrameDispose(&this->imgFrameSelect);
	engineLayoutOld01ImageFrameDispose(&this->imgFrameActibve);
	engineLayoutOld01ImageFrameDispose(&this->imgFrameInactive);
	for(int i = 0; i < (int)(sizeof(this->imgText) / sizeof(*this->imgText)); i++){
		engineLayoutOld01ImageTextDispose(&this->imgText[i]);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

