#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// カラーマトリクスフィルタ構造体
struct engineLayoutPartsFilterColor{
	struct engineLayoutView super;
	struct engineMathMatrix44 matrix;
	struct engineMathVector4 vector;
	double alpha;
};

// カラーマトリクスフィルタ構造体 作成
struct engineLayoutPartsFilterColor *engineLayoutPartsFilterColorCreate(void);

// カラーマトリクスフィルタ構造体 色行列設定
void engineLayoutPartsFilterColorSetNegative1(struct engineLayoutPartsFilterColor *this);
void engineLayoutPartsFilterColorSetNegative2(struct engineLayoutPartsFilterColor *this);
void engineLayoutPartsFilterColorSetGrayscale(struct engineLayoutPartsFilterColor *this);
void engineLayoutPartsFilterColorSetSepia(struct engineLayoutPartsFilterColor *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

