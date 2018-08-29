#include "../../library.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsButtonOuterImplement{
	struct engineLayoutViewPartsButtonOuter super;
};

// ----------------------------------------------------------------

// タッチ判定
static bool isInner(struct engineLayoutViewPartsButtonOuterImplement *this, double x, double y){
	// 自要素の内側にあることを確認する
	if(!engineLayoutViewUtilPositionTransformIsInner((struct engineLayoutView*)this, x, y)){return false;}
	// 子要素の外側にあることを確認する 孫要素は確認しない
	struct engineLayoutView *temp = this->super.super.family.childrenHead;
	while(temp != NULL){
		if(temp->position.isInner(temp, x, y)){return false;}
		temp = temp->family.next;
	}
	return true;
}

// 初期化
static void init(struct engineLayoutViewPartsButtonOuterImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// タッチ設定
	this->super.super.position.isInner = (bool(*)(struct engineLayoutView*, double, double))isInner;
	this->super.super.interact.setting.isTouchable = true;
}

// ----------------------------------------------------------------

// 外枠ボタン構造体 作成
struct engineLayoutViewPartsButtonOuter *engineLayoutViewPartsButtonOuterCreate(){
	struct engineLayoutViewPartsButtonOuterImplement *this = (struct engineLayoutViewPartsButtonOuterImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsButtonOuter", 1, sizeof(struct engineLayoutViewPartsButtonOuterImplement));
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = engineLayoutViewDefaultDispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewUtilGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewUtilGraphicObjectDefaultBufferCreate;
	return (struct engineLayoutViewPartsButtonOuter*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

