#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// アルファマスク シェーダー カラーフィルタ適用 使用準備
void engineGraphicShaderTextureFilterColorUse(void);

// アルファマスク シェーダー カラーフィルタ適用 カラーフィルタの設定
void engineGraphicShaderTextureFilterColorSetFilterColorAlpha(double a);
void engineGraphicShaderTextureFilterColorSetFilterColorMatrix(struct engineMathMatrix44 *matrix);
void engineGraphicShaderTextureFilterColorSetFilterColorVector(struct engineMathVector4 *vector);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

