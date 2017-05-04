#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャタイプ列挙型
enum engineGraphicObjectTexType{
	ENGINEGRAPHICOBJECTTEXTYPE_LINEAR,
	ENGINEGRAPHICOBJECTTEXTYPE_NEAREST,
};

// 描画モード列挙型
enum engineGraphicEngineModeDraw{
	ENGINEGRAPHICENGINEMODEDRAW_NORMAL,
	ENGINEGRAPHICENGINEMODEDRAW_2D,
	ENGINEGRAPHICENGINEMODEDRAW_ALPHA_ADD,
	ENGINEGRAPHICENGINEMODEDRAW_HKNW,
	ENGINEGRAPHICENGINEMODEDRAW_SPHERE,
};

// ステンシルマスクモード列挙型
enum engineGraphicEngineModeStencil{
	ENGINEGRAPHICENGINEMODESTENCIL_NONE,
	ENGINEGRAPHICENGINEMODESTENCIL_MASK_0,
	ENGINEGRAPHICENGINEMODESTENCIL_MASK_1,
	ENGINEGRAPHICENGINEMODESTENCIL_MASK_2,
	ENGINEGRAPHICENGINEMODESTENCIL_WRITE_0,
	ENGINEGRAPHICENGINEMODESTENCIL_WRITE_1,
	ENGINEGRAPHICENGINEMODESTENCIL_WRITE_2,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ2,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1_MASK_0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1_WRITE_0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1_MASK_2,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1_WRITE_2,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_GE1,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_GE1_MASK_0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_GE1_WRITE_0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_GE1_MASK_INCR,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_GE1_WRITE_INCR,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_LE1,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_LE1_MASK_0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_LE1_WRITE_0,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_LE1_MASK_INCR,
	ENGINEGRAPHICENGINEMODESTENCIL_READ_LE1_WRITE_INCR,
};

// 3Dオブジェクト識別子
typedef int engineGraphicObjectVBOId;
typedef int engineGraphicObjectIBOId;
typedef int engineGraphicObjectTexId;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3Dエンジン初期化処理
void engineGraphicGameInit(void);
// 3Dエンジン終了処理
void engineGraphicGameExit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 3Dオブジェクト

// 3DオブジェクトVBO作成
engineGraphicObjectVBOId engineGraphicObjectVBOCreate(int length, GLfloat *vertices);
// 3DオブジェクトIBO作成
engineGraphicObjectIBOId engineGraphicObjectIBOCreate(int length, GLushort *indexes);
// 3DオブジェクトTex作成
engineGraphicObjectTexId engineGraphicObjectTexCreateLocal(char *src, enum engineGraphicObjectTexType type);
engineGraphicObjectTexId engineGraphicObjectTexCreateFont(enum pluginTextureFontSetId fontSetId, char *letterList, enum engineGraphicObjectTexType type);

// 3DオブジェクトTex読み込み完了確認
bool engineGraphicObjectTexIsComplete(engineGraphicObjectTexId egoId);

// VBOID取得
bool engineGraphicObjectVBOGetGLId(engineGraphicObjectVBOId egoId, GLuint *glId);
// IBOID取得
bool engineGraphicObjectIBOGetGLId(engineGraphicObjectIBOId egoId, GLuint *glId);
// テクスチャID取得
bool engineGraphicObjectTexGetGLId(engineGraphicObjectTexId egoId, GLuint *glId, enum engineGraphicObjectTexType *type);
bool engineGraphicObjectTexGetCodeList(engineGraphicObjectTexId egoId, int *codeListIndex, int *codeListLength, enum engineGraphicObjectTexType *type);

// 3DオブジェクトVBO除去
void engineGraphicObjectVBODispose(engineGraphicObjectVBOId egoId);
// 3DオブジェクトIBO除去
void engineGraphicObjectIBODispose(engineGraphicObjectIBOId egoId);
// 3DオブジェクトTex除去
void engineGraphicObjectTexDispose(engineGraphicObjectTexId egoId);
// 全3Dオブジェクト除去
void engineGraphicObjectDispose(void);

