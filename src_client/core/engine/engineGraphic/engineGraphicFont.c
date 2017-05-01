#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../engineMath/engineMath.h"
#include "engineGraphic.h"

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

// 描画
static void draw(struct engineGraphicTrans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineGraphicFont *this = (struct engineGraphicFont*)((char*)that - offsetof(struct engineGraphicFont, trans));
	engineGraphicFontDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineGraphicTrans *that){
	struct engineGraphicFont *this = (struct engineGraphicFont*)((char*)that - offsetof(struct engineGraphicFont, trans));
	engineGraphicFontDispose(this);
}

// ----------------------------------------------------------------

// ネイティブ文字列描画構造体 初期化
void engineGraphicFontInit(struct engineGraphicFont *this){
	engineGraphicTransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	this->dynamicInfo.x = 0;
	this->dynamicInfo.y = 0;
	this->dynamicInfo.w = 0;
	this->dynamicInfo.h = 0;
	this->dynamicInfo.size = 10;
	this->dynamicInfo.lineNum = 0;
	this->dynamicInfo.wordNum = 0;
	this->dynamicInfo.xalign = 0;
	this->dynamicInfo.yalign = 0;
}

// ネイティブ文字列描画構造体 文字列設定
void engineGraphicFontSet(struct engineGraphicFont *this, int fontSetId, char *text){
	// 一旦情報リセット
	engineGraphicObjectVBODispose(this->egoIdVert);
	engineGraphicObjectVBODispose(this->egoIdTexc);
	engineGraphicObjectIBODispose(this->egoIdFace);
	engineGraphicObjectTexDispose(this->fontInfo.egoIdTexFont);
	this->egoIdVert = 0;
	this->egoIdTexc = 0;
	this->egoIdFace = 0;
	this->fontInfo.egoIdTexFont = 0;
	this->fontInfo.codeListIndex = -1;
	this->fontInfo.codeListLength = 0;
	this->fontInfo.type = ENGINEGRAPHICOBJECTTEXTYPE_LINEAR;
	// 文字列設定
	if(text != NULL){this->fontInfo.egoIdTexFont = engineGraphicObjectTexCreateFont(fontSetId, text, this->fontInfo.type);}
}

