#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ネイティブ文字列描画構造体
struct engineLayoutOld01Font{
	struct engineLayoutOld01Trans trans;
	int faceIndex;
	int faceNum;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	struct{
		engineGraphicTextureId egoIdTexFont;
		int codeListIndex;
		int codeListLength;
		enum engineGraphicTextureType type;
		double textWidth;
		double textHeight;
	} fontInfo;
	struct{
		double x;
		double y;
		double w;
		double h;
		double size;
		int lineNum;
		int wordNum;
		int xalign;
		int yalign;
		bool shouldCreateBuffer;
	} dynamicInfo;
};

// ネイティブ文字列描画構造体 初期化
void engineLayoutOld01FontInit(struct engineLayoutOld01Font *this);
// ネイティブ文字列描画構造体 文字列設定
void engineLayoutOld01FontSet(struct engineLayoutOld01Font *this, enum pluginTextureFontSetId fontSetId, char *text);
// ネイティブ文字列描画構造体 描画
void engineLayoutOld01FontDraw(struct engineLayoutOld01Font *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// ネイティブ文字列描画構造体 破棄
void engineLayoutOld01FontDispose(struct engineLayoutOld01Font *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

