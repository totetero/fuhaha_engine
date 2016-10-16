#include "../../library.h"
#include "../../texpos.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineGraphicTrans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineGraphicImageRect *this = (struct engineGraphicImageRect*)((char*)that - offsetof(struct engineGraphicImageRect, trans));
	engineGraphicImageRectDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineGraphicTrans *that){
	struct engineGraphicImageRect *this = (struct engineGraphicImageRect*)((char*)that - offsetof(struct engineGraphicImageRect, trans));
	engineGraphicImageRectDispose(this);
}

// ----------------------------------------------------------------

// 画像描画構造体 初期化
void engineGraphicImageRectInit(struct engineGraphicImageRect *this){
	engineGraphicTransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
}

// 配列に画像の描画情報を追加
void engineGraphicImageRectCreateArray(struct engineGraphicImageRect *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th){
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 1;

	// 頂点座標データを生成
	engineGraphicBufferPushTetraVert(x, y, w, h);
	// テクスチャ座標データを生成
	engineGraphicBufferPushTetraTexc(imgw, imgh, tu, tv, tw, th);
	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	if(this == NULL){return;}
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// 配列に画像の描画情報を追加
void engineGraphicImageRectCreateArrayWhite(struct engineGraphicImageRect *this, double x, double y, double w, double h){
	double tu = TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25;
	double tv = TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25;
	double tw = TEXPOS_SYSTEM_BOXWHITE_W * 0.5;
	double th = TEXPOS_SYSTEM_BOXWHITE_H * 0.5;
	engineGraphicImageRectCreateArray(this, TEXSIZ_SYSTEM_WH, x, y, w, h, tu, tv, tw, th);
}

// ----------------------------------------------------------------

// 画像描画構造体 描画
void engineGraphicImageRectDraw(struct engineGraphicImageRect *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	engineGraphicTransBindAll(&this->trans, mat, color);
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// 画像描画構造体 破棄
void engineGraphicImageRectDispose(struct engineGraphicImageRect *this){
	this->faceIndex = 0;
	this->faceNum = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

