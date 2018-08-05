#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout/engineLayout.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define CODE_TAG1 60 // '<'
#define CODE_TAG2 62 // '>'
#define CODE_RETURN 10 // '\n'
#define CODE_JP_COMMA 12289 // '、'
#define CODE_JP_PERIOD 12290 // '。'
#define IS_CODE_B(code) (code ==  66 || code ==  98)
#define IS_CODE_C(code) (code ==  67 || code ==  99)
#define IS_CODE_G(code) (code ==  71 || code == 103)
#define IS_CODE_K(code) (code ==  75 || code == 107)
#define IS_CODE_L(code) (code ==  76 || code == 108)
#define IS_CODE_M(code) (code ==  77 || code == 109)
#define IS_CODE_R(code) (code ==  82 || code == 114)
#define IS_CODE_S(code) (code ==  83 || code == 115)
#define IS_CODE_W(code) (code ==  87 || code == 119)
#define IS_CODE_Y(code) (code ==  89 || code == 121)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 構造体実体
struct engineLayoutViewPartsFontTextImplement{
	struct engineLayoutViewPartsFontText super;

	struct engineLayoutViewPartsFontTextBufferCompare{
		int generationCount;
		struct{
			double size;
			int lineNum;
			int maxWidth;
			int maxHeight;
			int xalign;
		} fontStyle;
	} bufferCompare;
	engineGraphicTextureId egoIdTexFont;

	int generationCount;
	struct{
		char *buff;
		int length;
		int generationCount;
		enum pluginTextureFontSetId fontSetId;
		enum engineGraphicTextureType textureType;
		struct engineLayoutViewPartsFontTextTextCompare{
			int generationCount;
			enum pluginTextureFontSetId fontSetId;
			enum engineGraphicTextureType textureType;
		} textCompare;
	} textInfo;
	struct{
		int codeListIndex;
		int codeListLength;
		enum engineGraphicTextureType type;
		int colNum;
		double textWidth;
		double textHeight;
	} fontInfo;

