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
struct engineLayout02ViewPartsMaskRectImplement{
	struct engineLayout02ViewPartsMaskRect super;

	struct engineLayout02ViewPartsMaskRectBufferCompare{
		int generationCount;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexTest;

	int generationCount;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02ViewPartsMaskRectImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);
	this->generationCount++;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsMaskRectImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsMaskRectImplement *this){
	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// バッファ配列作成
static void createBufferArrayRect(struct engineLayout02ViewPartsMaskRectImplement *this){
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

// バッファ作成
static void createBuffer(struct engineLayout02ViewPartsMaskRectImplement *this){
	struct engineLayout02ViewPartsMaskRectBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsMaskRectBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsMaskRectBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		createBufferArrayRect(this);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct engineLayout02ViewPartsMaskRectImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
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
	engineMathMat4Scale(&tempMat1, w, h, 1.0);
	engineGraphicEngineSetMatrix(&tempMat1);

	// ステンシル描画
	engineGraphicStencilStackMaskWriteIncrement();
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
	engineGraphicStencilStackMaskRead();

	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);

	// ステンシル描画
	engineGraphicStencilStackMaskWriteDecrement();
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
	engineGraphicStencilStackMaskRead();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsMaskRectImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsMaskRectImplement *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewDetouch((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsMaskRect", this);
}

// ----------------------------------------------------------------

// 長方形マスク構造体 作成
struct engineLayout02ViewPartsMaskRect *engineLayout02ViewPartsMaskRectCreate(){
	struct engineLayout02ViewPartsMaskRectImplement *this = (struct engineLayout02ViewPartsMaskRectImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsMaskRect", 1, sizeof(struct engineLayout02ViewPartsMaskRectImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal("img/system.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsMaskRect*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

