#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画モード列挙型
enum engineGraphicEngineModeDraw{
	ENGINEGRAPHICENGINEMODEDRAW_3D,
	ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_NORMAL,
	ENGINEGRAPHICENGINEMODEDRAW_3D_ALPHA_ADD,
	ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_NORMAL,
	ENGINEGRAPHICENGINEMODEDRAW_2D_ALPHA_ADD,
	ENGINEGRAPHICENGINEMODEDRAW_2D_FILTER_COLOR_ALPHA_NORMAL,
	ENGINEGRAPHICENGINEMODEDRAW_PHONG,
	ENGINEGRAPHICENGINEMODEDRAW_HKNW,
	ENGINEGRAPHICENGINEMODEDRAW_SPHERE,
};

struct engineGraphicEngineShader{
	struct engineGraphicEngineShader *prev;
	struct engineGraphicEngineShader *next;
	bool isUse;
	void (*setup)(void);
	void (*unuse)(void);
	void (*dispose)(void);
	GLint attr_pos;
	GLint attr_nrm;
	GLint attr_col;
	GLint attr_uvc;
	GLint unif_mat_pos;
	GLint unif_mat_nrm;
	GLint unif_col;
};

// ----------------------------------------------------------------

// 初期化
void engineGraphicEngineInit(void);
// 解放
void engineGraphicEngineExit(void);

// ----------------------------------------------------------------

// ビューポート設定
void engineGraphicEngineViewport(int x, int y, int w, int h);

// シェーダー使用を宣言する
void engineGraphicEngineShaderUse(struct engineGraphicEngineShader *shader);
// シェーダー使用中確認
bool engineGraphicEngineShaderIsUse(struct engineGraphicEngineShader *shader);
// シェーダー破棄
void engineGraphicEngineShaderDispose(struct engineGraphicEngineShader *shader);

// グラフィックエンジン命令 描画のクリア
void engineGraphicEngineClearAll(void);
// グラフィックエンジン命令 深度バッファのクリア
void engineGraphicEngineClearDepth(void);

// グラフィックエンジン命令 描画モード設定
void engineGraphicEngineSetDrawMode(enum engineGraphicEngineModeDraw mode);

// グラフィックエンジン命令 深度バッファを設定
void engineGraphicEngineSetDepthMask(bool isDepthMask);
void engineGraphicEngineSetDepthTest(bool isDepthTest);
// グラフィックエンジン命令 深度バッファを一時的に無効化
void engineGraphicEngineIgnoreDepthMask(bool isIgnore);
void engineGraphicEngineIgnoreDepthTest(bool isIgnore);

// ----------------------------------------------------------------

// 重複動作阻止のためのVBO状態記録をリセット
void engineGraphicEngineMemoryResetVBO(void);
// 重複動作阻止のためのIBO状態記録をリセット
void engineGraphicEngineMemoryResetIBO(void);
// 重複動作阻止のためのTex状態記録をリセット
void engineGraphicEngineMemoryResetTex(void);
// 重複動作阻止のための色をリセット
void engineGraphicEngineMemoryResetColor(void);

// ----------------------------------------------------------------

// グラフィックエンジン命令 テクスチャを指定
void engineGraphicEngineBindTexture(engineGraphicTextureId egtId);
void engineGraphicEngineBindTextureGlId(GLuint glId, enum engineGraphicTextureType type);
// グラフィックエンジン命令 VBO登録 頂点座標
void engineGraphicEngineBindVertVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 VBO登録 法線方向
void engineGraphicEngineBindNormVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 VBO登録 カラーrgb
void engineGraphicEngineBindClorVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 VBO登録 テクスチャ座標
void engineGraphicEngineBindTexcVBO(engineGraphicObjectVBOId egoId);
// グラフィックエンジン命令 IBO登録 頂点インデックス
void engineGraphicEngineBindFaceIBO(engineGraphicObjectIBOId egoId);

// グラフィックエンジン命令 行列の設定
void engineGraphicEngineSetMatrix(struct engineMathMatrix44 *matrix);
void engineGraphicEngineSetMatrixNorm(struct engineMathMatrix44 *matrixProjection, struct engineMathMatrix44 *matrixModelView);
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

