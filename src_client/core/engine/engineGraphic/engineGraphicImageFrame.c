#include "../../library.h"
#include "../../define/texpos.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineGraphicTrans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineGraphicImageFrame *this = (struct engineGraphicImageFrame*)((char*)that - offsetof(struct engineGraphicImageFrame, trans));
	engineGraphicImageFrameDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineGraphicTrans *that){
	struct engineGraphicImageFrame *this = (struct engineGraphicImageFrame*)((char*)that - offsetof(struct engineGraphicImageFrame, trans));
	engineGraphicImageFrameDispose(this);
}

// ----------------------------------------------------------------

// 枠付描画構造体 初期化
void engineGraphicImageFrameInit(struct engineGraphicImageFrame *this){
	engineGraphicTransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	this->createArrayInfo.imgw = TEXSIZ_SYSTEM_W;
	this->createArrayInfo.imgh = TEXSIZ_SYSTEM_H;
	this->createArrayInfo.texture.tu = TEXPOS_SYSTEM_BOXBASIC_X;
	this->createArrayInfo.texture.tv = TEXPOS_SYSTEM_BOXBASIC_Y;
	this->createArrayInfo.texture.tw = TEXPOS_SYSTEM_BOXBASIC_W;
	this->createArrayInfo.texture.th = TEXPOS_SYSTEM_BOXBASIC_H;
	this->createArrayInfo.border.bt = 20;
	this->createArrayInfo.border.bb = 20;
	this->createArrayInfo.border.br = 20;
	this->createArrayInfo.border.bl = 20;
	this->createArrayInfo.scale = 0.5;
}

