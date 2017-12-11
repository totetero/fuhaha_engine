#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体
struct engineLayout01Trans{
	engineGraphicTextureId *texture;
	engineGraphicObjectVBOId *vertVBO;
	engineGraphicObjectVBOId *clorVBO;
	engineGraphicObjectVBOId *texcVBO;
	engineGraphicObjectIBOId *faceIBO;
	struct engineLayout01Trans *parent;
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
	void (*draw)(struct engineLayout01Trans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*dispose)(struct engineLayout01Trans *this);
};

// 変形構造体 初期化
void engineLayout01TransInit(struct engineLayout01Trans *this);

// 変形構造体 拡大縮小
void engineLayout01TransSetScale(struct engineLayout01Trans *this, double x, double y, double z);
// 変形構造体 回転
void engineLayout01TransSetRotate(struct engineLayout01Trans *this, double radx, double rady, double radz);
// 変形構造体 平行移動
void engineLayout01TransSetTranslate(struct engineLayout01Trans *this, double x, double y, double z);

// 変形構造体 引数matに対して行列を掛け合わせる
void engineLayout01TransMultMatrix(struct engineLayout01Trans *this, struct engineMathMatrix44 *mat);

// 変形構造体 バッファ登録
void engineLayout01TransBindBuffer(struct engineLayout01Trans *this);
// 変形構造体 色情報登録
void engineLayout01TransBindColor(struct engineLayout01Trans *this, struct engineMathVector4 *color);
// 変形構造体 いろいろ登録
void engineLayout01TransBindAll(struct engineLayout01Trans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);

// ----------------------------------------------------------------

// 変形管理構造体
struct engineLayout01TransManager{
	struct engineLayout01Trans **transList;
	int transLength;
};

// 変形管理構造体 リセット
void engineLayout01TransManagerReset(struct engineLayout01TransManager *this);
// 変形管理構造体 追加
void engineLayout01TransManagerPush(struct engineLayout01TransManager *this, struct engineLayout01Trans *trans);
// 変形管理構造体 追加したすべての要素を描画
void engineLayout01TransManagerDraw(struct engineLayout01TransManager *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 変形管理構造体 破棄
void engineLayout01TransManagerDispose(struct engineLayout01TransManager *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体 情報設定
#define engineLayout01TransSetProperty(this, argParent, argZIndex) do{(this)->parent = argParent; (this)->zIndex = argZIndex;}while(false)

// 変形構造体 3Dオブジェクト設定
#define engineLayout01TransSetBuffer(this, argTexture, argVertVBO, argClorVBO, argTexcVBO, argFaceIBO) do{(this)->texture = argTexture; (this)->vertVBO = argVertVBO; (this)->clorVBO = argClorVBO; (this)->texcVBO = argTexcVBO; (this)->faceIBO = argFaceIBO;}while(false)

// 変形構造体 色の設定
#define engineLayout01TransSetColorRgba(this, r, g, b, a) engineMathVec4Set(&(this)->color, r, g, b, a)

// 変形構造体 色の設定
#define engineLayout01TransSetColorVec(this, color) memcpy(&(this)->color, color, sizeof(struct engineMathVector4))

// 変形管理構造体 設定を行った上で追加
#define engineLayout01TransManagerPushProperty(this, trans, parent, zIndex) do{engineLayout01TransManagerPush(this, trans); engineLayout01TransSetProperty(trans, parent, zIndex);}while(false)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

