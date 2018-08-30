#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsImageNumberImplement{
	struct engineLayoutPartsImageNumber super;

	struct engineLayoutPartsImageNumberBufferCompare{
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} number;
		struct{
			int xalign;
			int yalign;
		} fontStyle;
	} bufferCompare;
	engineGraphicTextureId egoIdTexImage;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsImageNumberImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// デフォルトパラメータ設定
	this->super.fontStyle.scale = 0.5;
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutPartsImageNumberImplement *this){
	struct engineLayoutPartsImageNumberBufferCompare bufferCompare;
	bufferCompare.number.imgw = this->super.number.imgw;
	bufferCompare.number.imgh = this->super.number.imgh;
	bufferCompare.number.tu = this->super.number.tu;
	bufferCompare.number.tv = this->super.number.tv;
	bufferCompare.number.tw = this->super.number.tw;
	bufferCompare.number.th = this->super.number.th;
	bufferCompare.fontStyle.xalign = this->super.fontStyle.xalign;
	bufferCompare.fontStyle.yalign = this->super.fontStyle.yalign;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsImageNumberBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsImageNumberBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ作成
static void bufferCreate(struct engineLayoutPartsImageNumberImplement *this){
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
static void draw(struct engineLayoutPartsImageNumberImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
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
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct engineLayoutPartsImageNumberImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexImage);
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutPartsImageNumber", this);
}

// ----------------------------------------------------------------

// 画像数字描画構造体 作成
struct engineLayoutPartsImageNumber *engineLayoutPartsImageNumberCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayoutPartsImageNumberImplement *this = (struct engineLayoutPartsImageNumberImplement*)engineUtilMemoryInfoCalloc("engineLayoutPartsImageNumber", 1, sizeof(struct engineLayoutPartsImageNumberImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexImage = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.number.imgw = imgw;
	this->super.number.imgh = imgh;
	this->super.number.tu = tu;
	this->super.number.tv = tv;
	this->super.number.tw = tw;
	this->super.number.th = th;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutPartsImageNumber*)this;
}

// 画像数字描画構造体 作成 デフォルト
struct engineLayoutPartsImageNumber *engineLayoutPartsImageNumberCreateDefault(int value){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tw0 = (TEXPOS_SYSTEM_FONTABCD_W / 16);
	int th0 = (TEXPOS_SYSTEM_FONTABCD_H / 6);
	int tu = TEXPOS_SYSTEM_FONTABCD_X + tw0 * 0;
	int tv = TEXPOS_SYSTEM_FONTABCD_Y + th0 * 1;
	int tw = tw0 * 10;
	int th = th0 * 1;
	struct engineLayoutPartsImageNumber *this = engineLayoutPartsImageNumberCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
	this->value = value;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