// 全データロード再読み込み
void engineGraphicObjectReload(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- シェーダー

extern char *externGlobal_vsh1_src;
extern char *externGlobal_fsh1_src;
extern char *externGlobal_vsh2_src;
extern char *externGlobal_fsh2_src;
extern char *externGlobal_vsh3_src;
extern char *externGlobal_fsh3_src;
extern char *externGlobal_vsh4_src;
extern char *externGlobal_fsh4_src;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 3Dエンジン

// 初期化
void engineGraphicEngineInit(void);
// 解放
void engineGraphicEngineExit(void);

// グラフィックエンジン命令 描画のクリア
void engineGraphicEngineClearAll(void);
// グラフィックエンジン命令 深度バッファのクリア
void engineGraphicEngineClearDepth(void);
// グラフィックエンジン命令 ステンシルバッファのクリア
void engineGraphicEngineClearStencil(void);
// 重複動作阻止のためのVBO状態記録をリセット
void engineGraphicEngineMemoryResetVBO(void);
// 重複動作阻止のためのIBO状態記録をリセット
void engineGraphicEngineMemoryResetIBO(void);
// 重複動作阻止のためのTex状態記録をリセット
void engineGraphicEngineMemoryResetTex(void);

// グラフィックエンジン命令 描画モード設定
void engineGraphicEngineSetDrawMode(enum engineGraphicEngineModeDraw mode);
// グラフィックエンジン命令 ステンシルマスクモード設定
void engineGraphicEngineSetStencilMode(enum engineGraphicEngineModeStencil mode);
// グラフィックエンジン命令 深度バッファを一時的に無効化
void engineGraphicEngineIgnoreDepthMode(bool isIgnore);

// グラフィックエンジン命令 テクスチャを指定
void engineGraphicEngineBindTexture(engineGraphicObjectTexId egoId);
void engineGraphicEngineBindTextureGlId(GLuint glId, enum engineGraphicObjectTexType type);
// グラフィックエンジン命令 VBO登録 頂点座標
void engineGraphicEngineBindVertVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 VBO登録 カラーrgb
void engineGraphicEngineBindClorVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 VBO登録 テクスチャ座標
void engineGraphicEngineBindTexcVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 IBO登録 頂点インデックス
void engineGraphicEngineBindFaceIBO(engineGraphicObjectIBOId egoId);

// グラフィックエンジン命令 行列の設定
void engineGraphicEngineSetMatrix(struct engineMathMatrix44 *matrix);
// グラフィックエンジン命令 色の設定
void engineGraphicEngineSetColorRgba(double r, double g, double b, double a);
void engineGraphicEngineSetColorVec(struct engineMathVector4 *color);

// グラフィックエンジン命令 頂点インデックスを元に描画
void engineGraphicEngineDrawIndex(int offset, int count);

// グラフィックエンジン命令クラス 描画確定
void engineGraphicEngineFlush(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 3Dエンジンバッファ作成支援

// バッファ作成開始
void engineGraphicBufferBegin(void);
// バッファ作成完了
void engineGraphicBufferEnd(engineGraphicObjectVBOId *egoIdVert, engineGraphicObjectVBOId *egoIdClor, engineGraphicObjectVBOId *egoIdTexc, engineGraphicObjectIBOId *egoIdFace);

// 頂点座標配列に要素追加
void engineGraphicBufferPushVert(double x, double y, double z);
// 色彩配列に要素追加
void engineGraphicBufferPushClor(double r, double g, double b);
// テクスチャ座標配列に要素追加
void engineGraphicBufferPushTexc(double u, double v);
// 頂点番号配配列に要素追加
void engineGraphicBufferPushFace(int index, int t0, int t1, int t2);

// 頂点座標配列に四角形の要素追加
void engineGraphicBufferPushTetraVert(double x, double y, double w, double h);
// テクスチャ座標配列に要素追加
void engineGraphicBufferPushTetraTexc(int imgw, int imgh, double u, double v, double w, double h);
// 頂点番号配配列に要素追加
void engineGraphicBufferPushTetraFace(int index);

// VBOバッファ配列内の位置獲得
int engineGraphicBufferVretIndexGet(void);
// IBOバッファ配列内の位置獲得
int engineGraphicBufferFaceIndexGet(void);

// バッファ片付け
void engineGraphicBufferClean(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体
struct engineGraphicTrans{
	engineGraphicObjectTexId *texture;
	engineGraphicObjectVBOId *vertVBO;
	engineGraphicObjectVBOId *clorVBO;
	engineGraphicObjectVBOId *texcVBO;
	engineGraphicObjectIBOId *faceIBO;
	struct engineGraphicTrans *parent;
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
	void (*draw)(struct engineGraphicTrans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
	void (*dispose)(struct engineGraphicTrans *this);
};

// 変形構造体 初期化
void engineGraphicTransInit(struct engineGraphicTrans *this);

// 変形構造体 拡大縮小
void engineGraphicTransSetScale(struct engineGraphicTrans *this, double x, double y, double z);
// 変形構造体 回転
void engineGraphicTransSetRotate(struct engineGraphicTrans *this, double radx, double rady, double radz);
// 変形構造体 平行移動
void engineGraphicTransSetTranslate(struct engineGraphicTrans *this, double x, double y, double z);

// 変形構造体 引数matに対して行列を掛け合わせる
void engineGraphicTransMultMatrix(struct engineGraphicTrans *this, struct engineMathMatrix44 *mat);

// 変形構造体 バッファ登録
void engineGraphicTransBindBuffer(struct engineGraphicTrans *this);
// 変形構造体 色情報登録
void engineGraphicTransBindColor(struct engineGraphicTrans *this, struct engineMathVector4 *color);
// 変形構造体 いろいろ登録
void engineGraphicTransBindAll(struct engineGraphicTrans *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);

// ----------------------------------------------------------------

// 変形管理構造体
struct engineGraphicTransManager{
	struct engineGraphicTrans **transList;
	int transLength;
};

// 変形管理構造体 リセット
void engineGraphicTransManagerReset(struct engineGraphicTransManager *this);
// 変形管理構造体 追加
void engineGraphicTransManagerPush(struct engineGraphicTransManager *this, struct engineGraphicTrans *trans);
// 変形管理構造体 追加したすべての要素を描画
void engineGraphicTransManagerDraw(struct engineGraphicTransManager *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 変形管理構造体 破棄
void engineGraphicTransManagerDispose(struct engineGraphicTransManager *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 画像描画構造体
struct engineGraphicImageRect{
	struct engineGraphicTrans trans;
	int faceIndex;
	int faceNum;
};

// 画像描画構造体 初期化
void engineGraphicImageRectInit(struct engineGraphicImageRect *this);
// 配列に画像の描画情報を追加
void engineGraphicImageRectCreateArray(struct engineGraphicImageRect *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th);
void engineGraphicImageRectCreateArrayWhite(struct engineGraphicImageRect *this, double x, double y, double w, double h);

// 画像描画構造体 描画
void engineGraphicImageRectDraw(struct engineGraphicImageRect *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 画像描画構造体 破棄
void engineGraphicImageRectDispose(struct engineGraphicImageRect *this);

// ----------------------------------------------------------------

// 枠付描画構造体
struct engineGraphicImageFrame{
	struct engineGraphicTrans trans;
	int faceIndex;
	int faceNum;
	struct{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} texture;
		struct{int bt; int bb; int br; int bl;} border;
		double scale;
	} createArrayInfo;
	double x;
	double y;
	double w;
	double h;
};

// 枠付描画構造体 初期化
void engineGraphicImageFrameInit(struct engineGraphicImageFrame *this);
// 配列に枠付の描画情報を追加
void engineGraphicImageFrameCreateArray(struct engineGraphicImageFrame *this, double x, double y, double w, double h);

// 枠付描画構造体 描画
void engineGraphicImageFrameDraw(struct engineGraphicImageFrame *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 枠付描画構造体 破棄
void engineGraphicImageFrameDispose(struct engineGraphicImageFrame *this);

// ----------------------------------------------------------------

// 文字列描画構造体
struct engineGraphicImageText{
	struct engineGraphicTrans trans;
	int faceIndex;
	int faceNum;
	struct{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} alphabet;
		struct{int tu; int tv; int tw; int th;} hiragana;
		struct{int tu; int tv; int tw; int th;} katakana;
		double scale;
		int xalign;
		int yalign;
	} createArrayInfo;
};

// 文字列描画構造体 初期化
void engineGraphicImageTextInit(struct engineGraphicImageText *this);
// 配列に文字列(utf8)の描画情報を追加
void engineGraphicImageTextCreateArray(struct engineGraphicImageText *this, double x, double y, char *text);

// 文字列描画構造体 描画
void engineGraphicImageTextDraw(struct engineGraphicImageText *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 文字列描画構造体 破棄
void engineGraphicImageTextDispose(struct engineGraphicImageText *this);

// ----------------------------------------------------------------

// 数値列描画構造体
struct engineGraphicImageNumber{
	struct engineGraphicTrans trans;
	int faceIndex;
	int faceNum;
	struct{
		int imgw;
		int imgh;
		struct{int tu; int tv; int tw; int th;} number;
		double scale;
		int xalign;
		int yalign;
	} createArrayInfo;
	double width;
	int value;
};

// 数値列描画構造体 初期化
void engineGraphicImageNumberInit(struct engineGraphicImageNumber *this);
// 配列に数値列(utf8)の描画情報を追加
void engineGraphicImageNumberCreateArray(struct engineGraphicImageNumber *this, double x, double y, int number);

// 数値列描画構造体 描画
void engineGraphicImageNumberDraw(struct engineGraphicImageNumber *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// 数値列描画構造体 破棄
void engineGraphicImageNumberDispose(struct engineGraphicImageNumber *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ネイティブ文字列描画構造体
struct engineGraphicFont{
	struct engineGraphicTrans trans;
	int faceIndex;
	int faceNum;
	engineGraphicObjectVBOId egoIdVert;
	engineGraphicObjectVBOId egoIdTexc;
	engineGraphicObjectIBOId egoIdFace;
	struct{
		engineGraphicObjectTexId egoIdTexFont;
		int codeListIndex;
		int codeListLength;
		enum engineGraphicObjectTexType type;
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
void engineGraphicFontInit(struct engineGraphicFont *this);
// ネイティブ文字列描画構造体 文字列設定
void engineGraphicFontSet(struct engineGraphicFont *this, enum pluginTextureFontSetId fontSetId, char *text);
// ネイティブ文字列描画構造体 描画
void engineGraphicFontDraw(struct engineGraphicFont *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color);
// ネイティブ文字列描画構造体 破棄
void engineGraphicFontDispose(struct engineGraphicFont *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 変形構造体 情報設定
#define engineGraphicTransSetProperty(this, argParent, argZIndex) do{(this)->parent = argParent; (this)->zIndex = argZIndex;}while(false)

// 変形構造体 3Dオブジェクト設定
#define engineGraphicTransSetBuffer(this, argTexture, argVertVBO, argClorVBO, argTexcVBO, argFaceIBO) do{(this)->texture = argTexture; (this)->vertVBO = argVertVBO; (this)->clorVBO = argClorVBO; (this)->texcVBO = argTexcVBO; (this)->faceIBO = argFaceIBO;}while(false)

// 変形構造体 色の設定
#define engineGraphicTransSetColorRgba(this, r, g, b, a) engineMathVec4Set(&(this)->color, r, g, b, a)

// 変形構造体 色の設定
#define engineGraphicTransSetColorVec(this, color) memcpy(&(this)->color, color, sizeof(struct engineMathVector4))

// 変形管理構造体 設定を行った上で追加
#define engineGraphicTransManagerPushProperty(this, trans, parent, zIndex) do{engineGraphicTransManagerPush(this, trans); engineGraphicTransSetProperty(trans, parent, zIndex);}while(false)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

