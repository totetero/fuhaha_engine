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
	struct engineLayoutOld01ImageRect *this = (struct engineLayoutOld01ImageRect*)((char*)that - offsetof(struct engineLayoutOld01ImageRect, trans));
	engineLayoutOld01ImageRectDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayoutOld01Trans *that){
	struct engineLayoutOld01ImageRect *this = (struct engineLayoutOld01ImageRect*)((char*)that - offsetof(struct engineLayoutOld01ImageRect, trans));
	engineLayoutOld01ImageRectDispose(this);
}

// ----------------------------------------------------------------

// 画像描画構造体 初期化
void engineLayoutOld01ImageRectInit(struct engineLayoutOld01ImageRect *this){
	engineLayoutOld01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
}

// 配列に画像の描画情報を追加
void engineLayoutOld01ImageRectCreateArray(struct engineLayoutOld01ImageRect *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th){
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
void engineLayoutOld01ImageRectCreateArrayWhite(struct engineLayoutOld01ImageRect *this, double x, double y, double w, double h){
	double tu = TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25;
	double tv = TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25;
	double tw = TEXPOS_SYSTEM_BOXWHITE_W * 0.5;
	double th = TEXPOS_SYSTEM_BOXWHITE_H * 0.5;
	engineLayoutOld01ImageRectCreateArray(this, TEXSIZ_SYSTEM_WH, x, y, w, h, tu, tv, tw, th);
}

// ----------------------------------------------------------------

// 画像描画構造体 描画
void engineLayoutOld01ImageRectDraw(struct engineLayoutOld01ImageRect *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	engineLayoutOld01TransBindAll(&this->trans, mat, color);
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// 画像描画構造体 破棄
void engineLayoutOld01ImageRectDispose(struct engineLayoutOld01ImageRect *this){
	this->faceIndex = 0;
	this->faceNum = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

