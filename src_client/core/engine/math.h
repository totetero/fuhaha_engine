#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 数学関数

#define CONST_PI 3.14159265359
#define CONST_RAD2DEG (180 / CONST_PI)
#define CONST_DEG2RAD (CONST_PI / 180)

#define CONST_COS000  1.00000000000
#define CONST_COS030  0.86602540378
#define CONST_COS045  0.70710678118
#define CONST_COS060  0.50000000000
#define CONST_COS090  0.00000000000
#define CONST_COS120 -0.50000000000
#define CONST_COS135 -0.70710678118
#define CONST_COS150 -0.86602540378
#define CONST_COS180 -1.00000000000
#define CONST_COS210 -0.86602540378
#define CONST_COS225 -0.70710678118
#define CONST_COS240 -0.50000000000
#define CONST_COS270  0.00000000000
#define CONST_COS300  0.50000000000
#define CONST_COS315  0.70710678118
#define CONST_COS330  0.86602540378

#define CONST_SIN000  0.00000000000
#define CONST_SIN030  0.50000000000
#define CONST_SIN045  0.70710678118
#define CONST_SIN060  0.86602540378
#define CONST_SIN090  1.00000000000
#define CONST_SIN120  0.86602540378
#define CONST_SIN135  0.70710678118
#define CONST_SIN150  0.50000000000
#define CONST_SIN180  0.00000000000
#define CONST_SIN210 -0.50000000000
#define CONST_SIN225 -0.70710678118
#define CONST_SIN240 -0.86602540378
#define CONST_SIN270 -1.00000000000
#define CONST_SIN300 -0.86602540378
#define CONST_SIN315 -0.70710678118
#define CONST_SIN330 -0.50000000000

#define CONST_SQRT2 1.41421356237
#define CONST_SQRT3 1.73205080757

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
// 行列演算

// ベクトル行列構造体
struct mathVector3{union{struct{double x, y, z;}; double v[3];};};
struct mathVector4{union{struct{double x, y, z, w;}; struct{double r, g, b, a;}; double v[4];};};
struct mathMatrix44{union{struct{double m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;}; double m[16];};};

void mathMat4Copy(struct mathMatrix44 *dst, struct mathMatrix44 *src);
void mathMat4Identity(struct mathMatrix44 *mat);
void mathMat4Ortho(struct mathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
void mathMat4Frustum(struct mathMatrix44 *mat, double left, double right, double bottom, double top, double near, double far);
void mathMat4Multiply(struct mathMatrix44 *mat, struct mathMatrix44 *m0, struct mathMatrix44 *m1);
void mathMat4Translate(struct mathMatrix44 *mat, double x, double y, double z);
void mathMat4Scale(struct mathMatrix44 *mat, double x, double y, double z);
void mathMat4RotateX(struct mathMatrix44 *mat, double rad);
void mathMat4RotateY(struct mathMatrix44 *mat, double rad);
void mathMat4RotateZ(struct mathMatrix44 *mat, double rad);
void mathMat4Invert(struct mathMatrix44 *mat);

void mathVec3MultiplyMat4(struct mathVector3 *dst, struct mathVector3 *src, struct mathMatrix44 *mat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 乱数処理

void mathRandomInit(uint64_t seed);
uint64_t mathRandomGet();
void mathRandomGetSeed(uint64_t seed[]);
void mathRandomSetSeed(uint64_t seed[]);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

