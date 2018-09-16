#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsRootImplement{
	struct engineLayoutPartsRoot super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsRootImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutPartsRootImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// レイアウトモード変更 描画モード
	engineLayoutViewGearPositionModeSetDraw();

	// レイアウト計算
	engineLayoutViewGearPositionCalcLayoutAll((struct engineLayoutView*)this);

	// 描画オブジェクト作成
	engineLayoutViewGearGraphicObjectBufferActiveAll((struct engineLayoutView*)this);
	engineLayoutViewGearGraphicObjectBufferCreateAll((struct engineLayoutView*)this);

	// ルートなので引数の行列を無視して正射影行列作成
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Ortho(&tempMat1, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);
	mat = &tempMat1;

	// ルートなので引数の色を無視して自身の色使用
	color = &this->super.super.color;

	// 描画モード設定
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL);
	engineGraphicStencilStackMaskRead();

	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);

	// レイアウトモード変更 計算モード
	engineLayoutViewGearPositionModeSetCalc();
}

// ----------------------------------------------------------------

// ルート構造体 作成
struct engineLayoutPartsRoot *engineLayoutPartsRootCreate(void){
	struct engineLayoutPartsRootImplement *this = (struct engineLayoutPartsRootImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineLayoutPartsRootImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutPartsRoot*)this;
}

// ----------------------------------------------------------------

// ルート構造体 運用
void engineLayoutPartsRootTouch(struct engineLayoutPartsRoot *this, bool isCancel){engineLayoutViewGearInteractTouchRoot((struct engineLayoutView*)this, isCancel);}
void engineLayoutPartsRootCalc(struct engineLayoutPartsRoot *this, bool isCancel){this->super.calc((struct engineLayoutView*)this, isCancel);}
void engineLayoutPartsRootDraw(struct engineLayoutPartsRoot *this){this->super.draw((struct engineLayoutView*)this, NULL, NULL);}
void engineLayoutPartsRootPause(struct engineLayoutPartsRoot *this){this->super.pause((struct engineLayoutView*)this);}
void engineLayoutPartsRootDispose(struct engineLayoutPartsRoot *this){this->super.dispose((struct engineLayoutView*)this);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

