#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest3{
	struct engineCartridgePage super;

	struct engineLayoutView *viewRoot;
	struct engineLayoutViewPartsButtonBox *viewTestButton;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest3 *this){
	// ルート作成
	this->viewRoot = engineLayoutViewCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm(this->viewRoot, 0, 0, 0, 0);

	// フレーム作成
	struct engineLayoutViewPartsFrame *viewTestFrame = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestFrame, 10, 10, 10, 10);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)viewTestFrame);

	// ボタン作成
	this->viewTestButton = engineLayoutViewPartsButtonBoxCreateText("テストボタン");
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewTestButton, 10);
	engineLayoutViewUtilPositionSetWidth((struct engineLayoutView*)this->viewTestButton, 100);
	engineLayoutViewUtilPositionSetHeight((struct engineLayoutView*)this->viewTestButton, 30);
	engineLayoutViewUtilPositionSetCenteringHorizontal((struct engineLayoutView*)this->viewTestButton);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton);

	// スクローラマスク作成
	struct engineLayoutViewPartsMaskRect *viewTestScrollerMask = engineLayoutViewPartsMaskRectCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestScrollerMask, 10, 10, 10, 100);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestScrollerMask);
	// スクローラ作成
	struct engineLayoutViewPartsScroller *viewTestScroller = engineLayoutViewPartsScrollerCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestScroller, 0, 0, 0, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScrollerMask, (struct engineLayoutView*)viewTestScroller);
	viewTestScroller->inner.w = 1000;
	viewTestScroller->inner.h = 1000;
	// 水平軸スクロールバー作成
	struct engineLayoutViewPartsRect *viewScrollBarX = engineLayoutViewPartsRectCreateWhite();
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)viewScrollBarX, 0);
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)viewScrollBarX, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)viewScrollBarX, 0);
	engineLayoutViewUtilPositionSetHeight((struct engineLayoutView*)viewScrollBarX, 6);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScroller->viewBarX, (struct engineLayoutView*)viewScrollBarX);
	// 垂直軸スクロールバー作成
	struct engineLayoutViewPartsRect *viewScrollBarY = engineLayoutViewPartsRectCreateWhite();
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetWidth((struct engineLayoutView*)viewScrollBarY, 6);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);
	// スクロール要素作成
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			struct engineLayoutViewPartsButtonBox *viewScrollItem = engineLayoutViewPartsButtonBoxCreateText("あ");
			engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)viewScrollItem, 10 + 110 * j);
			engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)viewScrollItem, 10 + 110 * i);
			engineLayoutViewUtilPositionSetWidth((struct engineLayoutView*)viewScrollItem, 100);
			engineLayoutViewUtilPositionSetHeight((struct engineLayoutView*)viewScrollItem, 100);
			engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
		}
	}
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageCartridgeTest3 *this){
	// ルート基盤タッチ処理
	engineLayoutViewUtilInteractTouchRoot((struct engineLayoutView*)this->viewRoot, false);

	// ボタン処理
	if(((struct engineLayoutView*)this->viewTestButton)->interact.status.isTriggerUp){
		((struct engineLayoutView*)this->viewTestButton)->interact.status.isTriggerUp = false;
		// ボタン押下時
		trace("press button");
	}

	// ルート計算処理
	engineLayoutViewCalc((struct engineLayoutView*)this->viewRoot);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct pageCartridgeTest3 *this){
	engineLayoutViewUtilPositionModeSetDraw();
	engineGraphicEngineClearAll();
	engineGraphicStencilClear();

	struct engineMathMatrix44 tempMat1;
	engineMathMat4Ortho(&tempMat1, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);

	struct engineMathVector4 tempColor1;
	tempColor1.r = 1.0;
	tempColor1.g = 1.0;
	tempColor1.b = 1.0;
	tempColor1.a = 1.0;

	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL);
	engineGraphicStencilStackMaskRead();

	// ルート描画処理
	engineLayoutViewDraw((struct engineLayoutView*)this->viewRoot, &tempMat1, &tempColor1);

	engineGraphicEngineFlush();
	engineLayoutViewUtilPositionModeSetCalc();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest3 *this){
	engineLayoutViewPause((struct engineLayoutView*)this->viewRoot);
}

// 破棄
static void dispose(struct pageCartridgeTest3 *this){
	engineLayoutViewDispose((struct engineLayoutView*)this->viewRoot);
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

