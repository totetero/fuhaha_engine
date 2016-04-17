#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3Dエンジン初期化処理
void e3dGameInit();
// 3Dエンジン終了処理
void e3dGameExit();
// 3Dエンジン例文
void e3dSampleCode();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// テクスチャタイプ列挙型
enum e3dTexType{
	E3DTEXTYPE_LINEAR,
	E3DTEXTYPE_NEAREST,
};

// e3d描画モード列挙型
enum e3dModeDraw{
	E3DMODEDRAW_NORMAL,
	E3DMODEDRAW_2D,
	E3DMODEDRAW_ALPHA_ADD,
	E3DMODEDRAW_HKNW,
	E3DMODEDRAW_SPHERE,
};

// ステンシルマスクモード列挙型
enum e3dModeStencil{
	E3DMODESTENCIL_NONE,
	E3DMODESTENCIL_MASK_0,
	E3DMODESTENCIL_MASK_1,
	E3DMODESTENCIL_MASK_2,
	E3DMODESTENCIL_WRITE_0,
	E3DMODESTENCIL_WRITE_1,
	E3DMODESTENCIL_WRITE_2,
	E3DMODESTENCIL_READ_EQ0,
	E3DMODESTENCIL_READ_EQ1,
	E3DMODESTENCIL_READ_EQ2,
	E3DMODESTENCIL_READ_EQ1_MASK_0,
	E3DMODESTENCIL_READ_EQ1_WRITE_0,
	E3DMODESTENCIL_READ_EQ1_MASK_2,
	E3DMODESTENCIL_READ_EQ1_WRITE_2,
	E3DMODESTENCIL_READ_GE1,
	E3DMODESTENCIL_READ_GE1_MASK_0,
	E3DMODESTENCIL_READ_GE1_WRITE_0,
	E3DMODESTENCIL_READ_GE1_MASK_INCR,
	E3DMODESTENCIL_READ_GE1_WRITE_INCR,
	E3DMODESTENCIL_READ_LE1,
	E3DMODESTENCIL_READ_LE1_MASK_0,
	E3DMODESTENCIL_READ_LE1_WRITE_0,
	E3DMODESTENCIL_READ_LE1_MASK_INCR,
	E3DMODESTENCIL_READ_LE1_WRITE_INCR,
};

// 3Dオブジェクト識別子
typedef uint32_t e3dObjectVBOId;
typedef uint32_t e3dObjectIBOId;
typedef uint32_t e3dObjectTexId;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 3Dオブジェクト

// 3DオブジェクトVBO作成
e3dObjectVBOId e3dObjectVBOCreate(uint32_t size, double *vertices);
// 3DオブジェクトIBO作成
e3dObjectIBOId e3dObjectIBOCreate(uint32_t size, uint16_t *indexes);
// 3DオブジェクトTex作成
e3dObjectTexId e3dObjectTexCreate(char *src, enum e3dTexType type);

// VBOID取得
bool e3dObjectVBOGetGLId(e3dObjectVBOId e3dId, GLuint *glId);
// IBOID取得
bool e3dObjectIBOGetGLId(e3dObjectIBOId e3dId, GLuint *glId);
// テクスチャID取得
bool e3dObjectTexGetGLId(e3dObjectTexId e3dId, GLuint *glId, enum e3dTexType *type);

// 3DオブジェクトVBO除去
void e3dObjectVBODispose(e3dObjectVBOId e3dId);
// 3DオブジェクトIBO除去
void e3dObjectIBODispose(e3dObjectIBOId e3dId);
// 3DオブジェクトTex除去
void e3dObjectTexDispose(e3dObjectTexId e3dId);
// 全3Dオブジェクト除去
void e3dObjectDispose();

// 全データロード再読み込み
void e3dObjectReload();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 3Dエンジン

// 初期化
void e3dEngineInit();
// 解放
void e3dEngineExit();

// e3d命令 描画のクリア
void e3dClearAll();
// e3d命令 深度バッファのクリア
void e3dClearDepth();
// e3d命令 ステンシルバッファのクリア
void e3dClearStencil();
// 重複動作阻止のためのVBO状態記録をリセット
void e3dMemoryResetVBO();
// 重複動作阻止のためのIBO状態記録をリセット
void e3dMemoryResetIBO();
// 重複動作阻止のためのTex状態記録をリセット
void e3dMemoryResetTex();

// e3d命令 描画モード設定
void e3dSetDrawMode(enum e3dModeDraw mode);
// e3d命令 ステンシルマスクモード設定
void e3dSetStencilMode(enum e3dModeStencil mode);
// e3d命令 深度バッファを一時的に無効化
void e3dIgnoreDepthMode(bool isIgnore);

// e3d命令 テクスチャを指定
void e3dBindTexture(e3dObjectTexId e3dId);
// e3d命令 VBO登録 頂点座標
void e3dBindVertVBO(e3dObjectVBOId e3dId);
// e3d命令 VBO登録 カラーrgb
void e3dBindClorVBO(e3dObjectVBOId e3dId);
// e3d命令 VBO登録 テクスチャ座標
void e3dBindTexcVBO(e3dObjectVBOId e3dId);
// e3d命令 IBO登録 頂点インデックス
void e3dBindFaceIBO(e3dObjectIBOId e3dId);

// e3d命令 行列の設定
void e3dSetMatrix(struct mathMatrix44 *matrix);
// e3d命令 色の設定
void e3dSetColor(double r, double g, double b, double a);

// e3d命令 頂点インデックスを元に描画
void e3dDrawIndex(uint32_t offset, uint32_t count);

// e3d命令クラス 描画確定
void e3dFlush();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 3Dエンジンバッファ作成支援

// バッファ作成開始
void e3dBufferBegin();
// バッファ作成完了
void e3dBufferEnd(e3dObjectVBOId *e3dIdVert, e3dObjectVBOId *e3dIdClor, e3dObjectVBOId *e3dIdTexc, e3dObjectIBOId *e3dIdFace);

// 頂点座標配列に要素追加
void e3dBufferPushVert(double x, double y, double z);
// 色彩配列に要素追加
void e3dBufferPushClor(double r, double g, double b);
// テクスチャ座標配列に要素追加
void e3dBufferPushTexc(double u, double v);
// 頂点番号配配列に要素追加
void e3dBufferPushFace(uint16_t index, uint16_t t0, uint16_t t1, uint16_t t2);

// 頂点座標配列に四角形の要素追加
void e3dBufferPushTetraVert(double x, double y, double w, double h);
// テクスチャ座標配列に要素追加
void e3dBufferPushTetraTexc(uint16_t imgw, uint16_t imgh, double u, double v, double w, double h);
// 頂点番号配配列に要素追加
void e3dBufferPushTetraFace(uint16_t index);

// VBOバッファ配列内の位置獲得
uint32_t e3dBufferVretIndexGet();
// IBOバッファ配列内の位置獲得
uint32_t e3dBufferFaceIndexGet();

// バッファ片付け
void e3dBufferClean();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

