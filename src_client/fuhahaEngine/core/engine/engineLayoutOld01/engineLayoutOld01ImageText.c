#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayoutOld01/engineLayoutOld01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutOld01Trans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineLayoutOld01ImageText *this = (struct engineLayoutOld01ImageText*)((char*)that - offsetof(struct engineLayoutOld01ImageText, trans));
	engineLayoutOld01ImageTextDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayoutOld01Trans *that){
	struct engineLayoutOld01ImageText *this = (struct engineLayoutOld01ImageText*)((char*)that - offsetof(struct engineLayoutOld01ImageText, trans));
	engineLayoutOld01ImageTextDispose(this);
}

// ----------------------------------------------------------------

// 文字列描画構造体 初期化
void engineLayoutOld01ImageTextInit(struct engineLayoutOld01ImageText *this){
	engineLayoutOld01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	this->createArrayInfo.imgw = TEXSIZ_SYSTEM_W;
	this->createArrayInfo.imgh = TEXSIZ_SYSTEM_H;
	this->createArrayInfo.alphabet.tu = TEXPOS_SYSTEM_FONTABCD_X;
	this->createArrayInfo.alphabet.tv = TEXPOS_SYSTEM_FONTABCD_Y;
	this->createArrayInfo.alphabet.tw = TEXPOS_SYSTEM_FONTABCD_W;
	this->createArrayInfo.alphabet.th = TEXPOS_SYSTEM_FONTABCD_H;
	this->createArrayInfo.hiragana.tu = TEXPOS_SYSTEM_FONTHIRA_X;
	this->createArrayInfo.hiragana.tv = TEXPOS_SYSTEM_FONTHIRA_Y;
	this->createArrayInfo.hiragana.tw = TEXPOS_SYSTEM_FONTHIRA_W;
	this->createArrayInfo.hiragana.th = TEXPOS_SYSTEM_FONTHIRA_H;
	this->createArrayInfo.katakana.tu = TEXPOS_SYSTEM_FONTKATA_X;
	this->createArrayInfo.katakana.tv = TEXPOS_SYSTEM_FONTKATA_Y;
	this->createArrayInfo.katakana.tw = TEXPOS_SYSTEM_FONTKATA_W;
	this->createArrayInfo.katakana.th = TEXPOS_SYSTEM_FONTKATA_H;
	this->createArrayInfo.scale = 0.5;
	this->createArrayInfo.xalign = 0;
	this->createArrayInfo.yalign = 0;
}

// 文字コード(utf8)を確認
static unsigned int getUtf8Code(unsigned char *word){
	unsigned int code = 0;
	if(*word <= 0x7f){code = *(word + 0) & 0x7f;}
	else if(*word <= 0xbf){}
	else if(*word <= 0xdf){code = ((*(word + 0) & 0x1f) << 6) + (*(word + 1) & 0x3f);}
	else if(*word <= 0xef){code = ((*(word + 0) & 0x0f) << 12) + ((*(word + 1) & 0x3f) << 6) + (*(word + 2) & 0x3f);}
	else if(*word <= 0xf7){code = ((*(word + 0) & 0x07) << 18) + ((*(word + 1) & 0x3f) << 12) + ((*(word + 2) & 0x3f) << 6) + (*(word + 3) & 0x3f);}
	return code;
}

// 行数の計算
static int getColNum(char *text){
	int colnum = 1;
	while(*text){
		if(*text == '\n'){colnum++;}
		text++;
	}
	return colnum;
}

// 行に含まれる文字数の計算
static int getRowNum(char *text){
	int rownum = 0;
	while(*text && *text != '\n'){
		unsigned int code = getUtf8Code((unsigned char *)text);
		if(0x1f <= code && code !=0x7f){rownum++;}
		text++;
	}
	return rownum;
}

// 配列に文字列(utf8)の描画情報を追加
void engineLayoutOld01ImageTextCreateArray(struct engineLayoutOld01ImageText *this, double x, double y, char *text){
	if(this == NULL){return;}

	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 0;
	int col = 0;
	int row = 0;
	int colmax = getColNum(text);
	int rowmax = getRowNum(text);
	while(*text){
		if(*text == '\n'){
			col++;
			row = 0;
			rowmax = getRowNum(text + 1);
		}else{
			unsigned int code = getUtf8Code((unsigned char *)text);
			if(0x1f <= code && code !=0x7f){
				int tu = this->createArrayInfo.alphabet.tu;
				int tv = this->createArrayInfo.alphabet.tv;
				int tw = this->createArrayInfo.alphabet.tw / 16;
				int th = this->createArrayInfo.alphabet.th / 6;
				if(0x20 <= code && code <= 0x7f){
					code = code - 0x20;
				}else if(0x3040 <= code && code <= 0x309f){
					tu = this->createArrayInfo.hiragana.tu;
					tv = this->createArrayInfo.hiragana.tv;
					tw = this->createArrayInfo.alphabet.tw / 16;
					th = this->createArrayInfo.alphabet.th / 6;
					code = code - 0x3040;
				}else if(0x30a0 <= code && code <= 0x30ff){
					tu = this->createArrayInfo.katakana.tu;
					tv = this->createArrayInfo.katakana.tv;
					tw = this->createArrayInfo.alphabet.tw / 16;
					th = this->createArrayInfo.alphabet.th / 6;
					code = code - 0x30a0;
				}else{
					code = 0;
				}
				double w1 = tw * this->createArrayInfo.scale;
				double h1 = th * this->createArrayInfo.scale;
				double x1 = x + (row - rowmax * ((this->createArrayInfo.xalign > 0) ? 0.0 : (this->createArrayInfo.xalign == 0) ? 0.5 : 1.0)) * w1;
				double y1 = y + (col - colmax * ((this->createArrayInfo.yalign > 0) ? 0.0 : (this->createArrayInfo.yalign == 0) ? 0.5 : 1.0)) * h1;
				int u1 = tu + tw * (code % 16);
				int v1 = tv + th * engineMathFloor(code / 16);
				engineGraphicBufferPushTetraVert(x1, y1, w1, h1);
				engineGraphicBufferPushTetraTexc(this->createArrayInfo.imgw, this->createArrayInfo.imgh, u1, v1, tw, th);
				tetraNum++;
				row++;
			}
		}
		text++;
	}

	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// ----------------------------------------------------------------

// 文字列描画構造体 描画
void engineLayoutOld01ImageTextDraw(struct engineLayoutOld01ImageText *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	engineLayoutOld01TransBindAll(&this->trans, mat, color);
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// 文字列描画構造体 破棄
void engineLayoutOld01ImageTextDispose(struct engineLayoutOld01ImageText *this){
	this->faceIndex = 0;
	this->faceNum = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

