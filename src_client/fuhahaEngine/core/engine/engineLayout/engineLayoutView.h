#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プロトタイプ宣言
struct engineLayoutView;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素
struct engineLayoutViewUtilFamily{
	struct engineLayoutView *parent;
	struct engineLayoutView *prev;
	struct engineLayoutView *next;
	struct engineLayoutView *childrenHead;
	struct engineLayoutView *childrenTail;
	double zIndex;
	bool isInactive;
	bool isInvisible;
};

// 表示要素構造体子要素 初期化
void engineLayoutViewUtilFamilyInit(struct engineLayoutView *this);
// 表示要素構造体子要素 子要素追加
void engineLayoutViewUtilFamilyAdd(struct engineLayoutView *this, struct engineLayoutView *child);
// 表示要素構造体子要素 子要素除外
void engineLayoutViewUtilFamilyRemove(struct engineLayoutView *this, struct engineLayoutView *child, bool isDispose);
// 表示要素構造体子要素 破棄
void engineLayoutViewUtilFamilyDispose(struct engineLayoutView *this);

// 表示要素構造体子要素 タッチ処理
bool engineLayoutViewUtilChildrenTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
// 表示要素構造体子要素 計算
void engineLayoutViewUtilChildrenCalc(struct engineLayoutView *this);
// 表示要素構造体子要素 描画
void engineLayoutViewUtilChildrenDraw(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 表示要素構造体子要素 一時停止
void engineLayoutViewUtilChildrenPause(struct engineLayoutView *this);
// 表示要素構造体子要素 破棄
void engineLayoutViewUtilChildrenDispose(struct engineLayoutView *this);

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

// 表示要素構造体対話型
struct engineLayoutViewUtilInteract{
	struct{
		bool isTouchable;
		bool isMoveableX;
		bool isMoveableY;
	} setting;
	struct{
		int touchIndex;
		bool isActive;
		bool isHover;
		bool isMove;
		bool isTriggerDn;
		bool isTriggerUp;
		int startX;
		int startY;
		int currX;
		int currY;
	} status;
};

// 表示要素構造体対話型 タッチ処理
bool engineLayoutViewUtilInteractTouch(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);

// 表示要素構造体対話型 基盤タッチ処理
void engineLayoutViewUtilInteractTouchRoot(struct engineLayoutView *this, bool isCancel);

// 表示要素構造体対話型 ボタンタッチ確認
bool engineLayoutViewUtilInteractIsTriggerUp(struct engineLayoutView *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体
struct engineLayoutView{
	struct engineLayoutViewUtilFamily family;
	struct engineLayoutViewUtilPosition position;
	struct engineLayoutViewUtilInteract interact;

	bool (*touch)(struct engineLayoutView *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel);
	void (*calc)(struct engineLayoutView *this);
	void (*draw)(struct engineLayoutView *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*pause)(struct engineLayoutView *this);
	void (*dispose)(struct engineLayoutView *this);
};

// 表示要素構造体 作成
struct engineLayoutView *engineLayoutViewCreate();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ユーティリティマクロ

#define engineLayoutViewTouch(this, touchId, x, y, dn, mv, isCancel) (this)->touch(this, touchId, x, y, dn, mv, isCancel)
#define engineLayoutViewCalc(this) (this)->calc(this)
#define engineLayoutViewDraw(this, mat, color) (this)->draw(this, mat, color)
#define engineLayoutViewPause(this) (this)->pause(this)
#define engineLayoutViewDispose(this) (this)->dispose(this)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