// 配列に枠付の描画情報を追加
void engineGraphicImageFrameCreateArray(struct engineGraphicImageFrame *this, double x, double y, double w, double h){
	if(this == NULL){return;}

	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 9;

	// 頂点テクスチャ座標データを生成
	double tu = this->createArrayInfo.texture.tu;
	double tv = this->createArrayInfo.texture.tv;
	double tw = this->createArrayInfo.texture.tw;
	double th = this->createArrayInfo.texture.th;
	double bt = this->createArrayInfo.border.bt;
	double bb = this->createArrayInfo.border.bb;
	double br = this->createArrayInfo.border.br;
	double bl = this->createArrayInfo.border.bl;
	double x1 = -1.5; double u1 = (tu                           ) / this->createArrayInfo.imgw;
	double x2 = -0.5; double u2 = (tu + bl                      ) / this->createArrayInfo.imgw;
	double x3 =  0.5; double u3 = (tu + bl + (tw - bl - br)     ) / this->createArrayInfo.imgw;
	double x4 =  1.5; double u4 = (tu + bl + (tw - bl - br) + br) / this->createArrayInfo.imgw;
	double y1 = -1.5; double v1 = (tv                           ) / this->createArrayInfo.imgh;
	double y2 = -0.5; double v2 = (tv + bt                      ) / this->createArrayInfo.imgh;
	double y3 =  0.5; double v3 = (tv + bt + (th - bt - bb)     ) / this->createArrayInfo.imgh;
	double y4 =  1.5; double v4 = (tv + bt + (th - bt - bb) + bb) / this->createArrayInfo.imgh;
	engineGraphicBufferPushVert(x1, y1, 0.0); engineGraphicBufferPushVert(x2, y1, 0.0); engineGraphicBufferPushVert(x3, y1, 0.0); engineGraphicBufferPushVert(x4, y1, 0.0);
	engineGraphicBufferPushVert(x1, y2, 0.0); engineGraphicBufferPushVert(x2, y2, 0.0); engineGraphicBufferPushVert(x3, y2, 0.0); engineGraphicBufferPushVert(x4, y2, 0.0);
	engineGraphicBufferPushVert(x1, y3, 0.0); engineGraphicBufferPushVert(x2, y3, 0.0); engineGraphicBufferPushVert(x3, y3, 0.0); engineGraphicBufferPushVert(x4, y3, 0.0);
	engineGraphicBufferPushVert(x1, y4, 0.0); engineGraphicBufferPushVert(x2, y4, 0.0); engineGraphicBufferPushVert(x3, y4, 0.0); engineGraphicBufferPushVert(x4, y4, 0.0);
	engineGraphicBufferPushTexc(u1, v1); engineGraphicBufferPushTexc(u2, v1); engineGraphicBufferPushTexc(u3, v1); engineGraphicBufferPushTexc(u4, v1);
	engineGraphicBufferPushTexc(u1, v2); engineGraphicBufferPushTexc(u2, v2); engineGraphicBufferPushTexc(u3, v2); engineGraphicBufferPushTexc(u4, v2);
	engineGraphicBufferPushTexc(u1, v3); engineGraphicBufferPushTexc(u2, v3); engineGraphicBufferPushTexc(u3, v3); engineGraphicBufferPushTexc(u4, v3);
	engineGraphicBufferPushTexc(u1, v4); engineGraphicBufferPushTexc(u2, v4); engineGraphicBufferPushTexc(u3, v4); engineGraphicBufferPushTexc(u4, v4);
	// インデックスデータを作成
	engineGraphicBufferPushFace(vertIndex,  0,  1,  5); engineGraphicBufferPushFace(vertIndex,  5,  4,  0);
	engineGraphicBufferPushFace(vertIndex,  1,  2,  6); engineGraphicBufferPushFace(vertIndex,  6,  5,  1);
	engineGraphicBufferPushFace(vertIndex,  2,  3,  7); engineGraphicBufferPushFace(vertIndex,  7,  6,  2);
	engineGraphicBufferPushFace(vertIndex,  4,  5,  9); engineGraphicBufferPushFace(vertIndex,  9,  8,  4);
	engineGraphicBufferPushFace(vertIndex,  5,  6, 10); engineGraphicBufferPushFace(vertIndex, 10,  9,  5);
	engineGraphicBufferPushFace(vertIndex,  6,  7, 11); engineGraphicBufferPushFace(vertIndex, 11, 10,  6);
	engineGraphicBufferPushFace(vertIndex,  8,  9, 13); engineGraphicBufferPushFace(vertIndex, 13, 12,  8);
	engineGraphicBufferPushFace(vertIndex,  9, 10, 14); engineGraphicBufferPushFace(vertIndex, 14, 13,  9);
	engineGraphicBufferPushFace(vertIndex, 10, 11, 15); engineGraphicBufferPushFace(vertIndex, 15, 14, 10);

	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

// ----------------------------------------------------------------

// 枠付描画構造体 描画
void engineGraphicImageFrameDraw(struct engineGraphicImageFrame *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// バッファ登録
	engineGraphicTransBindBuffer(&this->trans);
	// 色登録
	engineGraphicTransBindColor(&this->trans, color);
	// 行列登録
	struct engineMathMatrix44 tempMat1;
	struct engineMathMatrix44 tempMat2;
	engineMathMat4Copy(&tempMat1, mat);
	engineGraphicTransMultMatrix(&this->trans, &tempMat1);
	engineMathMat4Translate(&tempMat1, this->x + this->w * 0.5, this->y + this->h * 0.5, 0.0);

	double bt = this->createArrayInfo.border.bt * this->createArrayInfo.scale;
	double bb = this->createArrayInfo.border.bb * this->createArrayInfo.scale;
	double br = this->createArrayInfo.border.br * this->createArrayInfo.scale;
	double bl = this->createArrayInfo.border.bl * this->createArrayInfo.scale;
	if(bt + 1 + bb > this->h){double ratio = (double)(this->h - 1) / (bt + bb); bt *= ratio; bb *= ratio;}
	if(br + 1 + bl > this->w){double ratio = (double)(this->w - 1) / (br + bl); br *= ratio; bl *= ratio;}
	double fw = this->w - br - bl;
	double fh = this->h - bt - bb;
	double ft = -(this->h * 0.5 - bt);
	double fb =  (this->h * 0.5 - bb);
	double fr =  (this->w * 0.5 - br);
	double fl = -(this->w * 0.5 - bl);

	// 左上
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, fl, ft, 0.0);
	engineMathMat4Scale(&tempMat2, bl, bt, 1.0);
	engineMathMat4Translate(&tempMat2, 0.5, 0.5, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 0) * 3, 2 * 3);
	// 中上
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, (bl - br) * 0.5, ft, 0.0);
	engineMathMat4Scale(&tempMat2, fw, bt, 1.0);
	engineMathMat4Translate(&tempMat2, 0.0, 0.5, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 2) * 3, 2 * 3);
	// 右上
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, fr, ft, 0.0);
	engineMathMat4Scale(&tempMat2, br, bt, 1.0);
	engineMathMat4Translate(&tempMat2, -0.5, 0.5, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 4) * 3, 2 * 3);
	// 左中
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, fl, (bt - bb) * 0.5, 0.0);
	engineMathMat4Scale(&tempMat2, bl, fh, 1.0);
	engineMathMat4Translate(&tempMat2, 0.5, 0.0, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 6) * 3, 2 * 3);
	// 中中
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, (bl - br) * 0.5, (bt - bb) * 0.5, 0.0);
	engineMathMat4Scale(&tempMat2, fw, fh, 1.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 8) * 3, 2 * 3);
	// 右中
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, fr, (bt - bb) * 0.5, 0.0);
	engineMathMat4Scale(&tempMat2, br, fh, 1.0);
	engineMathMat4Translate(&tempMat2, -0.5, 0.0, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 10) * 3, 2 * 3);
	// 左下
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, fl, fb, 0.0);
	engineMathMat4Scale(&tempMat2, bl, bb, 1.0);
	engineMathMat4Translate(&tempMat2, 0.5, -0.5, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 12) * 3, 2 * 3);
	// 中下
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, (bl - br) * 0.5, fb, 0.0);
	engineMathMat4Scale(&tempMat2, fw, bb, 1.0);
	engineMathMat4Translate(&tempMat2, 0.0, -0.5, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 14) * 3, 2 * 3);
	// 右下
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, fr, fb, 0.0);
	engineMathMat4Scale(&tempMat2, br, bb, 1.0);
	engineMathMat4Translate(&tempMat2, -0.5, -0.5, 0.0);
	engineGraphicEngineSetMatrix(&tempMat2);
	engineGraphicEngineDrawIndex((this->faceIndex + 16) * 3, 2 * 3);
}

// 枠付描画構造体 破棄
void engineGraphicImageFrameDispose(struct engineGraphicImageFrame *this){
	this->faceIndex = 0;
	this->faceNum = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

