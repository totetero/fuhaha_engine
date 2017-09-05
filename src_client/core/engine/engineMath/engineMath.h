#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 数学関数

#define ENGINEMATH_PI 3.14159265359
#define ENGINEMATH_RAD2DEG (180 / ENGINEMATH_PI)
#define ENGINEMATH_DEG2RAD (ENGINEMATH_PI / 180)

#define ENGINEMATH_COS000  1.00000000000
#define ENGINEMATH_COS030  0.86602540378
#define ENGINEMATH_COS045  0.70710678118
#define ENGINEMATH_COS060  0.50000000000
#define ENGINEMATH_COS090  0.00000000000
#define ENGINEMATH_COS120 -0.50000000000
#define ENGINEMATH_COS135 -0.70710678118
#define ENGINEMATH_COS150 -0.86602540378
#define ENGINEMATH_COS180 -1.00000000000
#define ENGINEMATH_COS210 -0.86602540378
#define ENGINEMATH_COS225 -0.70710678118
#define ENGINEMATH_COS240 -0.50000000000
#define ENGINEMATH_COS270  0.00000000000
#define ENGINEMATH_COS300  0.50000000000
#define ENGINEMATH_COS315  0.70710678118
#define ENGINEMATH_COS330  0.86602540378

#define ENGINEMATH_SIN000  0.00000000000
#define ENGINEMATH_SIN030  0.50000000000
#define ENGINEMATH_SIN045  0.70710678118
#define ENGINEMATH_SIN060  0.86602540378
#define ENGINEMATH_SIN090  1.00000000000
#define ENGINEMATH_SIN120  0.86602540378
#define ENGINEMATH_SIN135  0.70710678118
#define ENGINEMATH_SIN150  0.50000000000
#define ENGINEMATH_SIN180  0.00000000000
#define ENGINEMATH_SIN210 -0.50000000000
#define ENGINEMATH_SIN225 -0.70710678118
#define ENGINEMATH_SIN240 -0.86602540378
#define ENGINEMATH_SIN270 -1.00000000000
#define ENGINEMATH_SIN300 -0.86602540378
#define ENGINEMATH_SIN315 -0.70710678118
#define ENGINEMATH_SIN330 -0.50000000000

#define ENGINEMATH_SQRT2 1.41421356237
#define ENGINEMATH_SQRT3 1.73205080757

double engineMathSqrt(double x);
double engineMathCos(double x);
double engineMathSin(double x);
double engineMathTan(double x);
double engineMathAcos(double x);
double engineMathAsin(double x);
double engineMathAtan2(double y, double x);
double engineMathPow(double x, double y);
double engineMathAbs(double x);
double engineMathCeil(double x);
double engineMathRound(double x);
double engineMathFloor(double x);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 行列演算

// 行列複製
void engineMathMat4Copy(struct engineMathMatrix44 *dst, struct engineMathMatrix44 *src);
// 単位行列
void engineMathMat4Identity(struct engineMathMatrix44 *mat);
// 正射影行列
void engineMathMat4Ortho(struct engineMathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
// 透視射影行列
void engineMathMat4Frustum(struct engineMathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
void engineMathMat4Perspective(struct engineMathMatrix44 *mat, double fovy, double aspect, double near, double far);
// 行列の掛け合わせ
void engineMathMat4Multiply(struct engineMathMatrix44 *mat, struct engineMathMatrix44 *m0, struct engineMathMatrix44 *m1);
// 平行移動
void engineMathMat4Translate(struct engineMathMatrix44 *mat, double x, double y, double z);
// 拡大縮小
void engineMathMat4Scale(struct engineMathMatrix44 *mat, double x, double y, double z);
// x軸回転
void engineMathMat4RotateX(struct engineMathMatrix44 *mat, double rad);
// y軸回転
void engineMathMat4RotateY(struct engineMathMatrix44 *mat, double rad);
// z軸回転
void engineMathMat4RotateZ(struct engineMathMatrix44 *mat, double rad);
// 逆行列
void engineMathMat4Invert(struct engineMathMatrix44 *mat);

// ベクトル値設定
void engineMathVec4Set(struct engineMathVector4 *vec, double x, double y, double z, double w);
// ベクトル行列積
void engineMathVec3MultiplyMat4(struct engineMathVector3 *dst, struct engineMathVector3 *src, struct engineMathMatrix44 *mat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 乱数処理

// 乱数初期化
void engineMathRandomInit(uint64_t seed);
// 乱数獲得
uint64_t engineMathRandomGet(void);
// 乱数種取得
void engineMathRandomGetSeed(uint64_t seed[]);
// 乱数種設定
void engineMathRandomSetSeed(uint64_t seed[]);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

