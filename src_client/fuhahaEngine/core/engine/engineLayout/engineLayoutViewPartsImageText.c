#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsImageTextImplement{
	struct engineLayoutViewPartsImageText super;

	struct engineLayoutViewPartsImageTextBufferCompare{
		int generationCount;
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} alphabet;
		struct{int tu; int tv;} hiragana;
		struct{int tu; int tv;} katakana;
		struct{
			int xalign;
			int yalign;
		} fontStyle;
	} bufferCompare;
	engineGraphicTextureId egoIdTexTest;

	int generationCount;
	struct{char *buff; int length;} textInfo;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsImageTextImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	engineLayoutViewUtilGraphicObjectInit((struct engineLayoutView*)this);

	// デフォルトパラメータ設定
	this->super.fontStyle.scale = 0.5;
	this->super.color.r = 1.0;
	this->super.color.g = 1.0;
	this->super.color.b = 1.0;
	this->super.color.a = 1.0;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsImageTextImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsImageTextImplement *this){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsImageTextImplement *this){
	struct engineLayoutViewPartsImageTextBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;
	bufferCompare.alphabet.imgw = this->super.alphabet.imgw;
	bufferCompare.alphabet.imgh = this->super.alphabet.imgh;
	bufferCompare.alphabet.tu = this->super.alphabet.tu;
	bufferCompare.alphabet.tv = this->super.alphabet.tv;
	bufferCompare.alphabet.tw = this->super.alphabet.tw;
	bufferCompare.alphabet.th = this->super.alphabet.th;
	bufferCompare.hiragana.tu = this->super.hiragana.tu;
	bufferCompare.hiragana.tv = this->super.hiragana.tv;
	bufferCompare.katakana.tu = this->super.katakana.tu;
	bufferCompare.katakana.tv = this->super.katakana.tv;
	bufferCompare.fontStyle.xalign = this->super.fontStyle.xalign;
	bufferCompare.fontStyle.yalign = this->super.fontStyle.yalign;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsImageTextBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsImageTextBufferCompare));
		return true;
	}else{
		return false;
	}
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

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsImageTextImplement *this){
	// バッファポインタ取得
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 0;

	if(this->textInfo.buff != NULL){
		// 頂点座標データとテクスチャ座標データを生成
		char *textPtr = this->textInfo.buff;
		int col = 0;
		int row = 0;
		int colmax = getColNum(textPtr);
		int rowmax = getRowNum(textPtr);
		int tw = this->super.alphabet.tw / 16;
		int th = this->super.alphabet.th / 6;
		while(*textPtr){
			if(*textPtr == '\n'){
				textPtr++;
				col++;
				row = 0;
				rowmax = getRowNum(textPtr);
			}else{
				unsigned int code = getUtf8Code((unsigned char *)textPtr);
				if(0x1f <= code && code !=0x7f){
					int tu = this->super.alphabet.tu;
					int tv = this->super.alphabet.tv;
					if(0x20 <= code && code <= 0x7f){
						code = code - 0x20;
					}else if(0x3040 <= code && code <= 0x309f){
						tu = this->super.hiragana.tu;
						tv = this->super.hiragana.tv;
						code = code - 0x3040;
					}else if(0x30a0 <= code && code <= 0x30ff){
						tu = this->super.katakana.tu;
						tv = this->super.katakana.tv;
						code = code - 0x30a0;
					}else{
						code = 0;
					}
					double x1 = (row - rowmax * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0)) * tw;
					double y1 = (col - colmax * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0)) * th;
					int u1 = tu + tw * (code % 16);
					int v1 = tv + th * engineMathFloor(code / 16);
					engineGraphicBufferPushTetraVert(x1, y1, tw, th);
					engineGraphicBufferPushTetraTexc(this->super.alphabet.imgw, this->super.alphabet.imgh, u1, v1, tw, th);
					tetraNum++;
					row++;
				}
				textPtr++;
			}
		}
	}

	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	// バッファパラメータ記録
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// 描画
static void draw(struct engineLayoutViewPartsImageTextImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexTest);
	engineGraphicEngineBindVertVBO(this->super.super.graphicObject.egoIdVert);
	engineGraphicEngineBindTexcVBO(this->super.super.graphicObject.egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->super.super.graphicObject.egoIdFace);
	// 行列登録
	double w = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
	double h = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
	struct engineMathMatrix44 tempMat1;
	engineLayoutViewUtilPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
	double x0 = w * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0);
	double y0 = h * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0);
	engineMathMat4Translate(&tempMat1, x0, y0, 0);
	engineMathMat4Scale(&tempMat1, this->super.fontStyle.scale, this->super.fontStyle.scale, 1.0);
	engineGraphicEngineSetMatrix(&tempMat1);
	// 色登録
	struct engineMathVector4 tempColor1;
	tempColor1.r = this->super.color.r * color->r;
	tempColor1.g = this->super.color.g * color->g;
	tempColor1.b = this->super.color.b * color->b;
	tempColor1.a = this->super.color.a * color->a;
	engineGraphicEngineSetColorVec(&tempColor1);

	// 描画
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsImageTextImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsImageTextImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexTest);
	if(this->textInfo.buff != NULL){engineUtilMemoryInfoFree("engineLayoutViewPartsImageText textBuff", this->textInfo.buff);}
	engineLayoutViewUtilGraphicObjectDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsImageText", this);
}

