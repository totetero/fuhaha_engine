#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バッファ作成開始
void engineGraphicBufferBegin(void);
// バッファ作成完了
void engineGraphicBufferEnd(engineGraphicObjectVBOId *egoIdVert, engineGraphicObjectVBOId *egoIdNorm, engineGraphicObjectVBOId *egoIdClor, engineGraphicObjectVBOId *egoIdTexc, engineGraphicObjectIBOId *egoIdFace);

// 頂点座標配列に要素追加
void engineGraphicBufferPushVert(double x, double y, double z);
// 法線方向配列に要素追加
void engineGraphicBufferPushNorm(double x, double y, double z);
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

