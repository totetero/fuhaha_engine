#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout02/engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayout02ViewPartsImageTextImplement{
	struct engineLayout02ViewPartsImageText super;

	struct engineLayout02ViewPartsImageTextBufferCompare{
		int generationCount;
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} alphabet;
		struct{int tu; int tv;} hiragana;
		struct{int tu; int tv;} katakana;
		struct{
			int xalign;
			int yalign;
		} fontStyle;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexTest;

	int generationCount;
	struct{char *buff; int length;} textInfo;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02ViewPartsImageTextImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilFamilyInit((struct engineLayout02View*)this);
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);

	// デフォルトパラメータ設定
	this->super.fontStyle.scale = 0.5;
	this->super.color.r = 1.0;
	this->super.color.g = 1.0;
	this->super.color.b = 1.0;
	this->super.color.a = 1.0;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsImageTextImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsImageTextImplement *this){
	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

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

// バッファ配列作成
static void createBufferArrayText(struct engineLayout02ViewPartsImageTextImplement *this){
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

// バッファ作成
static void createBuffer(struct engineLayout02ViewPartsImageTextImplement *this){
	struct engineLayout02ViewPartsImageTextBufferCompare bufferCompare;
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

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsImageTextBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsImageTextBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		createBufferArrayText(this);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct engineLayout02ViewPartsImageTextImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 描画準備
	createBuffer(this);

	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexTest);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	// 行列登録
	double w = engineLayout02ViewUtilPositionGetW((struct engineLayout02View*)this);
	double h = engineLayout02ViewUtilPositionGetH((struct engineLayout02View*)this);
	struct engineMathMatrix44 tempMat1;
	engineLayout02ViewUtilPositionTransformCalcMatrix((struct engineLayout02View*)this, &tempMat1, mat);
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
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsImageTextImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsImageTextImplement *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	if(this->textInfo.buff != NULL){engineUtilMemoryInfoFree("engineLayout02ViewPartsImageText textBuff", this->textInfo.buff);}
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewUtilFamilyDispose((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsImageText", this);
}

// ----------------------------------------------------------------

// 画像文字列描画構造体 作成
struct engineLayout02ViewPartsImageText *engineLayout02ViewPartsImageTextCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayout02ViewPartsImageTextImplement *this = (struct engineLayout02ViewPartsImageTextImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsImageText", 1, sizeof(struct engineLayout02ViewPartsImageTextImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.alphabet.imgw = imgw;
	this->super.alphabet.imgh = imgh;
	this->super.alphabet.tu = tu;
	this->super.alphabet.tv = tv;
	this->super.alphabet.tw = tw;
	this->super.alphabet.th = th;

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsImageText*)this;
}

// 画像文字列描画構造体 作成 デフォルト
struct engineLayout02ViewPartsImageText *engineLayout02ViewPartsImageTextCreateDefault(char *text){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tu = TEXPOS_SYSTEM_FONTABCD_X;
	int tv = TEXPOS_SYSTEM_FONTABCD_Y;
	int tw = TEXPOS_SYSTEM_FONTABCD_W;
	int th = TEXPOS_SYSTEM_FONTABCD_H;
	struct engineLayout02ViewPartsImageText *this = engineLayout02ViewPartsImageTextCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
	this->hiragana.tu = TEXPOS_SYSTEM_FONTHIRA_X;
	this->hiragana.tv = TEXPOS_SYSTEM_FONTHIRA_Y;
	this->katakana.tu = TEXPOS_SYSTEM_FONTKATA_X;
	this->katakana.tv = TEXPOS_SYSTEM_FONTKATA_Y;
	engineLayout02ViewPartsImageTextSet(this, text);
	return this;
}

// 画像文字列描画構造体 文字列設定(utf8)
void engineLayout02ViewPartsImageTextSet(struct engineLayout02ViewPartsImageText *that, char *text){
	struct engineLayout02ViewPartsImageTextImplement *this = (struct engineLayout02ViewPartsImageTextImplement*)that;
	// 文字列領域作成
	int textLength = (text != NULL) ? (strlen(text) + 1) : 0;
	if(this->textInfo.length < textLength){
		if(this->textInfo.buff != NULL){engineUtilMemoryInfoFree("engineLayout02ViewPartsImageText textBuff", this->textInfo.buff);}
		this->textInfo.buff = (char*)engineUtilMemoryInfoMalloc("engineLayout02ViewPartsImageText textBuff", textLength * sizeof(char));
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

