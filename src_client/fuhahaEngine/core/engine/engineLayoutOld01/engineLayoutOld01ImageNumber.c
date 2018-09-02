#include "../../fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutOld01Trans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineLayoutOld01ImageNumber *this = (struct engineLayoutOld01ImageNumber*)((char*)that - offsetof(struct engineLayoutOld01ImageNumber, trans));
	engineLayoutOld01ImageNumberDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayoutOld01Trans *that){
	struct engineLayoutOld01ImageNumber *this = (struct engineLayoutOld01ImageNumber*)((char*)that - offsetof(struct engineLayoutOld01ImageNumber, trans));
	engineLayoutOld01ImageNumberDispose(this);
}

// ----------------------------------------------------------------

// 数値列描画構造体 初期化
void engineLayoutOld01ImageNumberInit(struct engineLayoutOld01ImageNumber *this){
	engineLayoutOld01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	this->createArrayInfo.imgw = TEXSIZ_SYSTEM_W;
	this->createArrayInfo.imgh = TEXSIZ_SYSTEM_H;
	int tw = (TEXPOS_SYSTEM_FONTABCD_W / 16);
	int th = (TEXPOS_SYSTEM_FONTABCD_H / 6);
	this->createArrayInfo.number.tu = TEXPOS_SYSTEM_FONTABCD_X;
	this->createArrayInfo.number.tv = TEXPOS_SYSTEM_FONTABCD_Y + th * 1;
	this->createArrayInfo.number.tw = tw * 10;
	this->createArrayInfo.number.th = th * 1;
	this->createArrayInfo.scale = 0.5;
	this->createArrayInfo.xalign = 0;
	this->createArrayInfo.yalign = 0;
}

// 配列に数値列(utf8)の描画情報を追加
void engineLayoutOld01ImageNumberCreateArray(struct engineLayoutOld01ImageNumber *this, double x, double y, int number){
	if(this == NULL){return;}

	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 10;

	int tu = this->createArrayInfo.number.tu;
	int tv = this->createArrayInfo.number.tv;
	int tw = this->createArrayInfo.number.tw / tetraNum;
	int th = this->createArrayInfo.number.th;
	double w1 = tw * this->createArrayInfo.scale;
	double h1 = th * this->createArrayInfo.scale;
	double y1 = y - ((this->createArrayInfo.yalign > 0) ? 0.0 : (this->createArrayInfo.yalign == 0) ? 0.5 : 1.0) * h1;
	for(int i = 0; i < tetraNum; i++){
		int u1 = tu + tw * i;
		engineGraphicBufferPushTetraVert(x, y1, w1, h1);
		engineGraphicBufferPushTetraTexc(this->createArrayInfo.imgw, this->createArrayInfo.imgh, u1, tv, tw, th);
	}

	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
	this->width = w1;
	this->value = number;
}

// ----------------------------------------------------------------

// 数値列描画構造体 描画
void engineLayoutOld01ImageNumberDraw(struct engineLayoutOld01ImageNumber *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 桁数確認
	int length = 0;
	int tempValue = engineMathAbs(this->value);
	do{tempValue /= 10; length++;}while(tempValue >= 1);

	// バッファ登録
	engineLayoutOld01TransBindBuffer(&this->trans);
	// 色登録
	engineLayoutOld01TransBindColor(&this->trans, color);
	// 行列登録
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineLayoutOld01TransMultMatrix(&this->trans, &tempMat1);
	engineMathMat4Translate(&tempMat1, ((this->createArrayInfo.xalign > 0) ? 1.0 : (this->createArrayInfo.xalign == 0) ? 0.5 : 0.0) * length * this->width, 0, 0);

	// 下の桁から描画
	tempValue = engineMathAbs(this->value);
	do{
		engineMathMat4Translate(&tempMat1, -this->width, 0, 0);
		engineGraphicEngineSetMatrix(&tempMat1);
		engineGraphicEngineDrawIndex((this->faceIndex + (tempValue % 10) * 2) * 3, 2 * 3);
		tempValue /= 10;
	}while(tempValue >= 1);
}

// 数値列描画構造体 破棄
void engineLayoutOld01ImageNumberDispose(struct engineLayoutOld01ImageNumber *this){
	this->faceIndex = 0;
	this->faceNum = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

