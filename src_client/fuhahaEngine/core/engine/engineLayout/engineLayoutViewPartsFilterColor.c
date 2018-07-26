#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsFilterColorImplement{
	struct engineLayoutViewPartsFilterColor super;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsFilterColorImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// デフォルトパラメータ設定
	this->super.alpha = 1.0;
	engineMathMat4Identity(&this->super.matrix);
	engineMathVec4Set(&this->super.vector, 0, 0, 0, 0);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsFilterColorImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsFilterColorImplement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsFilterColorImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsFilterColorImplement *this){
}

// 描画
static void draw(struct engineLayoutViewPartsFilterColorImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// モード変更
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D_FILTER_COLOR_ALPHA_NORMAL);
	engineGraphicEngineSetFilterColorAlpha(this->super.alpha);
	engineGraphicEngineSetFilterColorMatrix(&this->super.matrix);
	engineGraphicEngineSetFilterColorVector(&this->super.vector);

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);

	// モード戻す
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsFilterColorImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsFilterColorImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsFilterColor", this);
}

// ----------------------------------------------------------------

// カラーマトリクスフィルタ構造体 作成
struct engineLayoutViewPartsFilterColor *engineLayoutViewPartsFilterColorCreate(){
	struct engineLayoutViewPartsFilterColorImplement *this = (struct engineLayoutViewPartsFilterColorImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsFilterColor", 1, sizeof(struct engineLayoutViewPartsFilterColorImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsFilterColor*)this;
}

// ----------------------------------------------------------------

// カラーマトリクスフィルタ構造体 色行列設定 よくわからないけど色反転っぽい変換
void engineLayoutViewPartsFilterColorCreateSetNegative1(struct engineLayoutViewPartsFilterColor *this){
	struct engineMathMatrix44 *m = &this->matrix;
	struct engineMathVector4 *v = &this->vector;
	m->m00 = 0.0; m->m01 = 1.0; m->m02 = 1.0; m->m03 = 0.0; v->r = 0.0;
	m->m10 = 1.0; m->m11 = 0.0; m->m12 = 1.0; m->m13 = 0.0; v->g = 0.0;
	m->m20 = 1.0; m->m21 = 1.0; m->m22 = 0.0; m->m23 = 0.0; v->b = 0.0;
	m->m30 = 0.0; m->m31 = 0.0; m->m32 = 0.0; m->m33 = 1.0; v->a = 0.0;
}

// カラーマトリクスフィルタ構造体 色行列設定 よくわからないけど明度反転っぽい変換
void engineLayoutViewPartsFilterColorCreateSetNegative2(struct engineLayoutViewPartsFilterColor *this){
	struct engineMathMatrix44 *m = &this->matrix;
	struct engineMathVector4 *v = &this->vector;
	m->m00 =  0.0; m->m01 = -1.0; m->m02 = -1.0; m->m03 = 0.0; v->r = 2.0;
	m->m10 = -1.0; m->m11 =  0.0; m->m12 = -1.0; m->m13 = 0.0; v->g = 2.0;
	m->m20 = -1.0; m->m21 = -1.0; m->m22 =  0.0; m->m23 = 0.0; v->b = 2.0;
	m->m30 =  0.0; m->m31 =  0.0; m->m32 =  0.0; m->m33 = 1.0; v->a = 0.0;
}

// カラーマトリクスフィルタ構造体 色行列設定 グレースケール
void engineLayoutViewPartsFilterColorCreateSetGrayscale(struct engineLayoutViewPartsFilterColor *this){
	struct engineMathMatrix44 *m = &this->matrix;
	struct engineMathVector4 *v = &this->vector;
	m->m00 = 0.33; m->m01 = 0.59; m->m02 = 0.11; m->m03 = 0.0; v->r = 0.0;
	m->m10 = 0.33; m->m11 = 0.59; m->m12 = 0.11; m->m13 = 0.0; v->g = 0.0;
	m->m20 = 0.33; m->m21 = 0.59; m->m22 = 0.11; m->m23 = 0.0; v->b = 0.0;
	m->m30 = 0.00; m->m31 = 0.00; m->m32 = 0.00; m->m33 = 1.0; v->a = 0.0;
}

// カラーマトリクスフィルタ構造体 色行列設定 セピア
void engineLayoutViewPartsFilterColorCreateSetSepia(struct engineLayoutViewPartsFilterColor *this){
	struct engineMathMatrix44 *m = &this->matrix;
	struct engineMathVector4 *v = &this->vector;
	m->m00 = 0.393; m->m01 = 0.769; m->m02 = 0.189; m->m03 = 0.0; v->r = 0.0;
	m->m10 = 0.349; m->m11 = 0.686; m->m12 = 0.168; m->m13 = 0.0; v->g = 0.0;
	m->m20 = 0.272; m->m21 = 0.534; m->m22 = 0.131; m->m23 = 0.0; v->b = 0.0;
	m->m30 = 0.000; m->m31 = 0.000; m->m32 = 0.000; m->m33 = 1.0; v->a = 0.0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

