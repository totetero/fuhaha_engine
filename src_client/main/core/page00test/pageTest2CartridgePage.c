#include "fuhahaEngine.h"
#include "pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2CartridgePageImplement{
	struct pageTest2CartridgePage super;
	struct engineLayoutViewPartsRoot *viewRoot;
	struct engineLayoutViewPartsRect *viewCover;
	struct{
		void *param;
		struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePage *page, void *param);
		void(*paramDispose)(void *param);
	} viewCreator;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2CartridgePageImplement *this){
	// ルート作成
	this->viewRoot = engineLayoutViewPartsRootCreate();

	// 内部要素作成
	struct engineLayoutView *viewInner = (struct engineLayoutView*)this->viewCreator.viewCreate((struct pageTest2CartridgePage*)this, this->viewCreator.param);
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)viewInner);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)viewInner);

	// ポップアップ背景作成
	this->viewCover = engineLayoutViewPartsRectCreateWhite();
	engineLayoutViewUtilFamilyAdd((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)this->viewCover);
	engineLayoutViewUtilGraphicObjectConnect((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)this->viewCover);

	// ポップアップマネージャ作成
	engineCartridgePopupManagerInit(&this->super.popupManager);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct pageTest2CartridgePageImplement *this){
	// ポップアップ処理
	bool isPopup = engineCartridgePopupManagerCalc(&this->super.popupManager);
	// ポップアップ背景
	this->viewCover->super.family.isInvisible = !isPopup;
	if(isPopup){engineMathVec4Set(&this->viewCover->super.color, 0.0, 0.0, 0.0, this->super.popupManager.backAlpha);}

	// ルートタッチ処理
	engineLayoutViewPartsRootTouch(this->viewRoot, isPopup);

	// ルート計算処理
	engineLayoutViewPartsRootCalc(this->viewRoot, isPopup);
}

// ----------------------------------------------------------------

// 描画
static void draw(struct pageTest2CartridgePageImplement *this){
	engineGraphicEngineViewport(0, 0, global.window.w, global.window.h);
	engineGraphicEngineClearAll();
	engineGraphicStencilClear();

	// ルート描画処理
	engineLayoutViewPartsRootDraw(this->viewRoot);

	// ポップアップ描画
	engineCartridgePopupManagerDraw(&this->super.popupManager);

	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2CartridgePageImplement *this){
	engineCartridgePopupManagerPause(&this->super.popupManager);
	engineLayoutViewPartsRootPause(this->viewRoot);
}

// 破棄
static void dispose(struct pageTest2CartridgePageImplement *this){
	engineCartridgePopupManagerDispose(&this->super.popupManager);
	engineLayoutViewPartsRootDispose(this->viewRoot);
	if(this->viewCreator.paramDispose != NULL){this->viewCreator.paramDispose(this->viewCreator.param);}
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ構造体 作成
struct pageTest2CartridgePage *pageTest2CartridgePageCreate(void *param, struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePage *page, void *param), void(*paramDispose)(void *param)){
	struct pageTest2CartridgePageImplement *this = (struct pageTest2CartridgePageImplement*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2CartridgePageImplement));
	this->viewCreator.param = param;
	this->viewCreator.viewCreate = viewCreate;
	this->viewCreator.paramDispose = paramDispose;

	struct engineCartridgePage *cartridge = (struct engineCartridgePage*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePage*))init;
	cartridge->calc = (void(*)(struct engineCartridgePage*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePage*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePage*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePage*))dispose;
	return (struct pageTest2CartridgePage*)this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

