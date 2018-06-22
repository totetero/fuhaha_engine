#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsFontNumberImplement{
	struct engineLayoutViewPartsFontNumber super;

	struct engineLayoutViewPartsFontNumberBufferCompare{
		int generationCount;
	} bufferCompare;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	engineGraphicTextureId egoIdTexTest;

	int generationCount;
	struct{
		int generationCount;
		enum pluginTextureFontSetId fontSetId;
		enum engineGraphicTextureType textureType;
		struct engineLayoutViewPartsFontNumberTextCompare{
			int generationCount;
			enum pluginTextureFontSetId fontSetId;
			enum engineGraphicTextureType textureType;
		} textCompare;
	} textInfo;
	struct{
		int codeListIndex;
		int codeListLength;
		enum engineGraphicTextureType type;
	} fontInfo;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsFontNumberImplement *this){
	// レイアウト初期化
	engineLayoutViewUtilFamilyInit((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionInit((struct engineLayoutView*)this);
	engineLayoutViewUtilGraphicObjectInit((struct engineLayoutView*)this);
	this->textInfo.generationCount++;

	// デフォルトパラメータ設定
	this->super.fontStyle.size = 10;
	this->super.fontStyle.xalign = 0;
	this->super.fontStyle.yalign = 0;
	this->super.fontStyle.outline.size = 0.0;
	this->super.fontStyle.outline.quality = 8;
	this->super.fontStyle.outline.color.r = 0.0;
	this->super.fontStyle.outline.color.g = 0.0;
	this->super.fontStyle.outline.color.b = 0.0;
	this->super.fontStyle.outline.color.a = 1.0;
	this->super.color.r = 1.0;
	this->super.color.g = 1.0;
	this->super.color.b = 1.0;
	this->super.color.a = 1.0;
	this->fontInfo.codeListIndex = -1;
}

// ----------------------------------------------------------------

// タッチ処理
static bool touch(struct engineLayoutViewPartsFontNumberImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsFontNumberImplement *this){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this);
}

// ----------------------------------------------------------------

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutViewPartsFontNumberImplement *this){
	return false;
}

// バッファ作成
static void bufferCreate(struct engineLayoutViewPartsFontNumberImplement *this){
}

// プラットフォーム文字列作成
static void checkTexture(struct engineLayoutViewPartsFontNumberImplement *this){
	// プラットフォーム文字列作成 開始確認
	struct engineLayoutViewPartsFontNumberTextCompare textCompare;
	textCompare.generationCount = this->textInfo.generationCount;
	textCompare.fontSetId = this->textInfo.fontSetId;
	textCompare.textureType = this->textInfo.textureType;
	if(memcmp(&this->textInfo.textCompare, &textCompare, sizeof(struct engineLayoutViewPartsFontNumberTextCompare))){
		memcpy(&this->textInfo.textCompare, &textCompare, sizeof(struct engineLayoutViewPartsFontNumberTextCompare));
		// 一旦情報リセット
		this->fontInfo.codeListIndex = -1;
		this->fontInfo.codeListLength = 0;
		this->generationCount++;
		// 文字列作成開始
		engineGraphicTextureDispose(this->egoIdTexTest);
		this->egoIdTexTest = engineGraphicTextureCreateFont(this->textInfo.fontSetId, "0123456789", this->textInfo.textureType);
	}

	// プラットフォーム文字列作成 完了確認
	if(this->egoIdTexTest > 0 && this->fontInfo.codeListIndex < 0){
		int codeListIndex = -1;
		int codeListLength = 0;
		enum engineGraphicTextureType type = this->textInfo.textureType;
		engineGraphicTextureGetCodeList(this->egoIdTexTest, &codeListIndex, &codeListLength, &type);
		if(codeListIndex >= 0){
			this->fontInfo.codeListIndex = codeListIndex;
			this->fontInfo.codeListLength = codeListLength;
			this->fontInfo.type = type;
			this->generationCount++;
		}
	}
}

// バッファ配列作成
static void createBufferArrayText(struct engineLayoutViewPartsFontNumberImplement *this, struct pluginTextureFontCode *codeList){
	// バッファポインタ取得
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 0;

	// 頂点座標データとテクスチャ座標データを生成
	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		struct pluginTextureFontCode *codeData = &codeList[i];
		tetraNum++;
		codeData->layout.x = 0;
		codeData->layout.y = 0;
		codeData->layout.w = codeData->w / codeData->h;
		codeData->layout.h = 1;
		engineGraphicBufferPushTetraVert(codeData->layout.x, codeData->layout.y, codeData->layout.w, codeData->layout.h);
		engineGraphicBufferPushTetraTexc(codeData->imgw, codeData->imgh, codeData->x, codeData->y, codeData->w, codeData->h);
	}

	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	// バッファパラメータ記録
	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;
}

// バッファ作成
static void createBuffer(struct engineLayoutViewPartsFontNumberImplement *this){
	struct engineLayoutViewPartsFontNumberBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;

	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsFontNumberBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsFontNumberBufferCompare));

		// バッファ作成開始
		engineGraphicBufferBegin();

		// バッファ配列作成
		if(this->egoIdTexTest > 0 && this->fontInfo.codeListIndex >= 0){
			struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);
			createBufferArrayText(this, codeList);
		}else{
			this->faceIndex = 0;
			this->faceNum = 0;
		}

		// バッファ作成完了
		engineGraphicBufferEnd(&this->egoIdVert, NULL, NULL, &this->egoIdTexc, &this->egoIdFace);
	}
}

