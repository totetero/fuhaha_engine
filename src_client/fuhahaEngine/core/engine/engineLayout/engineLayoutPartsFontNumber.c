#include "fuhahaEngine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutPartsFontNumberImplement{
	struct engineLayoutPartsFontNumber super;

	struct engineLayoutPartsFontNumberBufferCompare{
		int generationCount;
	} bufferCompare;
	engineGraphicTextureId egoIdTexFont;

	int generationCount;
	struct{
		int generationCount;
		enum pluginTextureFontSetId fontSetId;
		enum engineGraphicTextureType textureType;
		struct engineLayoutPartsFontNumberTextCompare{
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
static void init(struct engineLayoutPartsFontNumberImplement *this){
	engineLayoutViewGearInit((struct engineLayoutView*)this);

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
	this->super.fontStyle.color.r = 1.0;
	this->super.fontStyle.color.g = 1.0;
	this->super.fontStyle.color.b = 1.0;
	this->super.fontStyle.color.a = 1.0;
	this->fontInfo.codeListIndex = -1;
}

// ----------------------------------------------------------------

// プラットフォーム文字列作成
static void checkTexture(struct engineLayoutPartsFontNumberImplement *this){
	// プラットフォーム文字列作成 開始確認
	struct engineLayoutPartsFontNumberTextCompare textCompare;
	textCompare.generationCount = this->textInfo.generationCount;
	textCompare.fontSetId = this->textInfo.fontSetId;
	textCompare.textureType = this->textInfo.textureType;
	if(memcmp(&this->textInfo.textCompare, &textCompare, sizeof(struct engineLayoutPartsFontNumberTextCompare))){
		memcpy(&this->textInfo.textCompare, &textCompare, sizeof(struct engineLayoutPartsFontNumberTextCompare));
		// 一旦情報リセット
		this->fontInfo.codeListIndex = -1;
		this->fontInfo.codeListLength = 0;
		this->generationCount++;
		// 文字列作成開始
		engineGraphicTextureDispose(this->egoIdTexFont);
		this->egoIdTexFont = engineGraphicTextureCreateFont(this->textInfo.fontSetId, "0123456789", this->textInfo.textureType);
	}

	// プラットフォーム文字列作成 完了確認
	if(this->egoIdTexFont > 0 && this->fontInfo.codeListIndex < 0){
		int codeListIndex = -1;
		int codeListLength = 0;
		enum engineGraphicTextureType type = this->textInfo.textureType;
		engineGraphicTextureGetCodeList(this->egoIdTexFont, &codeListIndex, &codeListLength, &type);
		if(codeListIndex >= 0){
			this->fontInfo.codeListIndex = codeListIndex;
			this->fontInfo.codeListLength = codeListLength;
			this->fontInfo.type = type;
			this->generationCount++;
		}
	}
}

// バッファ更新確認
static bool shouldBufferCreate(struct engineLayoutPartsFontNumberImplement *this){
	checkTexture(this);
	struct engineLayoutPartsFontNumberBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsFontNumberBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutPartsFontNumberBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ配列作成
static void createBufferArrayText(struct engineLayoutPartsFontNumberImplement *this, struct pluginTextureFontCode *codeList){
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
static void bufferCreate(struct engineLayoutPartsFontNumberImplement *this){
	// バッファ配列作成
	if(this->egoIdTexFont > 0 && this->fontInfo.codeListIndex >= 0){
		struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);
		createBufferArrayText(this, codeList);
	}else{
		this->faceIndex = 0;
		this->faceNum = 0;
	}
}

// 文字列描画
static void drawText(struct engineLayoutPartsFontNumberImplement *this, struct pluginTextureFontCode *codeList, struct engineMathMatrix44 *mat, double textWidth){
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
static void draw(struct engineLayoutPartsFontNumberImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	if(this->egoIdTexFont > 0 && this->fontInfo.codeListIndex >= 0){
		// 情報取得
		struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);

		// バッファ登録
		engineGraphicEngineBindVertVBO(this->super.super.graphicObject.egoIdVert);
		engineGraphicEngineBindTexcVBO(this->super.super.graphicObject.egoIdTexc);
		engineGraphicEngineBindFaceIBO(this->super.super.graphicObject.egoIdFace);
		// 行列登録準備
		double w = engineLayoutViewGearPositionGetW((struct engineLayoutView*)this);
		double h = engineLayoutViewGearPositionGetH((struct engineLayoutView*)this);
		struct engineMathMatrix44 tempMat1;
		struct engineMathMatrix44 tempMat2;
		engineLayoutViewGearPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
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
		tempColor1.r = this->super.fontStyle.color.r * color->r;
		tempColor1.g = this->super.fontStyle.color.g * color->g;
		tempColor1.b = this->super.fontStyle.color.b * color->b;
		tempColor1.a = this->super.fontStyle.color.a * color->a;
		engineGraphicEngineSetColorVec(&tempColor1);
		// 文字列描画
		drawText(this, codeList, &tempMat1, textWidth);
	}

	// 子要素描画
	engineLayoutViewGearChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct engineLayoutPartsFontNumberImplement *this){
	// 子要素破棄
	engineLayoutViewGearChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexFont);
	engineLayoutViewGearDispose((struct engineLayoutView*)this);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// フォント数字描画構造体 作成
struct engineLayoutPartsFontNumber *engineLayoutPartsFontNumberCreate(enum pluginTextureFontSetId fontSetId){
	struct engineLayoutPartsFontNumberImplement *this = (struct engineLayoutPartsFontNumberImplement*)engineUtilMemoryCalloc(1, sizeof(struct engineLayoutPartsFontNumberImplement));
	init(this);
	this->textInfo.fontSetId = fontSetId;
	this->textInfo.textureType = ENGINEGRAPHICTEXTURETYPE_LINEAR;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = engineLayoutViewDefaultTouch;
	view->calc = engineLayoutViewDefaultCalc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = engineLayoutViewDefaultPause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutPartsFontNumber*)this;
}

// フォント数字描画構造体 作成 デフォルト
struct engineLayoutPartsFontNumber *engineLayoutPartsFontNumberCreateDefault(int value){
	struct engineLayoutPartsFontNumber *this = engineLayoutPartsFontNumberCreate(PLUGINTEXTUREFONTSETID_DEFAULT);
	this->value = value;
	return this;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

