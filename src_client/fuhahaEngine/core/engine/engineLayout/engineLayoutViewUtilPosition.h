#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体位置関係
struct engineLayoutViewUtilPosition{
	struct engineLayoutViewStyle{
		struct{bool isActive; double value;} left;
		struct{bool isActive; double value;} right;
		struct{bool isActive; double value;} top;
		struct{bool isActive; double value;} bottom;
		struct{bool isActive; double value;} width;
		struct{bool isActive; double value;} height;
		struct{bool isActive; double value;} marginLeft;
		struct{bool isActive; double value;} marginRight;
		struct{bool isActive; double value;} marginTop;
		struct{bool isActive; double value;} marginBottom;
		struct{bool isActive; double value;} paddingLeft;
		struct{bool isActive; double value;} paddingRight;
		struct{bool isActive; double value;} paddingTop;
		struct{bool isActive; double value;} paddingBottom;
		struct{bool isActive;} centeringHorizontal;
		struct{bool isActive;} centeringVertical;
		struct{bool isActive; double originX; double originY; struct engineMathMatrix44 matrix;} transform;
	} style;

	struct engineLayoutViewLayout{
		int generationCount;
		double x;
		double y;
		double w;
		double h;
		struct{
			bool isActive;
			struct engineMathMatrix44 matrix;
			struct engineMathVector3 point[4];
		} transform;
	} layout;

	bool (*isInner)(struct engineLayoutView *this, double x, double y);
};

// 表示要素構造体位置関係 計算モード設定
void engineLayoutViewUtilPositionModeSetCalc();
// 表示要素構造体位置関係 描画モード設定
void engineLayoutViewUtilPositionModeSetDraw();

// 表示要素構造体位置関係 初期化
void engineLayoutViewUtilPositionInit(struct engineLayoutView *this);
// 表示要素構造体位置関係 破棄
void engineLayoutViewUtilPositionDispose(struct engineLayoutView *this);

// 表示要素構造体位置関係 レイアウト設定
void engineLayoutViewUtilPositionSetLeft(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetRight(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetTop(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetBottom(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetWidth(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetHeight(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetMarginLeft(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetMarginRight(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetMarginTop(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetMarginBottom(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetPaddingLeft(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetPaddingRight(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetPaddingTop(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetPaddingBottom(struct engineLayoutView *this, double value);
void engineLayoutViewUtilPositionSetCenteringVertical(struct engineLayoutView *this);
void engineLayoutViewUtilPositionSetCenteringHorizontal(struct engineLayoutView *this);
void engineLayoutViewUtilPositionSetTransformOrigin(struct engineLayoutView *this, double originX, double originY);
void engineLayoutViewUtilPositionSetTransformMatrix(struct engineLayoutView *this, struct engineMathMatrix44 *matrix);
void engineLayoutViewUtilPositionSetTransformScale(struct engineLayoutView *this, double x, double y, double z);
void engineLayoutViewUtilPositionSetTransformRotate(struct engineLayoutView *this, double rad);

// 表示要素構造体位置関係 レイアウト解除
void engineLayoutViewUtilPositionUnsetLeft(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetRight(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetTop(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetBottom(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetWidth(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetHeight(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetMarginLeft(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetMarginRight(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetMarginTop(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetMarginBottom(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetPaddingLeft(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetPaddingRight(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetPaddingTop(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetPaddingBottom(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetCenteringVertical(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetCenteringHorizontal(struct engineLayoutView *this);
void engineLayoutViewUtilPositionUnsetTransform(struct engineLayoutView *this);

// 表示要素構造体位置関係 レイアウト取得
double engineLayoutViewUtilPositionGetX(struct engineLayoutView *this);
double engineLayoutViewUtilPositionGetY(struct engineLayoutView *this);
double engineLayoutViewUtilPositionGetW(struct engineLayoutView *this);
double engineLayoutViewUtilPositionGetH(struct engineLayoutView *this);
// 表示要素構造体位置関係 変形レイアウト
void engineLayoutViewUtilPositionTransformCalcMatrix(struct engineLayoutView *this, struct engineMathMatrix44 *dstMat, struct engineMathMatrix44 *srcMat);
bool engineLayoutViewUtilPositionTransformIsInner(struct engineLayoutView *this, double x, double y);

// 表示要素構造体位置関係 設定ショートカット
void engineLayoutViewUtilPositionSetLtRtTpBm(struct engineLayoutView *this, double lt, double rt, double tp, double bm);
void engineLayoutViewUtilPositionSetLtTpWtHt(struct engineLayoutView *this, double lt, double tp, double wt, double ht);
void engineLayoutViewUtilPositionSetMargin(struct engineLayoutView *this, double margin);
void engineLayoutViewUtilPositionSetMarginHorizontal(struct engineLayoutView *this, double margin);
void engineLayoutViewUtilPositionSetMarginVertical(struct engineLayoutView *this, double margin);
void engineLayoutViewUtilPositionSetPadding(struct engineLayoutView *this, double padding);
void engineLayoutViewUtilPositionSetPaddingHorizontal(struct engineLayoutView *this, double padding);
void engineLayoutViewUtilPositionSetPaddingVertical(struct engineLayoutView *this, double padding);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