// バッファ作成
static void createBuffer(struct engineGraphicFont *this){
	if(this->fontInfo.codeListIndex < 0){
		// 情報取得
		int egoId = this->fontInfo.egoIdTexFont;
		int codeListIndex = -1;
		int codeListLength = 0;
		enum engineGraphicObjectTexType type = this->fontInfo.type;
		engineGraphicObjectTexGetCodeList(egoId, &codeListIndex, &codeListLength, &type);
		if(codeListIndex < 0){return;}
		this->fontInfo.codeListIndex = codeListIndex;
		this->fontInfo.codeListLength = codeListLength;
		this->fontInfo.type = type;
		// 作成必要性の設定
		this->dynamicInfo.shouldCreateBuffer = true;
	}

	// 作成必要性の確認
	if(!this->dynamicInfo.shouldCreateBuffer){return;}
	this->dynamicInfo.shouldCreateBuffer = false;

	// 情報取得
	struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);

	// タグ処理
	double size = this->dynamicInfo.size * 1.0;
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
				else if(IS_CODE_S(code1) && IS_CODE_L(code2)){size = this->dynamicInfo.size * 1.4; codeCount = countTag4;}
				else if(IS_CODE_S(code1) && IS_CODE_M(code2)){size = this->dynamicInfo.size * 1.0; codeCount = countTag4;}
				else if(IS_CODE_S(code1) && IS_CODE_S(code2)){size = this->dynamicInfo.size * 0.7; codeCount = countTag4;}
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

	// 文字位置計算1
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
			bool isOver = (this->dynamicInfo.w > 0 && nextWidth > this->dynamicInfo.w);
			bool isProhibition = (codeData->code == CODE_JP_COMMA || codeData->code == CODE_JP_PERIOD);
			if(isOver && !isProhibition){isReturn = true;}
		}

		if(isReturn){
			int colNumNext = colIndex + 1 + 1;
			// 行数の限界を超えていたら位置計算処理を打ち切る
			if(this->dynamicInfo.lineNum > 0 && colNumNext > this->dynamicInfo.lineNum){break;}
			// 縦幅の限界を超えていたら位置計算処理を打ち切る
			double nextHeight = colNumNext * this->dynamicInfo.size;
			if(this->dynamicInfo.h > 0 && nextHeight > this->dynamicInfo.h){break;}
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
	int colNum = colIndex + 1;
	double colHeight = colNum * this->dynamicInfo.size;
	this->fontInfo.textWidth = rowWidthMax;
	this->fontInfo.textHeight = colHeight;

	// 文字位置計算2
	for(int i = 0; i < colNum; i++){
		rowWidth = 0;
		if(this->dynamicInfo.xalign <= 0){
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
			double row = rowPosition;
			double col = i * this->dynamicInfo.size + (this->dynamicInfo.size - codeData->layout.h) * 0.5;
			codeData->layout.x = row + this->dynamicInfo.x + (this->dynamicInfo.w - rowWidth) * ((this->dynamicInfo.xalign > 0) ? 0.0 : (this->dynamicInfo.xalign == 0) ? 0.5 : 1.0);
			codeData->layout.y = col + this->dynamicInfo.y + (this->dynamicInfo.h - colHeight) * ((this->dynamicInfo.yalign > 0) ? 0.0 : (this->dynamicInfo.yalign == 0) ? 0.5 : 1.0);
			rowPosition += codeData->layout.w;
		}
	}

	// バッファ作成開始
	engineGraphicBufferBegin();

	int vertIndex = engineGraphicBufferVretIndexGet();
	int faceIndex = engineGraphicBufferFaceIndexGet();
	int tetraNum = 0;

	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		struct pluginTextureFontCode *codeData = &codeList[i];
		if(codeData->layout.colIndex < 0){continue;}
		tetraNum++;
		// 頂点座標データを生成
		engineGraphicBufferPushTetraVert(codeData->layout.x, codeData->layout.y, codeData->layout.w, codeData->layout.h);
		// テクスチャ座標データを生成
		engineGraphicBufferPushTetraTexc(codeData->imgw, codeData->imgh, codeData->x, codeData->y, codeData->w, codeData->h);
	}
	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}

	this->faceIndex = faceIndex;
	this->faceNum = tetraNum * 2;

	// バッファ作成完了
	engineGraphicBufferEnd(&this->egoIdVert, NULL, &this->egoIdTexc, &this->egoIdFace);
}

// ネイティブ文字列描画構造体 描画
void engineGraphicFontDraw(struct engineGraphicFont *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	// 描画準備
	createBuffer(this);
	if(this->fontInfo.codeListIndex < 0){return;}
	struct pluginTextureFontCode *codeList = corePluginTextureFontCodeListGet(this->fontInfo.codeListIndex);

	// バッファ登録
	engineGraphicEngineBindVertVBO(this->egoIdVert);
	engineGraphicEngineBindTexcVBO(this->egoIdTexc);
	engineGraphicEngineBindFaceIBO(this->egoIdFace);
	// 行列登録
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineGraphicTransMultMatrix(&this->trans, &tempMat1);
	engineGraphicEngineSetMatrix(&tempMat1);

	// 描画
	int index = 0;
	uint32_t tagColor = 0x00000000;
	for(int i = 0; i < this->fontInfo.codeListLength; i++){
		struct pluginTextureFontCode *codeData = &codeList[i];
		if(codeData->layout.colIndex < 0){continue;}
		if(tagColor != codeData->tag.color){
			// 色変更
			tagColor = codeData->tag.color;
			struct engineMathVector4 newColor;
			newColor.r = color->r * (double)((tagColor >> 24) & 0xff) / 0xff;
			newColor.g = color->g * (double)((tagColor >> 16) & 0xff) / 0xff;
			newColor.b = color->b * (double)((tagColor >>  8) & 0xff) / 0xff;
			newColor.a = color->a * (double)((tagColor >>  0) & 0xff) / 0xff;
			engineGraphicTransBindColor(&this->trans, &newColor);
		}
		engineGraphicEngineBindTextureGlId(codeList[i].glId, this->fontInfo.type);
		engineGraphicEngineDrawIndex((this->faceIndex + (index++) * 2) * 3, 3 * 2);
		if(this->dynamicInfo.wordNum > 0 && index >= this->dynamicInfo.wordNum){break;}
	}
}

// ネイティブ文字列描画構造体 破棄
void engineGraphicFontDispose(struct engineGraphicFont *this){
	engineGraphicFontSet(this, -1, NULL);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

