#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPopupActiveDisplayImplement{
	struct pageTest2PartsPopupActiveDisplay super;
	struct pageTest2Status *stat;

	struct enginePrimitiveSphere primitiveSphere;
	struct enginePrimitiveTorus primitiveTorus;
	struct enginePrimitiveWater primitiveWater;

	struct pageTest2PartsPopupActiveBufferCompare{
		int generationCount;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdNorm;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexSystem;

	double satelliteX;
	double satelliteY;
	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPopupActiveDisplayImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// 画像読み込み
	this->egoIdTexSystem = engineGraphicTextureCreateLocal("img/system.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);
	// 基本図形初期化
	enginePrimitiveSphereInit(&this->primitiveSphere, 1.0);
	enginePrimitiveTorusInit(&this->primitiveTorus, 3.0, 1.0);
	enginePrimitiveWaterInit(&this->primitiveWater, 1.2, 0.0, 0.4, 1.0);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2PartsPopupActiveDisplayImplement *this, bool isCancel){
	if(this->super.viewArrowKey != NULL){
		// 十字キーで移動
		if(this->super.viewArrowKey->isUp){this->satelliteY -= 0.1;}
		if(this->super.viewArrowKey->isDn){this->satelliteY += 0.1;}
		if(this->super.viewArrowKey->isRt){this->satelliteX += 0.1;}
		if(this->super.viewArrowKey->isLt){this->satelliteX -= 0.1;}
		if(this->satelliteX >  8){this->satelliteX =  8;}
		if(this->satelliteX < -8){this->satelliteX = -8;}
		if(this->satelliteY >  8){this->satelliteY =  8;}
		if(this->satelliteY < -8){this->satelliteY = -8;}
	}

	this->step++;

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// バッファ作成 必要なバッファが通常と違うために関数を分ける
static void bufferCreatePhong(struct pageTest2PartsPopupActiveDisplayImplement *this){
	struct pageTest2PartsPopupActiveBufferCompare bufferCompare;
	bufferCompare.generationCount = 1;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct pageTest2PartsPopupActiveBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct pageTest2PartsPopupActiveBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// 基本図形作成
		enginePrimitiveSphereCreateArray(&this->primitiveSphere);
		enginePrimitiveTorusCreateArray(&this->primitiveTorus);
		enginePrimitiveWaterCreateArray(&this->primitiveWater);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, &this->egoIdNorm, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 描画
static void draw(struct pageTest2PartsPopupActiveDisplayImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 描画準備
	bufferCreatePhong(this);

	// 行列作成
	struct engineMathMatrix44 matrixProjection;
	struct engineMathMatrix44 matrixModelViewWorld;
	struct engineMathMatrix44 tempMat1;
	struct engineMathMatrix44 tempMat2;
	// 透視投影からサイズ調整して平行投影を行い、3D物体を2Dの任意の位置に投影する
	double w = engineLayoutViewGearPositionGetW((struct engineLayoutView*)this);
	double h = engineLayoutViewGearPositionGetH((struct engineLayoutView*)this);
	engineLayoutViewGearPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
	engineMathMat4Scale(&tempMat1, w, -h, -1.0);
	engineMathMat4Scale(&tempMat1, 0.5, 0.5, 1.0);
	engineMathMat4Translate(&tempMat1, 1, -1, 0);
	engineMathMat4Perspective(&tempMat2, ENGINEMATH_DEG2RAD * 30, w / h, 1.0, 1000.0);
	engineMathMat4Multiply(&matrixProjection, &tempMat1, &tempMat2);
	// ワールド行列作成
	double distance = 20;
	double lookAtX = 0.0;
	double lookAtY = 0.0;
	double lookAtZ = 0.0;
	double angleCos = ENGINEMATH_COS045;
	double angleSin = ENGINEMATH_SIN045;
	double rotateCos = ENGINEMATH_COS000;
	double rotateSin = ENGINEMATH_SIN000;
	double lookEyeX = lookAtX + distance * angleCos * rotateSin;
	double lookEyeY = lookAtY + distance * angleSin;
	double lookEyeZ = lookAtZ + distance * angleCos * rotateCos;
	double lookUpX = 0.0;
	double lookUpY = 1.0;
	double lookUpZ = 0.0;
	engineMathMat4LookAt(&matrixModelViewWorld, lookEyeX, lookEyeY, lookEyeZ, lookAtX, lookAtY, lookAtZ, lookUpX, lookUpY, lookUpZ);

	// モードとバッファ設定
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_PHONG);
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindNormVBO(this->egoIdNorm);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	engineGraphicEngineBindTexture(this->egoIdTexSystem);
	engineGraphicEngineSetColorVec(color);

	// ローカル行列作成
	engineMathMat4Copy(&tempMat1, &matrixModelViewWorld);
	engineMathMat4Translate(&tempMat1, this->satelliteX, 0, this->satelliteY);
	engineMathMat4RotateY(&tempMat1, ENGINEMATH_DEG2RAD * this->step);

	// 球描画
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, 0.0, 0.0, 0.0);
	engineGraphicEngineSetMatrixNorm(&matrixProjection, &tempMat2);
	enginePrimitiveSphereDraw(&this->primitiveSphere);

	// トーラス描画
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, 0.0, 0.0, 0.0);
	engineGraphicEngineSetMatrixNorm(&matrixProjection, &tempMat2);
	enginePrimitiveTorusDraw(&this->primitiveTorus);

	// 水滴描画
	engineMathMat4Copy(&tempMat2, &tempMat1);
	engineMathMat4Translate(&tempMat2, 0.0, 1.0, 0.0);
	engineGraphicEngineSetMatrixNorm(&matrixProjection, &tempMat2);
	enginePrimitiveWaterDraw(&this->primitiveWater);

	// 描画モード設定
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL);

	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct pageTest2PartsPopupActiveDisplayImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexSystem);
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// アクティブポップアップディスプレイ構造体 作成
struct pageTest2PartsPopupActiveDisplay *pageTest2PartsPopupActiveDisplayCreate(struct pageTest2Status *stat){
	struct pageTest2PartsPopupActiveDisplayImplement *this = (struct pageTest2PartsPopupActiveDisplayImplement*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2PartsPopupActiveDisplayImplement));
	this->stat = stat;
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct pageTest2PartsPopupActiveDisplay*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

