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
struct engineLayout02ViewPartsRectImplement{
	struct engineLayout02ViewPartsRect super;

	struct engineLayout02ViewPartsRectBufferCompare{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} texture;
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
static void init(struct engineLayout02ViewPartsRectImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);

	// デフォルトパラメータ設定
	this->super.color.r = 1.0;
	this->super.color.g = 1.0;
	this->super.color.b = 1.0;
	this->super.color.a = 1.0;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsRectImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsRectImplement *this){
	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// バッファ配列作成
static void createBufferArrayRect(struct engineLayout02ViewPartsRectImplement *this){
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
	int imgw = this->super.imgw;
	int imgh = this->super.imgh;
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

// バッファ作成
static void createBuffer(struct engineLayout02ViewPartsRectImplement *this){
	struct engineLayout02ViewPartsRectBufferCompare bufferCompare;
	bufferCompare.imgw = this->super.imgw;
	bufferCompare.imgh = this->super.imgh;
	bufferCompare.texture.tu = this->super.texture.tu;
	bufferCompare.texture.tv = this->super.texture.tv;
	bufferCompare.texture.tw = this->super.texture.tw;
	bufferCompare.texture.th = this->super.texture.th;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsRectBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsRectBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		createBufferArrayRect(this);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct engineLayout02ViewPartsRectImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
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
	// 色登録
	struct engineMathVector4 tempColor1;
	tempColor1.r = this->super.color.r * color->r;
	tempColor1.g = this->super.color.g * color->g;
	tempColor1.b = this->super.color.b * color->b;
	tempColor1.a = this->super.color.a * color->a;
	engineGraphicEngineSetColorVec(&tempColor1);

	// 描画
	engineGraphicEngineDrawIndex(this->faceIndex * 3, this->faceNum * 3);

	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsRectImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsRectImplement *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewDetouch((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsRect", this);
}

// ----------------------------------------------------------------

// 画像描画構造体 作成
struct engineLayout02ViewPartsRect *engineLayout02ViewPartsRectCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayout02ViewPartsRectImplement *this = (struct engineLayout02ViewPartsRectImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsRect", 1, sizeof(struct engineLayout02ViewPartsRectImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.imgw = imgw;
	this->super.imgh = imgh;
	this->super.texture.tu = tu;
	this->super.texture.tv = tv;
	this->super.texture.tw = tw;
	this->super.texture.th = th;

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsRect*)this;
}

// 画像描画構造体 作成 白四角
struct engineLayout02ViewPartsRect *engineLayout02ViewPartsRectCreateWhite(){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tu = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_X + TEXPOS_SYSTEM_BOXWHITE_W * 0.25);
	int tv = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_Y + TEXPOS_SYSTEM_BOXWHITE_H * 0.25);
	int tw = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_W * 0.5);
	int th = (int)engineMathRound(TEXPOS_SYSTEM_BOXWHITE_H * 0.5);
	return engineLayout02ViewPartsRectCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