	int faceIndex;
	int faceNum;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct engineLayoutViewPartsFontTextImplement *this){
	engineLayoutViewUtilInit((struct engineLayoutView*)this);

	// デフォルトパラメータ設定
	this->super.fontStyle.size = 10;
	this->super.fontStyle.lineNum = 0;
	this->super.fontStyle.wordNum = 0;
	this->super.fontStyle.maxWidth = 0;
	this->super.fontStyle.maxHeight = 0;
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

// タッチ処理
static bool touch(struct engineLayoutViewPartsFontTextImplement *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	bool isActive = false;
	bool isActiveChild = engineLayoutViewUtilChildrenTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveChild || isActive;
	bool isActiveLocal = engineLayoutViewUtilInteractTouch((struct engineLayoutView*)this, touchIndex, x, y, dn, mv, isCancel || isActive); isActive = isActiveLocal || isActive;
	return isActive;
}

// 計算
static void calc(struct engineLayoutViewPartsFontTextImplement *this, bool isCancel){
	// 子要素計算
	engineLayoutViewUtilChildrenCalc((struct engineLayoutView*)this, isCancel);
}

// ----------------------------------------------------------------

// プラットフォーム文字列作成
static void checkTexture(struct engineLayoutViewPartsFontTextImplement *this){
	// プラットフォーム文字列作成 開始確認
	struct engineLayoutViewPartsFontTextTextCompare textCompare;
	textCompare.generationCount = this->textInfo.generationCount;
	textCompare.fontSetId = this->textInfo.fontSetId;
	textCompare.textureType = this->textInfo.textureType;
	if(memcmp(&this->textInfo.textCompare, &textCompare, sizeof(struct engineLayoutViewPartsFontTextTextCompare))){
		memcpy(&this->textInfo.textCompare, &textCompare, sizeof(struct engineLayoutViewPartsFontTextTextCompare));
		// 一旦情報リセット
		this->fontInfo.codeListIndex = -1;
		this->fontInfo.codeListLength = 0;
		this->generationCount++;
		// 文字列作成開始
		engineGraphicTextureDispose(this->egoIdTexFont);
		this->egoIdTexFont = (this->textInfo.buff != NULL) ? engineGraphicTextureCreateFont(this->textInfo.fontSetId, this->textInfo.buff, this->textInfo.textureType) : 0;
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
static bool shouldBufferCreate(struct engineLayoutViewPartsFontTextImplement *this){
	checkTexture(this);
	struct engineLayoutViewPartsFontTextBufferCompare bufferCompare;
	bufferCompare.generationCount = this->generationCount;
	bufferCompare.fontStyle.size = this->super.fontStyle.size;
	bufferCompare.fontStyle.lineNum = this->super.fontStyle.lineNum;
	bufferCompare.fontStyle.maxWidth = this->super.fontStyle.maxWidth;
	bufferCompare.fontStyle.maxHeight = this->super.fontStyle.maxHeight;
	bufferCompare.fontStyle.xalign = this->super.fontStyle.xalign;
	if(memcmp(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsFontTextBufferCompare))){
		memcpy(&this->bufferCompare, &bufferCompare, sizeof(struct engineLayoutViewPartsFontTextBufferCompare));
		return true;
	}else{
		return false;
	}
}

// バッファ配列作成準備ステップ1 タグ処理
static void prepareCreateBufferArrayText1(struct engineLayoutViewPartsFontTextImplement *this, struct pluginTextureFontCode *codeList){
	double size = this->super.fontStyle.size * 1.0;
	uint32_t tagColor = 0xFFFFFFFF;
	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		int codeCount = 1;
		bool isReturn = false;
		int code0 = codeList[i + 0].code;
		if(code0 == CODE_TAG1){
			// 4文字タグの確認
			int countTag4 = 4;
			
			if(i + countTag4 <= this->fontInfo.codeListLength && codeList[i + countTag4 - 1].code == CODE_TAG2){
				int code1 = codeList[i + 1].code;
				int code2 = codeList[i + 2].code;
				if(IS_CODE_B(code1) && IS_CODE_R(code2)){isReturn = true; codeCount = countTag4;}
				else if(IS_CODE_S(code1) && IS_CODE_L(code2)){size = this->super.fontStyle.size * 1.4; codeCount = countTag4;}
				else if(IS_CODE_S(code1) && IS_CODE_M(code2)){size = this->super.fontStyle.size * 1.0; codeCount = countTag4;}
				else if(IS_CODE_S(code1) && IS_CODE_S(code2)){size = this->super.fontStyle.size * 0.7; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_W(code2)){tagColor = 0xFFFFFFFF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_R(code2)){tagColor = 0xFF0000FF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_G(code2)){tagColor = 0x00FF00FF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_B(code2)){tagColor = 0x0000FFFF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_C(code2)){tagColor = 0x00FFFFFF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_M(code2)){tagColor = 0xFF00FFFF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_Y(code2)){tagColor = 0xFFFF00FF; codeCount = countTag4;}
				else if(IS_CODE_C(code1) && IS_CODE_K(code2)){tagColor = 0x000000FF; codeCount = countTag4;}
			}
		}else if(code0 == CODE_RETURN){
			// 改行コード
			isReturn = true;
		}

		for(int j = 0; j < codeCount; j++){
			struct pluginTextureFontCode *codeData = &codeList[i + j];
			// タグから得られた情報の設定
			codeData->tag.isTag = (codeCount > 1 || isReturn);
			codeData->tag.isReturn = (isReturn && j == 0);
			codeData->tag.color = tagColor;
			// 文字幅計算
			codeData->layout.h = codeData->tag.isTag ? 0 : size;
			codeData->layout.w = codeData->tag.isTag ? 0 : (size * codeData->w / codeData->h);
			codeData->layout.rowIndex = -1;
			codeData->layout.colIndex = -1;
		}

		// タグ分のカウントアップ
		if(codeCount > 1){i += codeCount - 1;}
	}
}

// バッファ配列作成準備ステップ2 改行と全体の大きさを計算する
static void prepareCreateBufferArrayText2(struct engineLayoutViewPartsFontTextImplement *this, struct pluginTextureFontCode *codeList){
	double rowWidth = 0;
	double rowWidthMax = 0;
	int rowIndex = 0;
	int colIndex = 0;
	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		struct pluginTextureFontCode *codeData = &codeList[i];
		bool isReturn = codeData->tag.isReturn;

		if(!isReturn && rowIndex > 0){
			// 文字はみ出しによる改行確認 禁則文字が先頭に来そうな時は改行しない
			double nextWidth = rowWidth + codeData->layout.w;
			bool isOver = (this->super.fontStyle.maxWidth > 0 && nextWidth > this->super.fontStyle.maxWidth);
			bool isProhibition = (codeData->code == CODE_JP_COMMA || codeData->code == CODE_JP_PERIOD);
			if(isOver && !isProhibition){isReturn = true;}
		}

		if(isReturn){
			int colNumNext = colIndex + 1 + 1;
			// 行数の限界を超えていたら位置計算処理を打ち切る
			if(this->super.fontStyle.lineNum > 0 && colNumNext > this->super.fontStyle.lineNum){break;}
			// 縦幅の限界を超えていたら位置計算処理を打ち切る
			double nextHeight = colNumNext * this->super.fontStyle.size;
			if(this->super.fontStyle.maxHeight > 0 && nextHeight > this->super.fontStyle.maxHeight){break;}
			// 次の行へ進む
			if(rowWidthMax < rowWidth){rowWidthMax = rowWidth;}
			rowWidth = 0;
			rowIndex = 0;
			colIndex++;
		}

		if(!codeData->tag.isTag){
			// 文字位置設定
			codeData->layout.rowIndex = rowIndex;
			codeData->layout.colIndex = colIndex;
			rowWidth += codeData->layout.w;
			rowIndex++;
		}
	}
	if(rowWidthMax < rowWidth){rowWidthMax = rowWidth;}
	this->fontInfo.colNum = colIndex + 1;
	this->fontInfo.textWidth = rowWidthMax;
	this->fontInfo.textHeight = this->fontInfo.colNum * this->super.fontStyle.size;
}

