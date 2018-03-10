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
struct engineLayoutViewPartsFilterColor *engineLayoutViewPartsFilterColorCreate();
// カラーマトリクスフィルタ構造体 色行列設定
void engineLayoutViewPartsFilterColorCreateSetNegative1(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorCreateSetNegative2(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorCreateSetGrayscale(struct engineLayoutViewPartsFilterColor *this);
void engineLayoutViewPartsFilterColorCreateSetSepia(struct engineLayoutViewPartsFilterColor *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

