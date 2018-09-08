#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsRootImplement{
	struct pageTest2PartsRoot super;
	struct pageTest2Status *stat;

	int pageW;
	struct{
		struct engineLayoutView *viewPage;
		struct engineLayoutPartsButtonBox *viewButton;
	} pages[3];
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsRootImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

	// ページャーマスク作成
	struct engineLayoutPartsMaskRect *viewPagerMask = engineLayoutPartsMaskRectCreate();
	engineLayoutViewGearPositionSetLtRtTpBm((struct engineLayoutView*)viewPagerMask, 0, 0, 40, 0);
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewPagerMask);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewPagerMask);
	// ページャー作成
	this->stat->viewPager = engineLayoutPartsSwipePagerCreate();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)this->stat->viewPager);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)this->stat->viewPager);

	// ページ作成
	this->pages[0].viewPage = (struct engineLayoutView*)pageTest2PartsPagerPage1Create(this->stat);
	this->pages[1].viewPage = (struct engineLayoutView*)pageTest2PartsPagerPage2Create(this->stat);
	this->pages[2].viewPage = (struct engineLayoutView*)pageTest2PartsPagerPage3Create(this->stat);
	// ボタン作成
	this->pages[0].viewButton = engineLayoutPartsButtonBoxCreateText("1");
	this->pages[1].viewButton = engineLayoutPartsButtonBoxCreateText("2");
	this->pages[2].viewButton = engineLayoutPartsButtonBoxCreateText("3");
	for(int i = 0; i < (int)(sizeof(this->pages) / sizeof(*this->pages)); i++){
		// ページ設定
		engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->stat->viewPager->viewInner, (struct engineLayoutView*)this->pages[i].viewPage);
		engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->stat->viewPager->viewInner, (struct engineLayoutView*)this->pages[i].viewPage);
		// ボタン設定
		engineLayoutViewGearPositionSetTop((struct engineLayoutView*)this->pages[i].viewButton, 0);
		engineLayoutViewGearPositionSetHeightValue((struct engineLayoutView*)this->pages[i].viewButton, 40);
		engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->pages[i].viewButton);
		engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->pages[i].viewButton);
	}
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2PartsRootImplement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewGearChildrenCalc((struct engineLayoutView*)this, isCancel);

	int pageW = engineMathFloor(engineLayoutViewGearPositionGetW((struct engineLayoutView*)this));
	if(this->pageW != pageW){
		// ページ幅が変わっていたらボタン幅変更
		this->pageW = pageW;
		int pageNum = (int)(sizeof(this->pages) / sizeof(*this->pages));
		int buttonBaseW = engineMathFloor(pageW / pageNum);
		int buttonRest = engineMathFloor(pageW - pageNum * buttonBaseW);
		int buttonPosition = 0;
		for(int i = 0; i < pageNum; i++){
			int buttonCurrW = buttonBaseW + ((i < buttonRest) ? 1 : 0);
			engineLayoutViewGearPositionSetLeft((struct engineLayoutView*)this->pages[i].viewButton, buttonPosition);
			engineLayoutViewGearPositionSetWidthValue((struct engineLayoutView*)this->pages[i].viewButton, buttonCurrW);
			buttonPosition += buttonCurrW;
		}
	}

	for(int i = 0; i < (int)(sizeof(this->pages) / sizeof(*this->pages)); i++){
		// ボタン色変更
		this->pages[i].viewButton->isSelect = (this->stat->viewPager->index == i);
		// ボタン処理 ページ遷移
		if(engineLayoutViewGearInteractIsTriggerUp((struct engineLayoutView*)this->pages[i].viewButton)){this->stat->viewPager->index = i;}
	}
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct pageTest2PartsRootImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// ページ状態破棄
	pageTest2StatusDispose(this->stat);

	// 自要素破棄
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページルート構造体 作成
struct pageTest2PartsRoot *pageTest2PartsRootCreate(struct engineCartridgeLayoutPage *cartridge){
	struct pageTest2PartsRootImplement *this = (struct pageTest2PartsRootImplement*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2PartsRootImplement));
	this->stat = pageTest2StatusCreate();
	this->stat->cartridge = cartridge;
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = engineLayoutViewDefaultDraw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = engineLayoutViewGearGraphicObjectDefaultShouldBufferCreate;
	view->graphicObject.bufferCreate = engineLayoutViewGearGraphicObjectDefaultBufferCreate;
	return (struct pageTest2PartsRoot*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

