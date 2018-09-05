#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineCartridgeLayoutPageImplement{
	struct engineCartridgeLayoutPage super;
	struct engineLayoutPartsRoot *viewRoot;
	struct engineLayoutPartsRect *viewCover;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineCartridgeLayoutPageImplement *this){
	// ポップアップマネージャ作成
	engineCartridgePopupManagerInit(&this->super.popupManager);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct engineCartridgeLayoutPageImplement *this){
	// ポップアップ処理
	bool isPopup = engineCartridgePopupManagerCalc(&this->super.popupManager);

	if(this->viewRoot != NULL){
		// ポップアップ背景
		this->viewCover->super.family.isInvisible = !isPopup;
		if(isPopup){engineMathVec4Set(&this->viewCover->super.color, 0.0, 0.0, 0.0, this->super.popupManager.backAlpha);}

		// ルートタッチ処理
		engineLayoutPartsRootTouch(this->viewRoot, isPopup);

		// ルート計算処理
		engineLayoutPartsRootCalc(this->viewRoot, isPopup);
	}
}

// ----------------------------------------------------------------

// 描画
static void draw(struct engineCartridgeLayoutPageImplement *this){
	engineGraphicEngineViewport(0, 0, global.window.w, global.window.h);
	engineGraphicEngineClearAll();
	engineGraphicStencilClear();

	// ルート描画処理
	if(this->viewRoot != NULL){engineLayoutPartsRootDraw(this->viewRoot);}

	// ポップアップ描画
	engineCartridgePopupManagerDraw(&this->super.popupManager);

	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineCartridgeLayoutPageImplement *this){
	engineCartridgePopupManagerPause(&this->super.popupManager);
	if(this->viewRoot != NULL){engineLayoutPartsRootPause(this->viewRoot);}
}

// 破棄
static void dispose(struct engineCartridgeLayoutPageImplement *this){
	engineCartridgePopupManagerDispose(&this->super.popupManager);
	if(this->viewRoot != NULL){engineLayoutPartsRootDispose(this->viewRoot);}
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ構造体 作成
struct engineCartridgeLayoutPage *engineCartridgeLayoutPageCreate(){
	struct engineCartridgeLayoutPageImplement *this = (struct engineCartridgeLayoutPageImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineCartridgeLayoutPageImplement));

	struct engineCartridgePage *cartridge = (struct engineCartridgePage*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePage*))init;
	cartridge->calc = (void(*)(struct engineCartridgePage*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePage*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePage*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePage*))dispose;
	return (struct engineCartridgeLayoutPage*)this;
}

// ページカートリッジ構造体 設定
void engineCartridgeLayoutPageSetView(struct engineCartridgeLayoutPage *that, struct engineLayoutView *view){
	struct engineCartridgeLayoutPageImplement *this = (struct engineCartridgeLayoutPageImplement*)((char*)that - offsetof(struct engineCartridgeLayoutPageImplement, super));
	if(this->viewRoot != NULL){return;}

	// ルート作成
	this->viewRoot = engineLayoutPartsRootCreate();

	// 内部要素付加
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)view);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)view);

	// ポップアップ背景作成
	this->viewCover = engineLayoutPartsRectCreateWhite();
	engineLayoutViewGearFamilyAdd((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)this->viewCover);
	engineLayoutViewGearGraphicObjectConnect((struct engineLayoutView*)this->viewRoot, (struct engineLayoutView*)this->viewCover);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

