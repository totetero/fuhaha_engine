#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTest3{
	struct engineCartridgePage super;

	struct engineLayoutViewPartsRoot *viewRoot;
	struct engineLayoutViewPartsArrowKey *viewArrowKey;
	struct engineLayoutViewPartsButtonBox *viewTestButton;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTest3 *this){
	// ルート作成
	this->viewRoot = engineLayoutViewPartsRootCreate();

	// フィルタ作成
	struct engineLayoutViewPartsFilterColor *viewTestFilter = engineLayoutViewPartsFilterColorCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestFilter, 0, 0, 0, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)viewTestFilter);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)viewTestFilter);
	//engineLayoutViewPartsFilterColorCreateSetNegative2(viewTestFilter);

	// フレーム作成
	struct engineLayoutViewPartsFrame *viewTestFrame = engineLayoutViewPartsFrameCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestFrame, 10, 10, 10, 10);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFilter, (struct engineLayoutView*)viewTestFrame);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFilter, (struct engineLayoutView*)viewTestFrame);

	// 十字キー作成
	this->viewArrowKey = engineLayoutViewPartsArrowKeyCreate();
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)this->viewArrowKey, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewArrowKey, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewArrowKey);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewArrowKey);

	// ボタン作成
	this->viewTestButton = engineLayoutViewPartsButtonBoxCreateText("テストボタン");
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)this->viewTestButton, 10);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)this->viewTestButton, 100);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)this->viewTestButton, 30);
	engineLayoutViewUtilPositionSetCenteringHorizontal((struct engineLayoutView*)this->viewTestButton);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)this->viewTestButton);

	// スクローラマスク作成
	struct engineLayoutViewPartsMaskRect *viewTestScrollerMask = engineLayoutViewPartsMaskRectCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestScrollerMask, 10, 10, 10, 144);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestScrollerMask);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestFrame, (struct engineLayoutView*)viewTestScrollerMask);
	// スクローラ作成
	struct engineLayoutViewPartsScroller *viewTestScroller = engineLayoutViewPartsScrollerCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewTestScroller, 0, 0, 0, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScrollerMask, (struct engineLayoutView*)viewTestScroller);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestScrollerMask, (struct engineLayoutView*)viewTestScroller);
	viewTestScroller->inner.w = 1000;
	viewTestScroller->inner.h = 1000;
	// 水平軸スクロールバー作成
	struct engineLayoutViewPartsRect *viewScrollBarX = engineLayoutViewPartsRectCreateWhite();
	engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)viewScrollBarX, 0);
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)viewScrollBarX, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)viewScrollBarX, 0);
	engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewScrollBarX, 6);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScroller->viewBarX, (struct engineLayoutView*)viewScrollBarX);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestScroller->viewBarX, (struct engineLayoutView*)viewScrollBarX);
	// 垂直軸スクロールバー作成
	struct engineLayoutViewPartsRect *viewScrollBarY = engineLayoutViewPartsRectCreateWhite();
	engineLayoutViewUtilPositionSetRight((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetBottom((struct engineLayoutView*)viewScrollBarY, 0);
	engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewScrollBarY, 6);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestScroller->viewBarY, (struct engineLayoutView*)viewScrollBarY);
	// スクロール要素作成
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			struct engineLayoutViewPartsButtonBox *viewScrollItem = engineLayoutViewPartsButtonBoxCreateText("あ");
			engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)viewScrollItem, 10 + 110 * j);
			engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)viewScrollItem, 10 + 110 * i);
			engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)viewScrollItem, 100);
			engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)viewScrollItem, 100);
			engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewTestScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
			engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewTestScroller->viewInner, (struct engineLayoutView*)viewScrollItem);
		}
	}
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageCartridgeTest3 *this){
	// ルートタッチ処理
	engineLayoutViewPartsRootTouch(this->viewRoot, false);

	// ボタン処理
	if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->viewTestButton)){
		trace("press button");
	}

	// ルート計算処理
	engineLayoutViewPartsRootCalc(this->viewRoot);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct pageCartridgeTest3 *this){
	engineGraphicEngineClearAll();
	engineGraphicStencilClear();

	// ルート描画処理
	engineLayoutViewPartsRootDraw(this->viewRoot);

	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTest3 *this){
	engineLayoutViewPartsRootPause(this->viewRoot);
}

// 破棄
static void dispose(struct pageCartridgeTest3 *this){
	engineLayoutViewPartsRootDispose(this->viewRoot);
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

