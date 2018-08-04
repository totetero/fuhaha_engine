#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// カラーマトリクスフィルタ構造体
struct engineLayoutViewPartsFilterColor{
	struct engineLayoutView super;
	struct engineMathMatrix44 matrix;
	struct engineMathVector4 vector;
	double alpha;
};

// カラーマトリクスフィルタ構造体 作成
struct engineLayoutViewPartsFilterColor *engineLayoutViewPartsFilterColorCreate(void);

// カラーマトリクスフィルタ構造体 色行列設定
void engineLayoutViewPartsFilterColorSetNegative1(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorSetNegative2(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorSetGrayscale(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorSetSepia(struct engineLayoutViewPartsFilterColor *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

