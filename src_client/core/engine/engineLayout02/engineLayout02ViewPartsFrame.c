#include "../../library.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "../engineGraphic/engineGraphic.h"
#include "./engineLayout02.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayout02ViewPartsFrameImplement{
	struct engineLayout02ViewPartsFrame super;

	struct engineLayout02ViewPartsFrameBufferCompare{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} texture;
		struct{int bt; int bb; int br; int bl;} border;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexTest;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02ViewPartsFrameImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);

	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal("img/system.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);

	// デフォルトパラメータ設定
	this->super.imgw = TEXSIZ_SYSTEM_W;
	this->super.imgh = TEXSIZ_SYSTEM_H;
	this->super.texture.tu = TEXPOS_SYSTEM_BOXBASIC_X;
	this->super.texture.tv = TEXPOS_SYSTEM_BOXBASIC_Y;
	this->super.texture.tw = TEXPOS_SYSTEM_BOXBASIC_W;
	this->super.texture.th = TEXPOS_SYSTEM_BOXBASIC_H;
	this->super.border.bt = 20;
	this->super.border.bb = 20;
	this->super.border.br = 20;
	this->super.border.bl = 20;
	this->super.scale = 0.5;
}

// ----------------------------------------------------------------

// 計算
static void calc(struct engineLayout02ViewPartsFrameImplement *this){
	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// バッファ配列作成
static void createBufferArrayRect(struct engineLayout02ViewPartsFrameImplement *this){
	// バッファポインタ取得
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 9;

	// 頂点テクスチャ座標データを生成
	double tu = this->super.texture.tu;
	double tv = this->super.texture.tv;
	double tw = this->super.texture.tw;
	double th = this->super.texture.th;
	double bt = this->super.border.bt;
	double bb = this->super.border.bb;
	double br = this->super.border.br;
	double bl = this->super.border.bl;
	double x1 = -1.5; double u1 = (tu                           ) / this->super.imgw;
	double x2 = -0.5; double u2 = (tu + bl                      ) / this->super.imgw;
	double x3 =  0.5; double u3 = (tu + bl + (tw - bl - br)     ) / this->super.imgw;
	double x4 =  1.5; double u4 = (tu + bl + (tw - bl - br) + br) / this->super.imgw;
	double y1 = -1.5; double v1 = (tv                           ) / this->super.imgh;
	double y2 = -0.5; double v2 = (tv + bt                      ) / this->super.imgh;
	double y3 =  0.5; double v3 = (tv + bt + (th - bt - bb)     ) / this->super.imgh;
	double y4 =  1.5; double v4 = (tv + bt + (th - bt - bb) + bb) / this->super.imgh;
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

	// バッファパラメータ記録
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// バッファ作成
static void createBuffer(struct engineLayout02ViewPartsFrameImplement *this){
	struct engineLayout02ViewPartsFrameBufferCompare bufferCompare;
	bufferCompare.imgw = this->super.imgw;
	bufferCompare.imgh = this->super.imgh;
	bufferCompare.texture.tu = this->super.texture.tu;
	bufferCompare.texture.tv = this->super.texture.tv;
	bufferCompare.texture.tw = this->super.texture.tw;
	bufferCompare.texture.th = this->super.texture.th;
	bufferCompare.border.bt = this->super.border.bt;
	bufferCompare.border.bb = this->super.border.bb;
	bufferCompare.border.br = this->super.border.br;
	bufferCompare.border.bl = this->super.border.bl;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsFrameBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsFrameBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		createBufferArrayRect(this);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct engineLayout02ViewPartsFrameImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 描画準備
	createBuffer(this);

	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);

	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexTest);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	// 行列登録
	double x = engineLayout02ViewUtilPositionGetX((struct engineLayout02View*)this);
	double y = engineLayout02ViewUtilPositionGetY((struct engineLayout02View*)this);
	double w = engineLayout02ViewUtilPositionGetW((struct engineLayout02View*)this);
	double h = engineLayout02ViewUtilPositionGetH((struct engineLayout02View*)this);
	struct engineMathMatrix44 tempMat1;
	struct engineMathMatrix44 tempMat2;
	engineMathMat4Copy(&tempMat1, mat);
	engineMathMat4Translate(&tempMat1, x + w * 0.5, y + h * 0.5, 0.0);
	// 色登録
	engineGraphicEngineSetColorVec(color);

	double bt = this->super.border.bt * this->super.scale;
	double bb = this->super.border.bb * this->super.scale;
	double br = this->super.border.br * this->super.scale;
	double bl = this->super.border.bl * this->super.scale;
	if(bt + 1 + bb > h){double ratio = (double)(h - 1) / (bt + bb); bt *= ratio; bb *= ratio;}
	if(br + 1 + bl > w){double ratio = (double)(w - 1) / (br + bl); br *= ratio; bl *= ratio;}
	double fw = w - br - bl;
	double fh = h - bt - bb;
	double ft = -(h * 0.5 - bt);
	double fb =  (h * 0.5 - bb);
	double fr =  (w * 0.5 - br);
	double fl = -(w * 0.5 - bl);

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

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsFrameImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsFrameImplement *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsFrame", this);
}

// ----------------------------------------------------------------

// 枠付描画構造体 作成
struct engineLayout02ViewPartsFrame *engineLayout02ViewPartsFrameCreate(){
	struct engineLayout02ViewPartsFrameImplement *this = (struct engineLayout02ViewPartsFrameImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsFrame", 1, sizeof(struct engineLayout02ViewPartsFrameImplement));
	init(this);

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsFrame*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

