#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsRootImplement{
	struct engineLayoutViewPartsRoot super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsRootImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutViewPartsRootImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// レイアウトモード変更 描画モード
	engineLayoutViewUtilPositionModeSetDraw();

	// レイアウト計算
	engineLayoutViewUtilPositionCalcLayoutAll((struct engineLayoutView*)this);

	// 描画オブジェクト作成
	engineLayoutViewUtilGraphicObjectBufferActiveAll((struct engineLayoutView*)this);
	engineLayoutViewUtilGraphicObjectBufferCreateAll((struct engineLayoutView*)this);

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
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);

	// レイアウトモード変更 計算モード
	engineLayoutViewUtilPositionModeSetCalc();
}

// ----------------------------------------------------------------

// ルート構造体 作成
struct engineLayoutViewPartsRoot *engineLayoutViewPartsRootCreate(){
	struct engineLayoutViewPartsRootImplement *this = (struct engineLayoutViewPartsRootImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsRoot", 1, sizeof(struct engineLayoutViewPartsRootImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewUtilGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewUtilGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutViewPartsRoot*)this;
}

// ----------------------------------------------------------------

// ルート構造体 運用
void engineLayoutViewPartsRootTouch(struct engineLayoutViewPartsRoot *this, bool isCancel){engineLayoutViewUtilInteractTouchRoot((struct engineLayoutView*)this, isCancel);}
void engineLayoutViewPartsRootCalc(struct engineLayoutViewPartsRoot *this, bool isCancel){this->super.calc((struct engineLayoutView*)this, isCancel);}
void engineLayoutViewPartsRootDraw(struct engineLayoutViewPartsRoot *this){this->super.draw((struct engineLayoutView*)this, NULL, NULL);}
void engineLayoutViewPartsRootPause(struct engineLayoutViewPartsRoot *this){this->super.pause((struct engineLayoutView*)this);}
void engineLayoutViewPartsRootDispose(struct engineLayoutViewPartsRoot *this){this->super.dispose((struct engineLayoutView*)this);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

