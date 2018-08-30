#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsRectImplement{
	struct engineLayoutPartsRect super;

	struct engineLayoutPartsRectBufferCompare{
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	} bufferCompare;
	engineGraphicTextureId egoIdTexImage;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsRectImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutPartsRectImplement *this){
	struct engineLayoutPartsRectBufferCompare bufferCompare;
	bufferCompare.texture.imgw = this->super.texture.imgw;
	bufferCompare.texture.imgh = this->super.texture.imgh;
	bufferCompare.texture.tu = this->super.texture.tu;
	bufferCompare.texture.tv = this->super.texture.tv;
	bufferCompare.texture.tw = this->super.texture.tw;
	bufferCompare.texture.th = this->super.texture.th;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsRectBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsRectBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ作成
static void bufferCreate(struct engineLayoutPartsRectImplement *this){
	// バッファポインタ取得
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 1;

	// 頂点座標データを生成
	int x = 0;
	int y = 0;
	int w = 1;
	int h = 1;
	engineGraphicBufferPushTetraVert(x, y, w, h);
	// テクスチャ座標データを生成
	int imgw = this->super.texture.imgw;
	int imgh = this->super.texture.imgh;
	int tu = this->super.texture.tu;
	int tv = this->super.texture.tv;
	int tw = this->super.texture.tw;
	int th = this->super.texture.th;
	engineGraphicBufferPushTetraTexc(imgw, imgh, tu, tv, tw, th);
	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	// バッファパラメータ記録
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// 描画
static void draw(struct engineLayoutPartsRectImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexImage);
	engineGraphicEngineBindVertVBO(this->super.super.graphicObject.egoIdVert);
	engineGraphicEngineBindTexcVBO(this->super.super.graphicObject.egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->super.super.graphicObject.egoIdFace);
	// 行列登録
	double w = engineLayoutViewGearPositionGetW((struct engineLayoutView*)this);
	double h = engineLayoutViewGearPositionGetH((struct engineLayoutView*)this);
	struct engineMathMatrix44 tempMat1;
	engineLayoutViewGearPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
	engineMathMat4Scale(&tempMat1, w, h, 1.0);
	engineGraphicEngineSetMatrix(&tempMat1);
	// 色登録
	engineGraphicEngineSetColorVec(color);

	// 描画
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);

	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct engineLayoutPartsRectImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexImage);
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutPartsRect", this);
}

// ----------------------------------------------------------------

// 画像描画構造体 作成
struct engineLayoutPartsRect *engineLayoutPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayoutPartsRectImplement *this = (struct engineLayoutPartsRectImplement*)engineUtilMemoryInfoCalloc("engineLayoutPartsRect", 1, sizeof(struct engineLayoutPartsRectImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexImage = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.texture.imgw = imgw;
	this->super.texture.imgh = imgh;
	this->super.texture.tu = tu;
	this->super.texture.tv = tv;
	this->super.texture.tw = tw;
	this->super.texture.th = th;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutPartsRect*)this;
}

// 画像描画構造体 作成 白四角
struct engineLayoutPartsRect *engineLayoutPartsRectCreateWhite(){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tu = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25);
	int tv = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25);
	int tw = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_W * 0.5);
	int th = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_H * 0.5);
	return engineLayoutPartsRectCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

