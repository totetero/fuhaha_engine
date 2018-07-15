#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsSwipePagerImplement{
	struct engineLayoutViewPartsSwipePager super;
	struct{int x;} posTemp;
	bool isSwipe;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsSwipePagerImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	this->super.super.interact.setting.isTouchable = true;
	this->super.super.interact.setting.isMoveableX = true;
	this->super.super.interact.setting.isMoveableY = false;

	// スクロール要素作成
	this->super.viewInner = engineLayoutViewCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->super.viewInner);
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsSwipePagerImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	return engineLayoutSwipeTouch(&this->super.swipe, (struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel);
}

// 計算
static void calc(struct engineLayoutViewPartsSwipePagerImplement *this){
	// ページ数を数える
	this->super.count = 0;
	struct engineLayoutView *temp1 = this->super.viewInner->family.childrenHead;
	while(temp1 != NULL){
		this->super.count++;
		temp1 = temp1->family.next;
	}

	// 大きさ取得
	double outerW = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
	double outerH = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
	double innerW = this->super.count * outerW;

	if(outerW > 0){
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
			double posX = -this->super.index * outerW;
			double diff = posX - this->super.position.x;
			if(engineMathAbs(diff) > 0.01){
				this->super.position.x += diff * 0.1;
			}else{
				this->super.position.x = posX;
			}
		}

		// 位置設定
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
			engineLayoutViewUtilPositionSetLtTpWtHt((struct engineLayoutView*)temp2, posLeft, 0, outerW, outerH);
			posLeft += outerW;

			temp2 = temp2->family.next;
		}
	}

	// 旧タッチ位置更新
	this->posTemp.x = this->super.swipe.x;

	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsSwipePagerImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsSwipePagerImplement *this){
}

// 描画
static void draw(struct engineLayoutViewPartsSwipePagerImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsSwipePagerImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsSwipePagerImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsSwipePager", this);
}

// ----------------------------------------------------------------

// ページャー構造体 作成
struct engineLayoutViewPartsSwipePager *engineLayoutViewPartsSwipePagerCreate(){
	struct engineLayoutViewPartsSwipePagerImplement *this = (struct engineLayoutViewPartsSwipePagerImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsSwipePager", 1, sizeof(struct engineLayoutViewPartsSwipePagerImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsSwipePager*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

