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
// 乱数処理

void mathRandomInit(uint64_t seed);
uint64_t mathRandomGet();
void mathRandomGetSeed(uint64_t seed[]);
void mathRandomSetSeed(uint64_t seed[]);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

