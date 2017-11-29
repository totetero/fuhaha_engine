#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プロトタイプ宣言
struct engineLayout02View;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素
struct engineLayout02ViewUtilChildren{
	struct engineLayout02View *parent;
	struct engineLayout02View *prev;
	struct engineLayout02View *next;
	struct engineLayout02View *childrenHead;
	struct engineLayout02View *childrenTail;
	double zIndex;
	bool isInactive;
	bool isInvisible;
};

// 表示要素構造体子要素 子要素追加
void engineLayout02ViewUtilChildrenAdd(struct engineLayout02View *this, struct engineLayout02View *child);
// 表示要素構造体子要素 子要素排除
void engineLayout02ViewUtilChildrenRemove(struct engineLayout02View *this, struct engineLayout02View *child);

// 表示要素構造体子要素 計算
void engineLayout02ViewUtilChildrenCalc(struct engineLayout02View *this);
// 表示要素構造体子要素 描画
void engineLayout02ViewUtilChildrenDraw(struct engineLayout02View *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示要素構造体子要素 一時停止
void engineLayout02ViewUtilChildrenPause(struct engineLayout02View *this);
// 表示要素構造体子要素 破棄
void engineLayout02ViewUtilChildrenDispose(struct engineLayout02View *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体位置関係
struct engineLayout02ViewUtilPosition{
	struct engineLayout02ViewStyle{
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
		struct{bool isActive;} horizontalCentering;
		struct{bool isActive;} verticalCentering;
		struct{bool isActive; double originX; double originY; struct engineMathMatrix44 matrix;} transform;
	} style;

	struct engineLayout02ViewLayout{
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
};

// 表示要素構造体位置関係 計算モード設定
void engineLayout02ViewUtilPositionModeSetCalc();
// 表示要素構造体位置関係 描画モード設定
void engineLayout02ViewUtilPositionModeSetDraw();

// 表示要素構造体位置関係 初期化
void engineLayout02ViewUtilPositionInit(struct engineLayout02View *this);
// 表示要素構造体位置関係 破棄
void engineLayout02ViewUtilPositionDispose(struct engineLayout02View *this);

// 表示要素構造体位置関係 レイアウト設定
void engineLayout02ViewUtilPositionSetLeft(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetRight(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetTop(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetBottom(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetWidth(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetHeight(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetMarginLeft(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetMarginRight(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetMarginTop(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetMarginBottom(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetPaddingLeft(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetPaddingRight(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetPaddingTop(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetPaddingBottom(struct engineLayout02View *this, double value);
void engineLayout02ViewUtilPositionSetVerticalCentering(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionSetHorizontalCentering(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionSetTransformOrigin(struct engineLayout02View *this, double originX, double originY);
void engineLayout02ViewUtilPositionSetTransformMatrix(struct engineLayout02View *this, struct engineMathMatrix44 *matrix);
void engineLayout02ViewUtilPositionSetTransformScale(struct engineLayout02View *this, double x, double y, double z);
void engineLayout02ViewUtilPositionSetTransformRotate(struct engineLayout02View *this, double rad);

// 表示要素構造体位置関係 レイアウト解除
void engineLayout02ViewUtilPositionUnsetLeft(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetRight(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetTop(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetBottom(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetWidth(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetHeight(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetMarginLeft(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetMarginRight(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetMarginTop(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetMarginBottom(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetPaddingLeft(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetPaddingRight(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetPaddingTop(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetPaddingBottom(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetVerticalCentering(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetHorizontalCentering(struct engineLayout02View *this);
void engineLayout02ViewUtilPositionUnsetTransform(struct engineLayout02View *this);

// 表示要素構造体位置関係 レイアウト取得
double engineLayout02ViewUtilPositionGetX(struct engineLayout02View *this);
double engineLayout02ViewUtilPositionGetY(struct engineLayout02View *this);
double engineLayout02ViewUtilPositionGetW(struct engineLayout02View *this);
double engineLayout02ViewUtilPositionGetH(struct engineLayout02View *this);
// 表示要素構造体位置関係 変形レイアウト
void engineLayout02ViewUtilPositionTransformCalcMatrix(struct engineLayout02View *this, struct engineMathMatrix44 *dstMat, struct engineMathMatrix44 *srcMat);
bool engineLayout02ViewUtilPositionTransformIsInner(struct engineLayout02View *this, double x, double y);

// 表示要素構造体位置関係 設定ショートカット
void engineLayout02ViewUtilPositionSetLtRtTpBm(struct engineLayout02View *this, double lt, double rt, double tp, double bm);
void engineLayout02ViewUtilPositionSetLtTpWtHt(struct engineLayout02View *this, double lt, double tp, double wt, double ht);
void engineLayout02ViewUtilPositionSetMargin(struct engineLayout02View *this, double margin);
void engineLayout02ViewUtilPositionSetMarginHorizontal(struct engineLayout02View *this, double margin);
void engineLayout02ViewUtilPositionSetMarginVertical(struct engineLayout02View *this, double margin);
void engineLayout02ViewUtilPositionSetPadding(struct engineLayout02View *this, double padding);
void engineLayout02ViewUtilPositionSetPaddingHorizontal(struct engineLayout02View *this, double padding);
void engineLayout02ViewUtilPositionSetPaddingVertical(struct engineLayout02View *this, double padding);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体
struct engineLayout02View{
	struct engineLayout02ViewUtilChildren children;
	struct engineLayout02ViewUtilPosition position;

	void (*calc)(struct engineLayout02View *this);
	void (*draw)(struct engineLayout02View *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*pause)(struct engineLayout02View *this);
	void (*dispose)(struct engineLayout02View *this);
};

// 表示要素構造体 作成
struct engineLayout02View *engineLayout02ViewCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ユーティリティマクロ

#define engineLayout02ViewCalc(this) (this)->calc(this)
#define engineLayout02ViewDraw(this, mat, color) (this)->draw(this, mat, color)
#define engineLayout02ViewPause(this) (this)->pause(this)
#define engineLayout02ViewDispose(this) (this)->dispose(this)
#define engineLayout02ViewDetouch(this) engineLayout02ViewUtilChildrenRemove((this)->children.parent, this)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