// バッファ配列作成準備ステップ3 それぞれの文字の位置を計算する
static void prepareCreateBufferArrayText3(struct engineLayoutViewPartsFontTextImplement *this, struct pluginTextureFontCode *codeList){
	for(int i = 0; i < this->fontInfo.colNum; i++){
		int rowWidth = 0;
		if(this->super.fontStyle.xalign <= 0){
			// 列の幅を調べる
			for(int j = 0; j < this->fontInfo.codeListLength; j++){
				struct pluginTextureFontCode *codeData = &codeList[j];
				if(codeData->layout.colIndex != i){continue;}
				rowWidth += codeData->layout.w;
			}
		}
		// 列の各要素位置を設定する
		double rowPosition = 0;
		for(int j = 0; j < this->fontInfo.codeListLength; j++){
			struct pluginTextureFontCode *codeData = &codeList[j];
			if(codeData->layout.colIndex != i){continue;}
			codeData->layout.x = rowPosition + (this->fontInfo.textWidth - rowWidth) * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0);
			codeData->layout.y = i * this->super.fontStyle.size + (this->super.fontStyle.size - codeData->layout.h) * 0.5;
			rowPosition += codeData->layout.w;
		}
	}
}

// バッファ配列作成
static void createBufferArrayText(struct engineLayoutViewPartsFontTextImplement *this, struct pluginTextureFontCode *codeList){
	// バッファポインタ取得
	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 0;

	// 頂点座標データとテクスチャ座標データを生成
	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		struct pluginTextureFontCode *codeData = &codeList[i];
		if(codeData->layout.colIndex < 0){continue;}
		tetraNum++;
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
static void bufferCreate(struct engineLayoutViewPartsFontTextImplement *this){
	// バッファ配列作成
	if(this->egoIdTexFont > 0 && this->fontInfo.codeListIndex >= 0){
		struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);
		prepareCreateBufferArrayText1(this, codeList);
		prepareCreateBufferArrayText2(this, codeList);
		prepareCreateBufferArrayText3(this, codeList);
		createBufferArrayText(this, codeList);
	}else{
		this->faceIndex = 0;
		this->faceNum = 0;
	}
}