// ----------------------------------------------------------------

// 画像文字列描画構造体 作成
struct engineLayoutViewPartsImageText *engineLayoutViewPartsImageTextCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayoutViewPartsImageTextImplement *this = (struct engineLayoutViewPartsImageTextImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsImageText", 1, sizeof(struct engineLayoutViewPartsImageTextImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.alphabet.imgw = imgw;
	this->super.alphabet.imgh = imgh;
	this->super.alphabet.tu = tu;
	this->super.alphabet.tv = tv;
	this->super.alphabet.tw = tw;
	this->super.alphabet.th = th;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsImageText*)this;
}

// 画像文字列描画構造体 作成 デフォルト
struct engineLayoutViewPartsImageText *engineLayoutViewPartsImageTextCreateDefault(char *text){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tu = TEXPOS_SYSTEM_FONTABCD_X;
	int tv = TEXPOS_SYSTEM_FONTABCD_Y;
	int tw = TEXPOS_SYSTEM_FONTABCD_W;
	int th = TEXPOS_SYSTEM_FONTABCD_H;
	struct engineLayoutViewPartsImageText *this = engineLayoutViewPartsImageTextCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
	this->hiragana.tu = TEXPOS_SYSTEM_FONTHIRA_X;
	this->hiragana.tv = TEXPOS_SYSTEM_FONTHIRA_Y;
	this->katakana.tu = TEXPOS_SYSTEM_FONTKATA_X;
	this->katakana.tv = TEXPOS_SYSTEM_FONTKATA_Y;
	engineLayoutViewPartsImageTextSet(this, text);
	return this;
}

// 画像文字列描画構造体 文字列設定(utf8)
void engineLayoutViewPartsImageTextSet(struct engineLayoutViewPartsImageText *that, char *text){
	struct engineLayoutViewPartsImageTextImplement *this = (struct engineLayoutViewPartsImageTextImplement*)that;
	// 文字列領域作成
	int textLength = (text != NULL) ? ((int)strlen(text) + 1) : 0;
	if(this->textInfo.length < textLength){
		if(this->textInfo.buff != NULL){engineUtilMemoryInfoFree("engineLayoutViewPartsImageText textBuff", this->textInfo.buff);}
		this->textInfo.buff = (char*)engineUtilMemoryInfoMalloc("engineLayoutViewPartsImageText textBuff", textLength * sizeof(char));
		this->textInfo.length = textLength;
	}
	// 文字列保存
	if(this->textInfo.buff != NULL){
		if(text != NULL){
			strcpy(this->textInfo.buff, text);
		}else{
			strcpy(this->textInfo.buff, "");
		}
	}
	// 世代交代
	this->generationCount++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