// 文字列描画
static void drawText(struct engineLayoutViewPartsFontNumberImplement *this, struct pluginTextureFontCode *codeList, struct engineMathMatrix44 *mat, double textWidth){
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineMathMat4Scale(&tempMat1, this->super.fontStyle.size, this->super.fontStyle.size, 1.0);
	engineMathMat4Translate(&tempMat1, textWidth * ((this->super.fontStyle.xalign > 0) ? 1.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 0.0), 0, 0);

	// 下の桁から描画
	int tempValue = engineMathAbs(this->super.value);
	do{
		int digit = tempValue % 10;
		struct pluginTextureFontCode *codeData = &codeList[digit];
		textWidth += codeData->layout.w;
		tempValue /= 10;
		engineMathMat4Translate(&tempMat1, -codeData->layout.w, 0, 0);
		engineGraphicEngineSetMatrix(&tempMat1);
		engineGraphicEngineBindTextureGlId(codeData->glId, this->fontInfo.type);
		engineGraphicEngineDrawIndex((this->faceIndex + digit * 2) * 3, 3 * 2);
	}while(tempValue >= 1);
}

// 描画
static void draw(struct engineLayoutViewPartsFontNumberImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 描画準備
	checkTexture(this);
	createBuffer(this);

	if(this->egoIdTexTest > 0 && this->fontInfo.codeListIndex >= 0){
		// 情報取得
		struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);

		// バッファ登録
		engineGraphicEngineBindVertVBO(this->egoIdVert);
		engineGraphicEngineBindTexcVBO(this->egoIdTexc);
		engineGraphicEngineBindFaceIBO(this->egoIdFace);
		// 行列登録準備
		double w = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
		double h = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
		struct engineMathMatrix44 tempMat1;
		struct engineMathMatrix44 tempMat2;
		engineLayoutViewUtilPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
		double x0 = w * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0);
		double y0 = (h - this->super.fontStyle.size) * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0);
		engineMathMat4Translate(&tempMat1, x0, y0, 0);
		// 色情報構造体
		struct engineMathVector4 tempColor1;

		// 桁数確認
		double textWidth = 0;
		int tempValue = engineMathAbs(this->super.value);
		do{
			int digit = tempValue % 10;
			struct pluginTextureFontCode *codeData = &codeList[digit];
			textWidth += codeData->layout.w;
			tempValue /= 10;
		}while(tempValue >= 1);

		if(this->super.fontStyle.outline.size > 0){
			// 色変更
			tempColor1.r = this->super.fontStyle.outline.color.r * color->r;
			tempColor1.g = this->super.fontStyle.outline.color.g * color->g;
			tempColor1.b = this->super.fontStyle.outline.color.b * color->b;
			tempColor1.a = this->super.fontStyle.outline.color.a * color->a;
			engineGraphicEngineSetColorVec(&tempColor1);
			// アウトライン描画
			for(int i = 0; i < this->super.fontStyle.outline.quality; i++){
				double theta = 2 * ENGINEMATH_PI * i / (double)this->super.fontStyle.outline.quality;
				double outlineX = this->super.fontStyle.outline.size * engineMathCos(theta);
				double outlineY = this->super.fontStyle.outline.size * engineMathSin(theta);
				engineMathMat4Copy(&tempMat2, &tempMat1);
				engineMathMat4Translate(&tempMat2, outlineX, outlineY, 0.0);
				drawText(this, codeList, &tempMat2, textWidth);
			}
		}

		// 色変更
		tempColor1.r = this->super.color.r * color->r;
		tempColor1.g = this->super.color.g * color->g;
		tempColor1.b = this->super.color.b * color->b;
		tempColor1.a = this->super.color.a * color->a;
		engineGraphicEngineSetColorVec(&tempColor1);
		// 文字列描画
		drawText(this, codeList, &tempMat1, textWidth);
	}

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsFontNumberImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsFontNumberImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicTextureDispose(this->egoIdTexTest);
	engineLayoutViewUtilGraphicObjectDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilPositionDispose((struct engineLayoutView*)this);
	engineLayoutViewUtilFamilyDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsFontNumber", this);
}

// ----------------------------------------------------------------

// フォント数字描画構造体 作成
struct engineLayoutViewPartsFontNumber *engineLayoutViewPartsFontNumberCreate(enum pluginTextureFontSetId fontSetId){
	struct engineLayoutViewPartsFontNumberImplement *this = (struct engineLayoutViewPartsFontNumberImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsFontNumber", 1, sizeof(struct engineLayoutViewPartsFontNumberImplement));
	init(this);
	this->textInfo.fontSetId = fontSetId;
	this->textInfo.textureType = ENGINEGRAPHICTEXTURETYPE_LINEAR;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsFontNumber*)this;
}

// フォント数字描画構造体 作成 デフォルト
struct engineLayoutViewPartsFontNumber *engineLayoutViewPartsFontNumberCreateDefault(int value){
	struct engineLayoutViewPartsFontNumber *this = engineLayoutViewPartsFontNumberCreate(PLUGINTEXTUREFONTSETID_DEFAULT);
	this->value = value;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

