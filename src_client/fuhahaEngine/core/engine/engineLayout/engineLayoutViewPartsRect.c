#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsRectImplement{
	struct engineLayoutViewPartsRect super;

	struct engineLayoutViewPartsRectBufferCompare{
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} texture;
	} bufferCompare;
	engineGraphicTextureId egoIdTexTest;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsRectImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsRectImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsRectImplement *this){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsRectImplement *this){
	struct engineLayoutViewPartsRectBufferCompare bufferCompare;
	bufferCompare.texture.imgw = this->super.texture.imgw;
	bufferCompare.texture.imgh = this->super.texture.imgh;
	bufferCompare.texture.tu = this->super.texture.tu;
	bufferCompare.texture.tv = this->super.texture.tv;
	bufferCompare.texture.tw = this->super.texture.tw;
	bufferCompare.texture.th = this->super.texture.th;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsRectBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsRectBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsRectImplement *this){
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
static void draw(struct engineLayoutViewPartsRectImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
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
	engineMathMat4Scale(&tempMat1, w, h, 1.0);
	engineGraphicEngineSetMatrix(&tempMat1);
	// 色登録
	engineGraphicEngineSetColorVec(color);

	// 描画
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsRectImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsRectImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsRect", this);
}

// ----------------------------------------------------------------

// 画像描画構造体 作成
struct engineLayoutViewPartsRect *engineLayoutViewPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayoutViewPartsRectImplement *this = (struct engineLayoutViewPartsRectImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsRect", 1, sizeof(struct engineLayoutViewPartsRectImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.texture.imgw = imgw;
	this->super.texture.imgh = imgh;
	this->super.texture.tu = tu;
	this->super.texture.tv = tv;
	this->super.texture.tw = tw;
	this->super.texture.th = th;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsRect*)this;
}

// 画像描画構造体 作成 白四角
struct engineLayoutViewPartsRect *engineLayoutViewPartsRectCreateWhite(){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tu = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25);
	int tv = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25);
	int tw = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_W * 0.5);
	int th = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_H * 0.5);
	return engineLayoutViewPartsRectCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

