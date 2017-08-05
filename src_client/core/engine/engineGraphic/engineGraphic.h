#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャタイプ列挙型
enum engineGraphicTextureType{
	ENGINEGRAPHICTEXTURETYPE_LINEAR,
	ENGINEGRAPHICTEXTURETYPE_NEAREST,
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
typedef int engineGraphicTextureId;

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

// VBOID取得
bool engineGraphicObjectVBOGetGLId(engineGraphicObjectVBOId egoId, GLuint *glId);
// IBOID取得
bool engineGraphicObjectIBOGetGLId(engineGraphicObjectIBOId egoId, GLuint *glId);

// 3DオブジェクトVBO除去
void engineGraphicObjectVBODispose(engineGraphicObjectVBOId egoId);
// 3DオブジェクトIBO除去
void engineGraphicObjectIBODispose(engineGraphicObjectIBOId egoId);
// 全3Dオブジェクト除去
void engineGraphicObjectDisposeAll(void);

// 全データロード再読み込み
void engineGraphicObjectReload(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- テクスチャ

// テクスチャ作成
engineGraphicTextureId engineGraphicTextureCreateLocal(char *src, enum engineGraphicTextureType type);
engineGraphicTextureId engineGraphicTextureCreateFont(enum pluginTextureFontSetId fontSetId, char *letterList, enum engineGraphicTextureType type);

// テクスチャ読み込み完了確認
bool engineGraphicTextureIsComplete(engineGraphicTextureId egtId);

// テクスチャID取得
bool engineGraphicTextureGetGLId(engineGraphicTextureId egtId, GLuint *glId, enum engineGraphicTextureType *type);
bool engineGraphicTextureGetCodeList(engineGraphicTextureId egtId, int *codeListIndex, int *codeListLength, enum engineGraphicTextureType *type);
// テクスチャ除去
void engineGraphicTextureDispose(engineGraphicTextureId egtId);
// 全テクスチャ除去
void engineGraphicTextureDisposeAll(void);

// 全テクスチャロード再読み込み
void engineGraphicTextureReload(void);

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
void engineGraphicEngineBindTexture(engineGraphicTextureId egtId);
void engineGraphicEngineBindTextureGlId(GLuint glId, enum engineGraphicTextureType type);
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

