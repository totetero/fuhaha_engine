#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsImageNumberImplement{
	struct engineLayoutViewPartsImageNumber super;

	struct engineLayoutViewPartsImageNumberBufferCompare{
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} number;
		struct{
			int xalign;
			int yalign;
		} fontStyle;
	} bufferCompare;
	engineGraphicTextureId egoIdTexTest;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsImageNumberImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// デフォルトパラメータ設定
	this->super.fontStyle.scale = 0.5;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsImageNumberImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsImageNumberImplement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsImageNumberImplement *this){
	struct engineLayoutViewPartsImageNumberBufferCompare bufferCompare;
	bufferCompare.number.imgw = this->super.number.imgw;
	bufferCompare.number.imgh = this->super.number.imgh;
	bufferCompare.number.tu = this->super.number.tu;
	bufferCompare.number.tv = this->super.number.tv;
	bufferCompare.number.tw = this->super.number.tw;
	bufferCompare.number.th = this->super.number.th;
	bufferCompare.fontStyle.xalign = this->super.fontStyle.xalign;
	bufferCompare.fontStyle.yalign = this->super.fontStyle.yalign;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsImageNumberBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsImageNumberBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsImageNumberImplement *this){
	// バッファポインタ取得
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 10;

	// 頂点座標データとテクスチャ座標データを生成
	int tu = this->super.number.tu;
	int tv = this->super.number.tv;
	int tw = this->super.number.tw / tetraNum;
	int th = this->super.number.th;
	double x1 = (-1 * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0)) * tw;
	double y1 = (-1 * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0)) * th;
	for(int i = 0; i < tetraNum; i++){
		int u1 = tu + tw * i;
		engineGraphicBufferPushTetraVert(x1, y1, tw, th);
		engineGraphicBufferPushTetraTexc(this->super.number.imgw, this->super.number.imgh, u1, tv, tw, th);
	}

	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	// バッファパラメータ記録
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// 描画
static void draw(struct engineLayoutViewPartsImageNumberImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
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
	double x0 = w * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0);
	double y0 = h * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0);
	engineMathMat4Translate(&tempMat1, x0, y0, 0);
	engineMathMat4Scale(&tempMat1, this->super.fontStyle.scale, this->super.fontStyle.scale, 1.0);
	// 色登録
	engineGraphicEngineSetColorVec(color);

	// 桁数確認
	int length = 0;
	int tempValue = engineMathAbs(this->super.value);
	do{length++; tempValue /= 10;}while(tempValue >= 1);
	int tw = this->super.number.tw / 10;
	// 下の桁から描画
	double x1 = (tw * (length - 1)) * ((this->super.fontStyle.xalign > 0) ? 1.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 0.0);
	engineMathMat4Translate(&tempMat1, x1 + tw, 0, 0);
	tempValue = engineMathAbs(this->super.value);
	do{
		engineMathMat4Translate(&tempMat1, -tw, 0, 0);
		engineGraphicEngineSetMatrix(&tempMat1);
		engineGraphicEngineDrawIndex((this->faceIndex + (tempValue % 10) * 2) * 3, 2 * 3);
		tempValue /= 10;
	}while(tempValue >= 1);

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsImageNumberImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsImageNumberImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsImageNumber", this);
}

// ----------------------------------------------------------------

// 画像数字描画構造体 作成
struct engineLayoutViewPartsImageNumber *engineLayoutViewPartsImageNumberCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayoutViewPartsImageNumberImplement *this = (struct engineLayoutViewPartsImageNumberImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsImageNumber", 1, sizeof(struct engineLayoutViewPartsImageNumberImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.number.imgw = imgw;
	this->super.number.imgh = imgh;
	this->super.number.tu = tu;
	this->super.number.tv = tv;
	this->super.number.tw = tw;
	this->super.number.th = th;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsImageNumber*)this;
}

// 画像数字描画構造体 作成 デフォルト
struct engineLayoutViewPartsImageNumber *engineLayoutViewPartsImageNumberCreateDefault(int value){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tw0 = (TEXPOS_SYSTEM_FONTABCD_W / 16);
	int th0 = (TEXPOS_SYSTEM_FONTABCD_H / 6);
	int tu = TEXPOS_SYSTEM_FONTABCD_X + tw0 * 0;
	int tv = TEXPOS_SYSTEM_FONTABCD_Y + th0 * 1;
	int tw = tw0 * 10;
	int th = th0 * 1;
	struct engineLayoutViewPartsImageNumber *this = engineLayoutViewPartsImageNumberCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
	this->value = value;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

