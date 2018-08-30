#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsSwipePagerImplement{
	struct engineLayoutPartsSwipePager super;
	struct{double w;} screenTemp;
	struct{int x;} posTemp;
	bool isSwipe;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutPartsSwipePagerImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.interact.setting.isTouchable = true;
	this->super.super.interact.setting.isMoveableX = true;
	this->super.super.interact.setting.isMoveableY = false;

	// スクロール要素作成
	this->super.viewInner = engineLayoutViewCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutPartsSwipePagerImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	return engineLayoutUtilSwipeTouch(&this->super.swipe, (struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel);
}

// 計算
static void calc(struct engineLayoutPartsSwipePagerImplement *this, bool isCancel){
	// ページ数を数える
	this->super.count = 0;
	struct engineLayoutView *temp1 = this->super.viewInner->family.childrenHead;
	while(temp1 != NULL){
		this->super.count++;
		temp1 = temp1->family.next;
	}

	// 大きさ取得
	double outerW = engineLayoutViewGearPositionGetW((struct engineLayoutView*)this);
	double innerW = this->super.count * outerW;

	if(outerW > 0){
		// 画面サイズ変更時の位置調整
		if(engineMathAbs(this->screenTemp.w - outerW) >= DBL_EPSILON){
			this->screenTemp.w = outerW;
			this->super.velocity.x = 0;
			this->super.position.x = -this->super.index * outerW;
			this->isSwipe = false;
		}

		if(this->super.super.interact.status.isActive && this->super.super.interact.status.isMove){
			// スワイプ中
			this->super.velocity.x = this->super.swipe.x - this->posTemp.x + this->super.velocity.x * 0.3;
			this->super.position.x += this->super.swipe.x - this->posTemp.x;
			this->isSwipe = true;
		}else{
			if(this->isSwipe){
				// スワイプ完了
				this->isSwipe = false;
				if(this->super.velocity.x > 5.0){
					// 前のページへ戻る
					this->super.index -= 1;
				}else if(this->super.velocity.x < -5.0){
					// 次のページへ進む
					this->super.index += 1;
				}else{
					// 現在表示しているページ
					this->super.index = (int)engineMathRound(-this->super.position.x / outerW);
				}
				this->super.velocity.x = 0;

				if(!this->super.isLoop){
					// ループしないので範囲の制限
					double indexMin = 0;
					double indexMax = this->super.count - 1;
					if(this->super.index < indexMin){this->super.index = indexMin;}
					if(this->super.index > indexMax){this->super.index = indexMax;}
				}
			}

			// 位置調整
			double posX0 = this->super.position.x;
			double posX1 = -this->super.index * outerW;
			this->super.position.x = (engineMathAbs(posX1 - posX0) > 0.01) ? (posX0 + (posX1 - posX0) * 0.1) : posX1;
		}

		// スクロール要素の位置設定
		double posMin = -innerW * 0.5;
		double posMax = innerW * 0.5;
		double posLeft = this->super.position.x;
		if(this->super.isLoop){while(posLeft < posMin){posLeft += innerW;}}
		struct engineLayoutView *temp2 = this->super.viewInner->family.childrenHead;
		while(temp2 != NULL){
			if(this->super.isLoop){while(posLeft > posMax){posLeft -= innerW;}}

			// 表示範囲制限
			temp2->family.isInvisible = (posLeft + outerW <= 0 || outerW <= posLeft);

			// 位置設定
			engineLayoutViewGearPositionSetLtTpBmWt((struct engineLayoutView*)temp2, posLeft, 0, 0, outerW);
			posLeft += outerW;

			temp2 = temp2->family.next;
		}
	}

	// 旧タッチ位置更新
	this->posTemp.x = this->super.swipe.x;

	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// ページャー構造体 作成
struct engineLayoutPartsSwipePager *engineLayoutPartsSwipePagerCreate(){
	struct engineLayoutPartsSwipePagerImplement *this = (struct engineLayoutPartsSwipePagerImplement*)engineUtilMemoryInfoCalloc("engineLayoutPartsSwipePager", 1, sizeof(struct engineLayoutPartsSwipePagerImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutPartsSwipePager*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

