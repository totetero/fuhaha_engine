#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 数学関数

#define MATH_PI 3.14159265359
#define MATH_RAD2DEG (180 / MATH_PI)
#define MATH_DEG2RAD (MATH_PI / 180)

#define MATH_COS000  1.00000000000
#define MATH_COS030  0.86602540378
#define MATH_COS045  0.70710678118
#define MATH_COS060  0.50000000000
#define MATH_COS090  0.00000000000
#define MATH_COS120 -0.50000000000
#define MATH_COS135 -0.70710678118
#define MATH_COS150 -0.86602540378
#define MATH_COS180 -1.00000000000
#define MATH_COS210 -0.86602540378
#define MATH_COS225 -0.70710678118
#define MATH_COS240 -0.50000000000
#define MATH_COS270  0.00000000000
#define MATH_COS300  0.50000000000
#define MATH_COS315  0.70710678118
#define MATH_COS330  0.86602540378

#define MATH_SIN000  0.00000000000
#define MATH_SIN030  0.50000000000
#define MATH_SIN045  0.70710678118
#define MATH_SIN060  0.86602540378
#define MATH_SIN090  1.00000000000
#define MATH_SIN120  0.86602540378
#define MATH_SIN135  0.70710678118
#define MATH_SIN150  0.50000000000
#define MATH_SIN180  0.00000000000
#define MATH_SIN210 -0.50000000000
#define MATH_SIN225 -0.70710678118
#define MATH_SIN240 -0.86602540378
#define MATH_SIN270 -1.00000000000
#define MATH_SIN300 -0.86602540378
#define MATH_SIN315 -0.70710678118
#define MATH_SIN330 -0.50000000000

#define MATH_SQRT2 1.41421356237
#define MATH_SQRT3 1.73205080757

double mathSqrt(double x);
double mathCos(double x);
double mathSin(double x);
double mathAcos(double x);
double mathAsin(double x);
double mathAtan2(double y, double x);
double mathAbs(double x);
double mathCeil(double x);
double mathRound(double x);
double mathFloor(double x);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 行列演算

// ベクトル行列構造体
struct mathVector3{union{struct{double x, y, z;}; double v[3];};};
struct mathVector4{union{struct{double x, y, z, w;}; struct{double r, g, b, a;}; double v[4];};};
struct mathMatrix44{union{struct{double m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;}; double m[16];};};

// 行列複製
void mathMat4Copy(struct mathMatrix44 *dst, struct mathMatrix44 *src);
// 単位行列
void mathMat4Identity(struct mathMatrix44 *mat);
// 正射影行列
void mathMat4Ortho(struct mathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
// 透視射影行列
void mathMat4Frustum(struct mathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
// 行列の掛け合わせ
void mathMat4Multiply(struct mathMatrix44 *mat, struct mathMatrix44 *m0, struct mathMatrix44 *m1);
// 平行移動
void mathMat4Translate(struct mathMatrix44 *mat, double x, double y, double z);
// 拡大縮小
void mathMat4Scale(struct mathMatrix44 *mat, double x, double y, double z);
// x軸回転
void mathMat4RotateX(struct mathMatrix44 *mat, double rad);
// y軸回転
void mathMat4RotateY(struct mathMatrix44 *mat, double rad);
// z軸回転
void mathMat4RotateZ(struct mathMatrix44 *mat, double rad);
// 逆行列
void mathMat4Invert(struct mathMatrix44 *mat);

// ベクトル行列積
void mathVec3MultiplyMat4(struct mathVector3 *dst, struct mathVector3 *src, struct mathMatrix44 *mat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 乱数処理

// 乱数初期化
void mathRandomInit(uint64_t seed);
// 乱数獲得
uint64_t mathRandomGet();
// 乱数種取得
void mathRandomGetSeed(uint64_t seed[]);
// 乱数種設定
void mathRandomSetSeed(uint64_t seed[]);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

