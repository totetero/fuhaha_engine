#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

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

