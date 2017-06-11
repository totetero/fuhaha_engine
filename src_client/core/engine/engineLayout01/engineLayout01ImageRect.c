#include "../../library.h"
#include "../../define/texpos.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "../engineCtrl/engineCtrl.h"
#include "../engineGraphic/engineGraphic.h"
#include "engineLayout01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayout01Trans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineLayout01ImageRect *this = (struct engineLayout01ImageRect*)((char*)that - offsetof(struct engineLayout01ImageRect, trans));
	engineLayout01ImageRectDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayout01Trans *that){
	struct engineLayout01ImageRect *this = (struct engineLayout01ImageRect*)((char*)that - offsetof(struct engineLayout01ImageRect, trans));
	engineLayout01ImageRectDispose(this);
}

// ----------------------------------------------------------------

// 画像描画構造体 初期化
void engineLayout01ImageRectInit(struct engineLayout01ImageRect *this){
	engineLayout01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
}

// 配列に画像の描画情報を追加
void engineLayout01ImageRectCreateArray(struct engineLayout01ImageRect *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th){
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
void engineLayout01ImageRectCreateArrayWhite(struct engineLayout01ImageRect *this, double x, double y, double w, double h){
	double tu = TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25;
	double tv = TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25;
	double tw = TEXPOS_SYSTEM_BOXWHITE_W * 0.5;
	double th = TEXPOS_SYSTEM_BOXWHITE_H * 0.5;
	engineLayout01ImageRectCreateArray(this, TEXSIZ_SYSTEM_WH, x, y, w, h, tu, tv, tw, th);
}

// ----------------------------------------------------------------

// 画像描画構造体 描画
void engineLayout01ImageRectDraw(struct engineLayout01ImageRect *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	engineLayout01TransBindAll(&this->trans, mat, color);
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
}

// 画像描画構造体 破棄
void engineLayout01ImageRectDispose(struct engineLayout01ImageRect *this){
	this->faceIndex = 0;
	this->faceNum = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

