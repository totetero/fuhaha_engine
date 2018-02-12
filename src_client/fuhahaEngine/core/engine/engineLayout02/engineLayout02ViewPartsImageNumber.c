#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout02/engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayout02ViewPartsImageNumberImplement{
	struct engineLayout02ViewPartsImageNumber super;

	struct engineLayout02ViewPartsImageNumberBufferCompare{
		struct{int imgw; int imgh; int tu; int tv; int tw; int th;} number;
		struct{
			int xalign;
			int yalign;
		} fontStyle;
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
static void init(struct engineLayout02ViewPartsImageNumberImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilFamilyInit((struct engineLayout02View*)this);
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);

	// デフォルトパラメータ設定
	this->super.fontStyle.scale = 0.5;
	this->super.color.r = 1.0;
	this->super.color.g = 1.0;
	this->super.color.b = 1.0;
	this->super.color.a = 1.0;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsImageNumberImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive) || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsImageNumberImplement *this){
	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// バッファ配列作成
static void createBufferArrayText(struct engineLayout02ViewPartsImageNumberImplement *this){
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

// バッファ作成
static void createBuffer(struct engineLayout02ViewPartsImageNumberImplement *this){
	struct engineLayout02ViewPartsImageNumberBufferCompare bufferCompare;
	bufferCompare.number.imgw = this->super.number.imgw;
	bufferCompare.number.imgh = this->super.number.imgh;
	bufferCompare.number.tu = this->super.number.tu;
	bufferCompare.number.tv = this->super.number.tv;
	bufferCompare.number.tw = this->super.number.tw;
	bufferCompare.number.th = this->super.number.th;
	bufferCompare.fontStyle.xalign = this->super.fontStyle.xalign;
	bufferCompare.fontStyle.yalign = this->super.fontStyle.yalign;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsImageNumberBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayout02ViewPartsImageNumberBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		createBufferArrayText(this);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct engineLayout02ViewPartsImageNumberImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
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
	double x0 = w * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0);
	double y0 = h * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0);
	engineMathMat4Translate(&tempMat1, x0, y0, 0);
	engineMathMat4Scale(&tempMat1, this->super.fontStyle.scale, this->super.fontStyle.scale, 1.0);
	// 色登録
	struct engineMathVector4 tempColor1;
	tempColor1.r = this->super.color.r * color->r;
	tempColor1.g = this->super.color.g * color->g;
	tempColor1.b = this->super.color.b * color->b;
	tempColor1.a = this->super.color.a * color->a;
	engineGraphicEngineSetColorVec(&tempColor1);

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
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsImageNumberImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsImageNumberImplement *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewUtilFamilyDispose((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsImageNumber", this);
}

// ----------------------------------------------------------------

// 画像数字描画構造体 作成
struct engineLayout02ViewPartsImageNumber *engineLayout02ViewPartsImageNumberCreate(char *src, int imgw, int imgh, int tu, int tv, int tw, int th){
	struct engineLayout02ViewPartsImageNumberImplement *this = (struct engineLayout02ViewPartsImageNumberImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsImageNumber", 1, sizeof(struct engineLayout02ViewPartsImageNumberImplement));
	init(this);
	// 画像読み込み
	this->egoIdTexTest = engineGraphicTextureCreateLocal(src, ENGINEGRAPHICTEXTURETYPE_LINEAR);
	this->super.number.imgw = imgw;
	this->super.number.imgh = imgh;
	this->super.number.tu = tu;
	this->super.number.tv = tv;
	this->super.number.tw = tw;
	this->super.number.th = th;

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsImageNumber*)this;
}

// 画像数字描画構造体 作成 デフォルト
struct engineLayout02ViewPartsImageNumber *engineLayout02ViewPartsImageNumberCreateDefault(int value){
	int imgw = TEXSIZ_SYSTEM_W;
	int imgh = TEXSIZ_SYSTEM_H;
	int tw0 = (TEXPOS_SYSTEM_FONTABCD_W / 16);
	int th0 = (TEXPOS_SYSTEM_FONTABCD_H / 6);
	int tu = TEXPOS_SYSTEM_FONTABCD_X + tw0 * 0;
	int tv = TEXPOS_SYSTEM_FONTABCD_Y + th0 * 1;
	int tw = tw0 * 10;
	int th = th0 * 1;
	struct engineLayout02ViewPartsImageNumber *this = engineLayout02ViewPartsImageNumberCreate("img/system.png", imgw, imgh, tu, tv, tw, th);
	this->value = value;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

