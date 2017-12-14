#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest3{
	struct engineCartridgePage super;

	struct engineLayout02View *viewRoot;
	struct engineLayout02ViewPartsButtonBox *viewTestButton;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest3 *this){
	// ルート作成
	this->viewRoot = engineLayout02ViewCreate();
	engineLayout02ViewUtilPositionSetLtRtTpBm(this->viewRoot, 0, 0, 0, 0);

	// フレーム作成
	struct engineLayout02ViewPartsFrame *viewTestFrame = engineLayout02ViewPartsFrameCreate();
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)viewTestFrame, 10, 10, 10, 10);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)this->viewRoot, (struct engineLayout02View*)viewTestFrame);

	// ボタン作成
	this->viewTestButton = engineLayout02ViewPartsButtonBoxCreateText("テストボタン");
	engineLayout02ViewUtilPositionSetBottom((struct engineLayout02View*)this->viewTestButton, 10);
	engineLayout02ViewUtilPositionSetWidth((struct engineLayout02View*)this->viewTestButton, 100);
	engineLayout02ViewUtilPositionSetHeight((struct engineLayout02View*)this->viewTestButton, 30);
	engineLayout02ViewUtilPositionSetCenteringHorizontal((struct engineLayout02View*)this->viewTestButton);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)viewTestFrame, (struct engineLayout02View*)this->viewTestButton);

	// スクローラマスク作成
	struct engineLayout02ViewPartsMaskRect *viewTestScrollerMask = engineLayout02ViewPartsMaskRectCreate();
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)viewTestScrollerMask, 10, 10, 10, 100);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)viewTestFrame, (struct engineLayout02View*)viewTestScrollerMask);
	// スクローラ作成
	struct engineLayout02ViewPartsScroller *viewTestScroller = engineLayout02ViewPartsScrollerCreate();
	engineLayout02ViewUtilPositionSetLtRtTpBm((struct engineLayout02View*)viewTestScroller, 0, 0, 0, 0);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)viewTestScrollerMask, (struct engineLayout02View*)viewTestScroller);
	viewTestScroller->inner.w = 1000;
	viewTestScroller->inner.h = 1000;
	// 水平軸スクロールバー作成
	struct engineLayout02ViewPartsRect *viewScrollBarX = engineLayout02ViewPartsRectCreateWhite();
	engineLayout02ViewUtilPositionSetLeft((struct engineLayout02View*)viewScrollBarX, 0);
	engineLayout02ViewUtilPositionSetRight((struct engineLayout02View*)viewScrollBarX, 0);
	engineLayout02ViewUtilPositionSetBottom((struct engineLayout02View*)viewScrollBarX, 0);
	engineLayout02ViewUtilPositionSetHeight((struct engineLayout02View*)viewScrollBarX, 6);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)viewTestScroller->viewBarX, (struct engineLayout02View*)viewScrollBarX);
	// 垂直軸スクロールバー作成
	struct engineLayout02ViewPartsRect *viewScrollBarY = engineLayout02ViewPartsRectCreateWhite();
	engineLayout02ViewUtilPositionSetRight((struct engineLayout02View*)viewScrollBarY, 0);
	engineLayout02ViewUtilPositionSetTop((struct engineLayout02View*)viewScrollBarY, 0);
	engineLayout02ViewUtilPositionSetBottom((struct engineLayout02View*)viewScrollBarY, 0);
	engineLayout02ViewUtilPositionSetWidth((struct engineLayout02View*)viewScrollBarY, 6);
	engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)viewTestScroller->viewBarY, (struct engineLayout02View*)viewScrollBarY);
	// スクロール要素作成
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			struct engineLayout02ViewPartsButtonBox *viewScrollItem = engineLayout02ViewPartsButtonBoxCreateText("あ");
			engineLayout02ViewUtilPositionSetLeft((struct engineLayout02View*)viewScrollItem, 10 + 110 * j);
			engineLayout02ViewUtilPositionSetTop((struct engineLayout02View*)viewScrollItem, 10 + 110 * i);
			engineLayout02ViewUtilPositionSetWidth((struct engineLayout02View*)viewScrollItem, 100);
			engineLayout02ViewUtilPositionSetHeight((struct engineLayout02View*)viewScrollItem, 100);
			engineLayout02ViewUtilChildrenAdd((struct engineLayout02View*)viewTestScroller->viewInner, (struct engineLayout02View*)viewScrollItem);
		}
	}
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageCartridgeTest3 *this){
	// ルート基盤タッチ処理
	engineLayout02ViewUtilInteractTouchRoot((struct engineLayout02View*)this->viewRoot, false);

	// ボタン処理
	if(((struct engineLayout02View*)this->viewTestButton)->interact.status.isTriggerUp){
		((struct engineLayout02View*)this->viewTestButton)->interact.status.isTriggerUp = false;
		// ボタン押下時
		trace("press button");
	}

	// ルート計算処理
	engineLayout02ViewCalc((struct engineLayout02View*)this->viewRoot);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct pageCartridgeTest3 *this){
	engineLayout02ViewUtilPositionModeSetDraw();
	engineGraphicEngineClearAll();
	engineGraphicStencilClear();

	struct engineMathMatrix44 tempMat1;
	engineMathMat4Ortho(&tempMat1, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);

	struct engineMathVector4 tempColor1;
	tempColor1.r = 1.0;
	tempColor1.g = 1.0;
	tempColor1.b = 1.0;
	tempColor1.a = 1.0;

	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicStencilStackMaskRead();

	// ルート描画処理
	engineLayout02ViewDraw((struct engineLayout02View*)this->viewRoot, &tempMat1, &tempColor1);

	engineGraphicEngineFlush();
	engineLayout02ViewUtilPositionModeSetCalc();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest3 *this){
	engineLayout02ViewPause((struct engineLayout02View*)this->viewRoot);
}

// 破棄
static void dispose(struct pageCartridgeTest3 *this){
	engineLayout02ViewDispose((struct engineLayout02View*)this->viewRoot);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
static struct engineCartridgePage *pageTest3Create(){
	struct pageCartridgeTest3 *this = (struct pageCartridgeTest3*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTest3));

	struct engineCartridgePage *cartridge = (struct engineCartridgePage*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePage*))init;
	cartridge->calc = (void(*)(struct engineCartridgePage*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePage*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePage*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePage*))dispose;
	return cartridge;
}

// ページ状態初期化
void pageTest3PushPage(){
	engineCartridgePageManagerPush(pageTest3Create());
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