// 文字列描画
static void drawText(struct engineLayoutViewPartsFontTextImplement *this, struct pluginTextureFontCode *codeList, struct engineMathVector4 *color1, struct engineMathVector4 *color2){
	int index = 0;
	uint32_t tagColor = 0x00000000;
	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		struct pluginTextureFontCode *codeData = &codeList[i];
		if(codeData->layout.colIndex < 0){continue;}
		if(index == 0 || tagColor != codeData->tag.color){
			// 色変更
			tagColor = codeData->tag.color;
			struct engineMathVector4 tempColor1;
			tempColor1.r = color1->r * color2->r * (double)((tagColor >> 24) & 0xff) / 0xff;
			tempColor1.g = color1->g * color2->g * (double)((tagColor >> 16) & 0xff) / 0xff;
			tempColor1.b = color1->b * color2->b * (double)((tagColor >>  8) & 0xff) / 0xff;
			tempColor1.a = color1->a * color2->a * (double)((tagColor >>  0) & 0xff) / 0xff;
			engineGraphicEngineSetColorVec(&tempColor1);
		}
		engineGraphicEngineBindTextureGlId(codeData->glId, this->fontInfo.type);
		engineGraphicEngineDrawIndex((this->faceIndex + (index++) * 2) * 3, 3 * 2);
		if(this->super.fontStyle.wordNum > 0 && index >= this->super.fontStyle.wordNum){break;}
	}
}

// 描画
static void draw(struct engineLayoutViewPartsFontTextImplement *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	if(this->egoIdTexFont > 0 && this->fontInfo.codeListIndex >= 0){
		// 情報取得
		struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);

		// バッファ登録
		engineGraphicEngineBindVertVBO(this->super.super.graphicObject.egoIdVert);
		engineGraphicEngineBindTexcVBO(this->super.super.graphicObject.egoIdTexc);
		engineGraphicEngineBindFaceIBO(this->super.super.graphicObject.egoIdFace);
		// 行列登録準備
		double w = engineLayoutViewUtilPositionGetW((struct engineLayoutView*)this);
		double h = engineLayoutViewUtilPositionGetH((struct engineLayoutView*)this);
		struct engineMathMatrix44 tempMat1;
		struct engineMathMatrix44 tempMat2;
		engineLayoutViewUtilPositionTransformCalcMatrix((struct engineLayoutView*)this, &tempMat1, mat);
		// 文字揃えの位置移動
		double alignX = (w - this->fontInfo.textWidth) * ((this->super.fontStyle.xalign > 0) ? 0.0 : (this->super.fontStyle.xalign == 0) ? 0.5 : 1.0);
		double alignY = (h - this->fontInfo.textHeight) * ((this->super.fontStyle.yalign > 0) ? 0.0 : (this->super.fontStyle.yalign == 0) ? 0.5 : 1.0);
		engineMathMat4Translate(&tempMat1, alignX, alignY, 0.0);

		if(this->super.fontStyle.outline.size > 0){
			// アウトライン描画
			for(int i = 0; i < this->super.fontStyle.outline.quality; i++){
				double theta = 2 * ENGINEMATH_PI * i / (double)this->super.fontStyle.outline.quality;
				double outlineX = this->super.fontStyle.outline.size * engineMathCos(theta);
				double outlineY = this->super.fontStyle.outline.size * engineMathSin(theta);
				engineMathMat4Copy(&tempMat2, &tempMat1);
				engineMathMat4Translate(&tempMat2, outlineX, outlineY, 0.0);
				engineGraphicEngineSetMatrix(&tempMat2);
				drawText(this, codeList, color, &this->super.fontStyle.outline.color);
			}
		}

		// 文字列描画
		engineGraphicEngineSetMatrix(&tempMat1);
		drawText(this, codeList, color, &this->super.fontStyle.color);
	}

	// 子要素描画
	engineLayoutViewUtilChildrenDraw((struct engineLayoutView*)this, mat, color);
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct engineLayoutViewPartsFontTextImplement *this){
	// 子要素一時停止
	engineLayoutViewUtilChildrenPause((struct engineLayoutView*)this);
}

