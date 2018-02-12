#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体
struct engineLayoutOld01Trans{
	engineGraphicTextureId *texture;
	engineGraphicObjectVBOId *vertVBO;
	engineGraphicObjectVBOId *clorVBO;
	engineGraphicObjectVBOId *texcVBO;
	engineGraphicObjectIBOId *faceIBO;
	struct engineLayoutOld01Trans *parent;
	struct engineMathVector4 color;
	// 行列変形情報
	struct engineMathVector3 scale;
	struct engineMathMatrix44 rotate;
	struct engineMathVector3 translate;
	bool isScale;
	bool isRotate;
	bool isTranslate;
	// リスト描画情報
	bool isExist;
	bool isVisible;
	int zIndex;
	void (*draw)(struct engineLayoutOld01Trans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*dispose)(struct engineLayoutOld01Trans *this);
};

// 変形構造体 初期化
void engineLayoutOld01TransInit(struct engineLayoutOld01Trans *this);

// 変形構造体 拡大縮小
void engineLayoutOld01TransSetScale(struct engineLayoutOld01Trans *this, double x, double y, double z);
// 変形構造体 回転
void engineLayoutOld01TransSetRotate(struct engineLayoutOld01Trans *this, double radx, double rady, double radz);
// 変形構造体 平行移動
void engineLayoutOld01TransSetTranslate(struct engineLayoutOld01Trans *this, double x, double y, double z);

// 変形構造体 引数matに対して行列を掛け合わせる
void engineLayoutOld01TransMultMatrix(struct engineLayoutOld01Trans *this, struct engineMathMatrix44 *mat);

// 変形構造体 バッファ登録
void engineLayoutOld01TransBindBuffer(struct engineLayoutOld01Trans *this);
// 変形構造体 色情報登録
void engineLayoutOld01TransBindColor(struct engineLayoutOld01Trans *this, struct engineMathVector4 *color);
// 変形構造体 いろいろ登録
void engineLayoutOld01TransBindAll(struct engineLayoutOld01Trans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);

// ----------------------------------------------------------------

// 変形管理構造体
struct engineLayoutOld01TransManager{
	struct engineLayoutOld01Trans **transList;
	int transLength;
};

// 変形管理構造体 リセット
void engineLayoutOld01TransManagerReset(struct engineLayoutOld01TransManager *this);
// 変形管理構造体 追加
void engineLayoutOld01TransManagerPush(struct engineLayoutOld01TransManager *this, struct engineLayoutOld01Trans *trans);
// 変形管理構造体 追加したすべての要素を描画
void engineLayoutOld01TransManagerDraw(struct engineLayoutOld01TransManager *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 変形管理構造体 破棄
void engineLayoutOld01TransManagerDispose(struct engineLayoutOld01TransManager *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体 情報設定
#define engineLayoutOld01TransSetProperty(this, argParent, argZIndex) do{(this)->parent = argParent; (this)->zIndex = argZIndex;}while(false)

// 変形構造体 3Dオブジェクト設定
#define engineLayoutOld01TransSetBuffer(this, argTexture, argVertVBO, argClorVBO, argTexcVBO, argFaceIBO) do{(this)->texture = argTexture; (this)->vertVBO = argVertVBO; (this)->clorVBO = argClorVBO; (this)->texcVBO = argTexcVBO; (this)->faceIBO = argFaceIBO;}while(false)

// 変形構造体 色の設定
#define engineLayoutOld01TransSetColorRgba(this, r, g, b, a) engineMathVec4Set(&(this)->color, r, g, b, a)

// 変形構造体 色の設定
#define engineLayoutOld01TransSetColorVec(this, color) memcpy(&(this)->color, color, sizeof(struct engineMathVector4))

// 変形管理構造体 設定を行った上で追加
#define engineLayoutOld01TransManagerPushProperty(this, trans, parent, zIndex) do{engineLayoutOld01TransManagerPush(this, trans); engineLayoutOld01TransSetProperty(trans, parent, zIndex);}while(false)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

