#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout02/engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayout02ViewPartsScrollerImplement{
	struct engineLayout02ViewPartsScroller super;
	struct{int x; int y;} temp0;
	struct{int x; int y;} temp1;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayout02ViewPartsScrollerImplement *this){
	// レイアウト初期化
	engineLayout02ViewUtilPositionInit((struct engineLayout02View*)this);
	this->super.super.interact.setting.isTouchable = true;

	this->super.viewInner = engineLayout02ViewCreate();
	this->super.viewBarX = engineLayout02ViewCreate();
	this->super.viewBarY = engineLayout02ViewCreate();
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.viewInner);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.viewBarX);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this, (struct engineLayout02View*)this->super.viewBarY);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayout02ViewPartsScrollerImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	// タッチ開始時に親要素の範囲外ならば子要素はタッチ開始できない
	if(dn && !mv && !engineLayout02ViewUtilPositionTransformIsInner((struct engineLayout02View*)this, x, y)){isCancel = true;}

	// 新タッチ位置更新
	this->temp1.x = x;
	this->temp1.y = y;

	// スクロール開始したら子要素のタッチ処理をキャンセルする
	// ただし子要素がスクロール開始したら自身はスクロール開始できない
	this->super.bubbling.isChildrenMove = false;
	bool isActive = false;
	bool isMove = (this->super.super.interact.status.isActive && this->super.super.interact.status.isMove);
	isActive = engineLayout02ViewUtilChildrenTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isActive || isMove) || isActive;
	bool isChildrenMove = this->super.bubbling.isChildrenMove;
	isActive = engineLayout02ViewUtilInteractTouch((struct engineLayout02View*)this, touchIndex, x, y, dn, mv, isCancel || isChildrenMove) || isActive;
	// 必要ならば親要素に子要素がスクロールしたことを伝える
	if(this->super.bubbling.parentScroller != NULL){
		isMove = (this->super.super.interact.status.isActive && this->super.super.interact.status.isMove);
		if(isMove || isChildrenMove){this->super.bubbling.parentScroller->bubbling.isChildrenMove = true;}
	}
	return isActive;
}

// 計算
static void calc(struct engineLayout02ViewPartsScrollerImplement *this){
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
	double outerW = engineLayout02ViewUtilPositionGetW((struct engineLayout02View*)this);
	double outerH = engineLayout02ViewUtilPositionGetH((struct engineLayout02View*)this);
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
	engineLayout02ViewUtilPositionSetLtTpWtHt((struct engineLayout02View*)this->super.viewInner, this->super.position.x, this->super.position.y, innerW, innerH);
	engineLayout02ViewUtilPositionSetLtTpWtHt((struct engineLayout02View*)this->super.viewBarX, barX, 0, barW, outerH);
	engineLayout02ViewUtilPositionSetLtTpWtHt((struct engineLayout02View*)this->super.viewBarY, 0, barY, outerW, barH);

	// 子要素計算
	engineLayout02ViewUtilChildrenCalc((struct engineLayout02View*)this);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayout02ViewPartsScrollerImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayout02ViewUtilChildrenDraw((struct engineLayout02View*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayout02ViewPartsScrollerImplement *this){
	// 子要素一時停止
	engineLayout02ViewUtilChildrenPause((struct engineLayout02View*)this);
}

// 破棄
static void dispose(struct engineLayout02ViewPartsScrollerImplement *this){
	// 子要素破棄
	engineLayout02ViewUtilChildrenDispose((struct engineLayout02View*)this);

	// 自要素破棄
	engineLayout02ViewUtilPositionDispose((struct engineLayout02View*)this);
	engineLayout02ViewDetouch((struct engineLayout02View*)this);
	engineUtilMemoryInfoFree("engineLayout02ViewPartsScroller", this);
}

// ----------------------------------------------------------------

// スクローラ構造体 作成
struct engineLayout02ViewPartsScroller *engineLayout02ViewPartsScrollerCreate(){
	struct engineLayout02ViewPartsScrollerImplement *this = (struct engineLayout02ViewPartsScrollerImplement*)engineUtilMemoryInfoCalloc("engineLayout02ViewPartsScroller", 1, sizeof(struct engineLayout02ViewPartsScrollerImplement));
	init(this);

	struct engineLayout02View *view = (struct engineLayout02View*)this;
	view->touch = (bool(*)(struct engineLayout02View*, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel))touch;
	view->calc = (void(*)(struct engineLayout02View*))calc;
	view->draw = (void(*)(struct engineLayout02View*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayout02View*))pause;
	view->dispose = (void(*)(struct engineLayout02View*))dispose;
	return (struct engineLayout02ViewPartsScroller*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