// 破棄
static void dispose(struct engineLayoutViewPartsFontTextImplement *this){
	// 子要素破棄
	engineLayoutViewUtilChildrenDispose((struct engineLayoutView*)this);

	// 自要素破棄
	engineGraphicTextureDispose(this->egoIdTexFont);
	if(this->textInfo.buff != NULL){engineUtilMemoryInfoFree("engineLayoutViewPartsFontText textBuff", this->textInfo.buff);}
	engineLayoutViewUtilDispose((struct engineLayoutView*)this);
	engineUtilMemoryInfoFree("engineLayoutViewPartsFontText", this);
}

// ----------------------------------------------------------------

// フォント文字列描画構造体 作成
struct engineLayoutViewPartsFontText *engineLayoutViewPartsFontTextCreate(enum pluginTextureFontSetId fontSetId){
	struct engineLayoutViewPartsFontTextImplement *this = (struct engineLayoutViewPartsFontTextImplement*)engineUtilMemoryInfoCalloc("engineLayoutViewPartsFontText", 1, sizeof(struct engineLayoutViewPartsFontTextImplement));
	init(this);
	this->textInfo.fontSetId = fontSetId;
	this->textInfo.textureType = ENGINEGRAPHICTEXTURETYPE_LINEAR;

	struct engineLayoutView *view = (struct engineLayoutView*)this;
	view->touch = (bool(*)(struct engineLayoutView*, int, double, double, bool, bool, bool))touch;
	view->calc = (void(*)(struct engineLayoutView*, bool))calc;
	view->draw = (void(*)(struct engineLayoutView*, struct engineMathMatrix44*, struct engineMathVector4*))draw;
	view->pause = (void(*)(struct engineLayoutView*))pause;
	view->dispose = (void(*)(struct engineLayoutView*))dispose;
	view->graphicObject.shouldBufferCreate = (bool(*)(struct engineLayoutView*))shouldBufferCreate;
	view->graphicObject.bufferCreate = (void(*)(struct engineLayoutView*))bufferCreate;
	return (struct engineLayoutViewPartsFontText*)this;
}

// フォント文字列描画構造体 作成 デフォルト
struct engineLayoutViewPartsFontText *engineLayoutViewPartsFontTextCreateDefault(char *text){
	struct engineLayoutViewPartsFontText *this = engineLayoutViewPartsFontTextCreate(PLUGINTEXTUREFONTSETID_DEFAULT);
	engineLayoutViewPartsFontTextSet(this, text);
	return this;
}

// フォント文字列描画構造体 文字列設定
void engineLayoutViewPartsFontTextSet(struct engineLayoutViewPartsFontText *that, char *text){
	struct engineLayoutViewPartsFontTextImplement *this = (struct engineLayoutViewPartsFontTextImplement*)that;
	// 文字列領域作成
	int textLength = (text != NULL) ? ((int)strlen(text) + 1) : 0;
	if(this->textInfo.length < textLength){
		if(this->textInfo.buff != NULL){engineUtilMemoryInfoFree("engineLayoutViewPartsFontText textBuff", this->textInfo.buff);}
		this->textInfo.buff = (char*)engineUtilMemoryInfoMalloc("engineLayoutViewPartsFontText textBuff", textLength * sizeof(char));
		this->textInfo.length = textLength;
	}
	// 文字列保存
	if(this->textInfo.buff != NULL){
		if(text != NULL){
			strcpy(this->textInfo.buff, text);
		}else{
			strcpy(this->textInfo.buff, "");
		}
	}
	// 世代交代
	this->textInfo.generationCount++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

