#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineCartridgeLayoutPopupImplement{
	struct engineCartridgeLayoutPopup super;
	struct engineLayoutPartsRoot *viewRoot;
	struct engineLayoutPartsRect *viewCover;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineCartridgeLayoutPopupImplement *this){
	// ポップアップマネージャ作成
	engineCartridgePopupManagerInit(&this->super.popupManager);
}

// ----------------------------------------------------------------

// 計算
static void calc(struct engineCartridgeLayoutPopupImplement *this){
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
static void draw(struct engineCartridgeLayoutPopupImplement *this){
	// ルート描画処理
	if(this->viewRoot != NULL){engineLayoutPartsRootDraw(this->viewRoot);}

	// ポップアップ描画
	engineCartridgePopupManagerDraw(&this->super.popupManager);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineCartridgeLayoutPopupImplement *this){
	engineCartridgePopupManagerPause(&this->super.popupManager);
	if(this->viewRoot != NULL){engineLayoutPartsRootPause(this->viewRoot);}
}

// 破棄
static void dispose(struct engineCartridgeLayoutPopupImplement *this){
	engineCartridgePopupManagerDispose(&this->super.popupManager);
	if(this->viewRoot != NULL){engineLayoutPartsRootDispose(this->viewRoot);}
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ポップアップカートリッジ構造体 作成
struct engineCartridgeLayoutPopup *engineCartridgeLayoutPopupCreate(){
	struct engineCartridgeLayoutPopupImplement *this = (struct engineCartridgeLayoutPopupImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineCartridgeLayoutPopupImplement));

	struct engineCartridgePopup *cartridge = (struct engineCartridgePopup*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePopup*))init;
	cartridge->calc = (void(*)(struct engineCartridgePopup*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePopup*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePopup*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePopup*))dispose;
	return (struct engineCartridgeLayoutPopup*)this;
}

// ポップアップカートリッジ構造体 設定
void engineCartridgeLayoutPopupSetView(struct engineCartridgeLayoutPopup *that, struct engineLayoutView *view){
	struct engineCartridgeLayoutPopupImplement *this = (struct engineCartridgeLayoutPopupImplement*)((char*)that - offsetof(struct engineCartridgeLayoutPopupImplement, super));
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

