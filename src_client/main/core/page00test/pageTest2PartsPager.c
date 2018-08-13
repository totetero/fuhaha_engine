#include "fuhahaEngine.h"
#include "./pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2PartsPagerImplement{
	struct pageTest2PartsPager super;
	struct pageTest2CartridgePage *page;
	struct pageTest2Status *stat;

	int pageW;
	struct{
		struct engineLayoutView *viewPage;
		struct engineLayoutViewPartsButtonBox *viewButton;
	} pages[3];
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2PartsPagerImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// ページャーマスク作成
	struct engineLayoutViewPartsMaskRect *viewPagerMask = engineLayoutViewPartsMaskRectCreate();
	engineLayoutViewUtilPositionSetLtRtTpBm((struct engineLayoutView*)viewPagerMask, 0, 0, 40, 0);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)viewPagerMask);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)viewPagerMask);
	// ページャー作成
	this->stat->viewPager = engineLayoutViewPartsSwipePagerCreate();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)this->stat->viewPager);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)viewPagerMask, (struct engineLayoutView*)this->stat->viewPager);

	// ページ作成
	this->pages[0].viewPage = (struct engineLayoutView*)pageTest2PartsPagerPage1Create(this->stat);
	this->pages[1].viewPage = (struct engineLayoutView*)pageTest2PartsPagerPage2Create(this->stat);
	this->pages[2].viewPage = (struct engineLayoutView*)pageTest2PartsPagerPage3Create(this->stat);
	// ボタン作成
	this->pages[0].viewButton = engineLayoutViewPartsButtonBoxCreateText("1");
	this->pages[1].viewButton = engineLayoutViewPartsButtonBoxCreateText("2");
	this->pages[2].viewButton = engineLayoutViewPartsButtonBoxCreateText("3");
	for(int i = 0; i < (int)(sizeof(this->pages) / sizeof(*this->pages)); i++){
		// ページ設定
		engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->stat->viewPager->viewInner, (struct engineLayoutView*)this->pages[i].viewPage);
		engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->stat->viewPager->viewInner, (struct engineLayoutView*)this->pages[i].viewPage);
		// ボタン設定
		engineLayoutViewUtilPositionSetTop((struct engineLayoutView*)this->pages[i].viewButton, 0);
		engineLayoutViewUtilPositionSetHeightValue((struct engineLayoutView*)this->pages[i].viewButton, 40);
		engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this, (struct engineLayoutView*)this->pages[i].viewButton);
		engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this, (struct engineLayoutView*)this->pages[i].viewButton);
	}
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct pageTest2PartsPagerImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct pageTest2PartsPagerImplement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);

	int pageW = engineMathFloor(engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this));
	if(this->pageW != pageW){
		// ページ幅が変わっていたらボタン幅変更
		this->pageW = pageW;
		int pageNum = (int)(sizeof(this->pages) / sizeof(*this->pages));
		int buttonBaseW = engineMathFloor(pageW / pageNum);
		int buttonRest = engineMathFloor(pageW - pageNum * buttonBaseW);
		int buttonPosition = 0;
		for(int i = 0; i < pageNum; i++){
			int buttonCurrW = buttonBaseW + ((i < buttonRest) ? 1 : 0);
			engineLayoutViewUtilPositionSetLeft((struct engineLayoutView*)this->pages[i].viewButton, buttonPosition);
			engineLayoutViewUtilPositionSetWidthValue((struct engineLayoutView*)this->pages[i].viewButton, buttonCurrW);
			buttonPosition += buttonCurrW;
		}
	}

	for(int i = 0; i < (int)(sizeof(this->pages) / sizeof(*this->pages)); i++){
		// ボタン色変更
		this->pages[i].viewButton->isSelect = (this->stat->viewPager->index == i);
		// ボタン処理 ページ遷移
		if(engineLayoutViewUtilInteractIsTriggerUp((struct engineLayoutView*)this->pages[i].viewButton)){this->stat->viewPager->index = i;}
	}
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct pageTest2PartsPagerImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct pageTest2PartsPagerImplement *this){
}

// 描画
static void draw(struct pageTest2PartsPagerImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2PartsPagerImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct pageTest2PartsPagerImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("pageTest2PartsPager", this);
}

// ----------------------------------------------------------------

// ページャー構造体 作成
struct pageTest2PartsPager *pageTest2PartsPagerCreate(struct pageTest2CartridgePage *page, struct pageTest2Status *stat){
	struct pageTest2PartsPagerImplement *this = (struct pageTest2PartsPagerImplement*)engineUtilMemoryInfoCalloc("pageTest2PartsPager", 1, sizeof(struct pageTest2PartsPagerImplement));
	this->page = page;
	this->stat = stat;
	init(this);

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct pageTest2PartsPager*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

