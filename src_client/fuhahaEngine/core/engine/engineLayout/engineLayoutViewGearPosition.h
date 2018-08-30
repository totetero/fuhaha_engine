#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体位置関係
struct engineLayoutViewGearPosition{
	struct engineLayoutViewStyle{
		struct{bool isActive; double value;} left;
		struct{bool isActive; double value;} right;
		struct{bool isActive; double value;} top;
		struct{bool isActive; double value;} bottom;
		struct{bool isActive; double value; bool isRatio;} width;
		struct{bool isActive; double value; bool isRatio;} height;
		struct{bool isActive; double value;} marginLeft;
		struct{bool isActive; double value;} marginRight;
		struct{bool isActive; double value;} marginTop;
		struct{bool isActive; double value;} marginBottom;
		struct{bool isActive; double value;} paddingLeft;
		struct{bool isActive; double value;} paddingRight;
		struct{bool isActive; double value;} paddingTop;
		struct{bool isActive; double value;} paddingBottom;
		struct{bool isActive; double min; double max; int align;} clampHorizontal;
		struct{bool isActive; double min; double max; int align;} clampVertical;
		struct{bool isActive; double originX; double originY; struct engineMathMatrix44 matrix;} transform;
	} style;

	struct engineLayoutViewLayout{
		int generationCount;
		int generationCountInvert;
		double x;
		double y;
		double w;
		double h;
		struct{
			bool isActive;
			struct engineMathMatrix44 matrix;
			struct engineMathMatrix44 invert;
		} transform;
	} layout;

	bool (*isInner)(struct engineLayoutView *this, double x, double y);
};

// 表示要素構造体位置関係 計算モード設定
void engineLayoutViewGearPositionModeSetCalc(void);
// 表示要素構造体位置関係 描画モード設定
void engineLayoutViewGearPositionModeSetDraw(void);

// レイアウトを明示的に計算
void engineLayoutViewGearPositionCalcLayoutAll(struct engineLayoutView *this);

// 表示要素構造体位置関係 初期化
void engineLayoutViewGearPositionInit(struct engineLayoutView *this);
// 表示要素構造体位置関係 破棄
void engineLayoutViewGearPositionDispose(struct engineLayoutView *this);

// 表示要素構造体位置関係 レイアウト設定
void engineLayoutViewGearPositionSetLeft(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetRight(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetTop(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetBottom(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetWidthValue(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetHeightValue(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetWidthRatio(struct engineLayoutView *this, double ratio);
void engineLayoutViewGearPositionSetHeightRatio(struct engineLayoutView *this, double ratio);
void engineLayoutViewGearPositionSetMarginLeft(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetMarginRight(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetMarginTop(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetMarginBottom(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetPaddingLeft(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetPaddingRight(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetPaddingTop(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetPaddingBottom(struct engineLayoutView *this, double value);
void engineLayoutViewGearPositionSetClampHorizontal(struct engineLayoutView *this, double min, double max, int align);
void engineLayoutViewGearPositionSetClampVertical(struct engineLayoutView *this, double min, double max, int align);
void engineLayoutViewGearPositionSetTransformOrigin(struct engineLayoutView *this, double originX, double originY);
void engineLayoutViewGearPositionSetTransformMatrix(struct engineLayoutView *this, struct engineMathMatrix44 *matrix);
void engineLayoutViewGearPositionSetTransformScale(struct engineLayoutView *this, double x, double y);
void engineLayoutViewGearPositionSetTransformRotate(struct engineLayoutView *this, double rad);

// 表示要素構造体位置関係 レイアウト解除
void engineLayoutViewGearPositionUnsetLeft(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetRight(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetTop(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetBottom(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetWidth(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetHeight(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetMarginLeft(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetMarginRight(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetMarginTop(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetMarginBottom(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetPaddingLeft(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetPaddingRight(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetPaddingTop(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetPaddingBottom(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetClampHorizontal(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetClampVertical(struct engineLayoutView *this);
void engineLayoutViewGearPositionUnsetTransform(struct engineLayoutView *this);

// 表示要素構造体位置関係 レイアウト取得
double engineLayoutViewGearPositionGetX(struct engineLayoutView *this);
double engineLayoutViewGearPositionGetY(struct engineLayoutView *this);
double engineLayoutViewGearPositionGetW(struct engineLayoutView *this);
double engineLayoutViewGearPositionGetH(struct engineLayoutView *this);
// 表示要素構造体位置関係 変形レイアウト
void engineLayoutViewGearPositionTransformCalcMatrix(struct engineLayoutView *this, struct engineMathMatrix44 *dstMat, struct engineMathMatrix44 *srcMat);
void engineLayoutViewGearPositionTransformCalcInvert(struct engineLayoutView *this, struct engineMathVector3 *position);
bool engineLayoutViewGearPositionTransformIsInner(struct engineLayoutView *this, double x, double y);

// 表示要素構造体位置関係 設定ショートカット
void engineLayoutViewGearPositionSetLtRtTpBm(struct engineLayoutView *this, double lt, double rt, double tp, double bm);
void engineLayoutViewGearPositionSetLtRtTpHt(struct engineLayoutView *this, double lt, double rt, double tp, double ht);
void engineLayoutViewGearPositionSetLtTpBmWt(struct engineLayoutView *this, double lt, double tp, double bm, double wt);
void engineLayoutViewGearPositionSetLtTpWtHt(struct engineLayoutView *this, double lt, double tp, double wt, double ht);
void engineLayoutViewGearPositionSetMargin(struct engineLayoutView *this, double margin);
void engineLayoutViewGearPositionSetMarginHorizontal(struct engineLayoutView *this, double margin);
void engineLayoutViewGearPositionSetMarginVertical(struct engineLayoutView *this, double margin);
void engineLayoutViewGearPositionSetPadding(struct engineLayoutView *this, double padding);
void engineLayoutViewGearPositionSetPaddingHorizontal(struct engineLayoutView *this, double padding);
void engineLayoutViewGearPositionSetPaddingVertical(struct engineLayoutView *this, double padding);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

