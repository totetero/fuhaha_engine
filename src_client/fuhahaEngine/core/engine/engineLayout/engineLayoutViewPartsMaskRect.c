#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsMaskRectImplement{
	struct engineLayoutViewPartsMaskRect super;

	struct engineLayoutViewPartsMaskRectBufferCompare{
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
static void init(struct engineLayoutViewPartsMaskRectImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	this->generationCount++;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsMaskRectImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsMaskRectImplement *this){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ配列作成
static void createBufferArrayRect(struct engineLayoutViewPartsMaskRectImplement *this){
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
static void createBuffer(struct engineLayoutViewPartsMaskRectImplement *this){
	struct engineLayoutViewPartsMaskRectBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsMaskRectBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsMaskRectBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		createBufferArrayRect(this);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct engineLayoutViewPartsMaskRectImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 描画準備
	createBuffer(this);

	// 行列作成
	double w = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
	double h = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
	struct engineMathMatrix44 tempMat1;
	engineLayoutViewUtilPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
	engineMathMat4Scale(&tempMat1, w, h, 1.0);

	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexTest);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	// 行列登録
	engineGraphicEngineSetMatrix(&tempMat1);

	// ステンシル描画 マスクをかける
	engineGraphicStencilStackMaskWriteIncrement();
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
	engineGraphicStencilStackMaskRead();

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);

	// バッファ登録
	engineGraphicEngineBindTexture(this->egoIdTexTest);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	// 行列登録
	engineGraphicEngineSetMatrix(&tempMat1);

	// ステンシル描画 マスクをはずす
	engineGraphicStencilStackMaskWriteDecrement();
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);
	engineGraphicStencilStackMaskRead();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsMaskRectImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsMaskRectImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsMaskRect", this);
}

// ----------------------------------------------------------------

// 長方形マスク構造体 作成
struct engineLayoutViewPartsMaskRect *engineLayoutViewPartsMaskRectCreate(){
	struct engineLayoutViewPartsMaskRectImplement *this = (struct engineLayoutViewPartsMaskRectImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsMaskRect", 1, sizeof(struct engineLayoutViewPartsMaskRectImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal("img/system.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	return (struct engineLayoutViewPartsMaskRect*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

