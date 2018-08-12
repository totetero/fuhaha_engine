#include "fuhahaEngine.h"
#include "pageTest2.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct pageTest2CartridgePopupImplement{
	struct pageTest2CartridgePopup super;
	struct engineLayoutViewPartsRoot *viewRoot;
	struct engineLayoutViewPartsRect *viewCover;
	struct{
		void *param;
		struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePopup *popup, void *param);
		void(*paramDispose)(void *param);
	} viewCreator;
	int result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageTest2CartridgePopupImplement *this){
	// ルート作成
	this->viewRoot = engineLayoutViewPartsRootCreate();

	// 内部要素作成
	struct engineLayoutView *viewInner = (struct engineLayoutView*)this->viewCreator.viewCreate((struct pageTest2CartridgePopup*)this, this->viewCreator.param);
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
static void calc(struct pageTest2CartridgePopupImplement *this){
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
static void draw(struct pageTest2CartridgePopupImplement *this){
	// ルート描画処理
	engineLayoutViewPartsRootDraw(this->viewRoot);

	// ポップアップ描画
	engineCartridgePopupManagerDraw(&this->super.popupManager);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageTest2CartridgePopupImplement *this){
	engineCartridgePopupManagerPause(&this->super.popupManager);
	engineLayoutViewPartsRootPause(this->viewRoot);
}

// 破棄
static void dispose(struct pageTest2CartridgePopupImplement *this){
	engineCartridgePopupManagerDispose(&this->super.popupManager);
	engineLayoutViewPartsRootDispose(this->viewRoot);
	if(this->viewCreator.paramDispose != NULL){this->viewCreator.paramDispose(this->viewCreator.param);}
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ポップアップカートリッジ構造体 作成
struct pageTest2CartridgePopup *pageTest2CartridgePopupCreate(void *param, struct engineLayoutView*(*viewCreate)(struct pageTest2CartridgePopup *popup, void *param), void(*paramDispose)(void *param)){
	struct pageTest2CartridgePopupImplement *this = (struct pageTest2CartridgePopupImplement*)engineUtilMemoryCalloc(1, sizeof(struct pageTest2CartridgePopupImplement));
	this->viewCreator.param = param;
	this->viewCreator.viewCreate = viewCreate;
	this->viewCreator.paramDispose = paramDispose;

	struct engineCartridgePopup *cartridge = (struct engineCartridgePopup*)this;
	cartridge->isExist = true;
	cartridge->init = (void(*)(struct engineCartridgePopup*))init;
	cartridge->calc = (void(*)(struct engineCartridgePopup*))calc;
	cartridge->draw = (void(*)(struct engineCartridgePopup*))draw;
	cartridge->pause = (void(*)(struct engineCartridgePopup*))pause;
	cartridge->dispose = (void(*)(struct engineCartridgePopup*))dispose;
	return (struct pageTest2CartridgePopup*)this;
}

// ポップアップカートリッジ構造体 結果設定
void pageTest2CartridgePopupResultSet(struct pageTest2CartridgePopup *that, int value){
	struct pageTest2CartridgePopupImplement *this = (struct pageTest2CartridgePopupImplement*)that;
	this->result = value;
}

// ポップアップカートリッジ構造体 結果取得
int pageTest2CartridgePopupResultGet(struct engineCartridgePopup *that){
	struct pageTest2CartridgePopupImplement *this = (struct pageTest2CartridgePopupImplement*)that;
	return this->result;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

