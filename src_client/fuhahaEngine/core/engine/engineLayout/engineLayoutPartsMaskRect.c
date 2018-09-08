#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsMaskRectImplement{
	struct engineLayoutPartsMaskRect super;

	struct engineLayoutPartsMaskRectBufferCompare{
		int generationCount;
	} bufferCompare;
	engineGraphicTextureId egoIdTexSystem;

	int generationCount;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsMaskRectImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	this->generationCount++;
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutPartsMaskRectImplement *this){
	struct engineLayoutPartsMaskRectBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsMaskRectBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsMaskRectBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ作成
static void bufferCreate(struct engineLayoutPartsMaskRectImplement *this){
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
	int imgw = 1;
	int imgh = 1;
	int tu = 0;
	int tv = 0;
	int tw = 1;
	int th = 1;
	engineGraphicBufferPushTetraTexc(imgw, imgh, tu, tv, tw, th);
	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	// バッファパラメータ記録
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// 描画
static void draw(struct engineLayoutPartsMaskRectImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 行列作成
	double w = engineLayoutViewGearPositionGetW((struct engineLayoutView*)this);
	double h = engineLayoutViewGearPositionGetH((struct engineLayoutView*)this);
	struct engineMathMatrix44 tempMat1;
	engineLayoutViewGearPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
	engineMathMat4Scale(&tempMat1, w, h, 1.0);

	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexSystem);
	engineGraphicEngineBindVertVBO(this->super.super.graphicObject.egoIdVert);
	engineGraphicEngineBindTexcVBO(this->super.super.graphicObject.egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->super.super.graphicObject.egoIdFace);
	// 行列登録
	engineGraphicEngineSetMatrix(&tempMat1);

	// ステンシル描画 マスクをかける
	engineGraphicStencilStackMaskWriteIncrement();
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
	engineGraphicStencilStackMaskRead();

	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);

	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexSystem);
	engineGraphicEngineBindVertVBO(this->super.super.graphicObject.egoIdVert);
	engineGraphicEngineBindTexcVBO(this->super.super.graphicObject.egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->super.super.graphicObject.egoIdFace);
	// 行列登録
	engineGraphicEngineSetMatrix(&tempMat1);

	// ステンシル描画 マスクをはずす
	engineGraphicStencilStackMaskWriteDecrement();
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
	engineGraphicStencilStackMaskRead();
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct engineLayoutPartsMaskRectImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexSystem);
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// 長方形マスク構造体 作成
struct engineLayoutPartsMaskRect *engineLayoutPartsMaskRectCreate(){
	struct engineLayoutPartsMaskRectImplement *this = (struct engineLayoutPartsMaskRectImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineLayoutPartsMaskRectImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexSystem = engineGraphicTextureCreateLocal("img/system.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutPartsMaskRect*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

