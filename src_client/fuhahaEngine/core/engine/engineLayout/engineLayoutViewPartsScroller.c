#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsScrollerImplement{
	struct engineLayoutViewPartsScroller super;
	struct{int x; int y;} temp0;
	struct{int x; int y;} temp1;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsScrollerImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	this->super.super.interact.setting.isTouchable = true;

	this->super.viewInner = engineLayoutViewCreate();
	this->super.viewBarX = engineLayoutViewCreate();
	this->super.viewBarY = engineLayoutViewCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewBarX);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewBarY);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsScrollerImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	// タッチ開始時に親要素の範囲外ならば子要素はタッチ開始できない
	if(dn && !mv && !this->super.super.position.isInner((struct engineLayoutView*)this, x, y)){isCancel = true;}

	// スクロール開始したら子要素のタッチ処理をキャンセルする
	// ただし子要素がスクロール開始したら自身はスクロール開始できない
	this->super.bubbling.isChildrenMove = false;
	bool isActive = false;
	bool isMove = (this->super.super.interact.status.isActive && this->super.super.interact.status.isMove);
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive || isMove); isActive = isActiveChild || isActive;
	bool isChildrenMove = this->super.bubbling.isChildrenMove;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isChildrenMove); isActive = isActiveLocal || isActive;

	// 必要ならば親要素に子要素がスクロールしたことを伝える
	if(this->super.bubbling.parentScroller != NULL){
		isMove = (this->super.super.interact.status.isActive && this->super.super.interact.status.isMove);
		if(isMove || isChildrenMove){this->super.bubbling.parentScroller->bubbling.isChildrenMove = true;}
	}

	if(isActiveLocal){
		// ローカル座標変換
		struct engineMathVector3 tempVec1;
		engineMathVec3Set(&tempVec1, x, y, 0);
		engineLayoutViewUtilPositionTransformCalcInvert((struct engineLayoutView*)this, &tempVec1);
		// 新タッチ位置更新
		this->temp1.x = tempVec1.x;
		this->temp1.y = tempVec1.y;
	}else if(this->super.super.interact.status.touchIndex == touchIndex){
		// タッチしていない時
	}

	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsScrollerImplement *this){
	if(this->super.super.interact.status.isActive && this->super.super.interact.status.isMove){
		// スクロール中
		this->super.velocity.x = this->temp1.x - this->temp0.x + this->super.velocity.x * 0.3;
		this->super.velocity.y = this->temp1.y - this->temp0.y + this->super.velocity.y * 0.3;
		this->super.position.x += this->temp1.x - this->temp0.x;
		this->super.position.y += this->temp1.y - this->temp0.y;
	}else if(engineMathAbs(this->super.velocity.x) > 0.01 || engineMathAbs(this->super.velocity.y) > 0.01){
		// 速度が生きてる
		this->super.velocity.x *= 0.8;
		this->super.velocity.y *= 0.8;
		this->super.position.x += this->super.velocity.x;
		this->super.position.y += this->super.velocity.y;
	}

	// 旧タッチ位置更新
	this->temp0.x = this->temp1.x;
	this->temp0.y = this->temp1.y;

	// 位置大きさ取得
	double innerW = this->super.inner.w;
	double innerH = this->super.inner.h;
	double outerW = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
	double outerH = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
	double barX = 0;
	double barY = 0;
	double barW = outerW;
	double barH = outerH;
	this->super.super.interact.setting.isMoveableX = (innerW > outerW);
	this->super.super.interact.setting.isMoveableY = (innerH > outerH);

	// x軸調整
	if(this->super.super.interact.setting.isMoveableX){
		double minX = 0;
		double maxX = innerW - outerW;
		if(this->super.position.x < -maxX){this->super.position.x = -maxX;}
		if(this->super.position.x > -minX){this->super.position.x = -minX;}
		barX = -this->super.position.x * outerW / innerW;
		barW = outerW * outerW / innerW;
	}else{
		this->super.position.x = 0;
		innerW = outerW;
	}

	// y軸調整
	if(this->super.super.interact.setting.isMoveableY){
		double minY = 0;
		double maxY = innerH - outerH;
		if(this->super.position.y < -maxY){this->super.position.y = -maxY;}
		if(this->super.position.y > -minY){this->super.position.y = -minY;}
		barY = -this->super.position.y * outerH / innerH;
		barH = outerH * outerH / innerH;
	}else{
		this->super.position.y = 0;
		innerH = outerH;
	}

	// 位置設定
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewInner, this->super.position.x, this->super.position.y, innerW, innerH);
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewBarX, barX, 0, barW, outerH);
	engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)this->super.viewBarY, 0, barY, outerW, barH);

	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayoutViewPartsScrollerImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsScrollerImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsScrollerImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsScroller", this);
}

// ----------------------------------------------------------------

// スクローラ構造体 作成
struct engineLayoutViewPartsScroller *engineLayoutViewPartsScrollerCreate(){
	struct engineLayoutViewPartsScrollerImplement *this = (struct engineLayoutViewPartsScrollerImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsScroller", 1, sizeof(struct engineLayoutViewPartsScrollerImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	return (struct engineLayoutViewPartsScroller*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

