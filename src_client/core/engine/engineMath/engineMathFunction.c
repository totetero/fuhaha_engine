#include "../../library.h"
#include "platform.h"
#include "engineMath.h"
#include <math.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

//double engineMathSqrt(double x){return sqrt(x);}
//double engineMathCos(double x){return cos(x);}
//double engineMathSin(double x){return sin(x);}
//double engineMathAcos(double x){return acos(x);}
//double engineMathAsin(double x){return asin(x);}
//double engineMathAtan2(double y, double x){return atan2(y, x);}
double engineMathAbs(double x){return fabs(x);}
double engineMathCeil(double x){return ceil(x);}
double engineMathRound(double x){return round(x);}
double engineMathFloor(double x){return floor(x);}

// 平方根を求める
double engineMathSqrt(double x){
	if(x < 0){return NAN;}
	if(x == 0){return 0;}
	// ニュートン法で求める
	double t = x * 0.5;
	for(int i = 1; i < 20; i++){
		t = (t + x / t) * 0.5;
	}
	return t;
}

// 三角関数を求める
double engineMathSin(double x){
	x -= (int)(x / (2 * ENGINEMATH_PI)) * (2 * ENGINEMATH_PI);
	// テイラー展開を行う
	double sum = x;
	double t = x;
	for(int i = 1; i < 10; i++){
		t *= - (x * x) / ((2 * i + 1) * (2 * i));
		sum += t;
	}
	return sum;
}
double engineMathCos(double x){return engineMathSin(ENGINEMATH_PI * 0.5 - x);}

// 逆三角関数を求める
static double engineMathAtan1(double x){
	if(x == 0){return 0;}
	if(isnan(x)){return x;}
	if(isinf(x)){return (x > 0 ? 0.5 : -0.5) * ENGINEMATH_PI;}
	if(x >  1){return  ENGINEMATH_PI * 0.5 - engineMathAtan1(1 / x);}
	if(x < -1){return -ENGINEMATH_PI * 0.5 - engineMathAtan1(1 / x);}
	double a = 0;
	for(int i = 24; i >= 1; i--){
		a = (i * i * x * x) / (2 * i + 1 + a);
	}
	return x / (1 + a);
}
double engineMathAsin(double x){
	if(x >  1){return  ENGINEMATH_PI * 0.5;}
	if(x < -1){return -ENGINEMATH_PI * 0.5;}
	return engineMathAtan1(x / engineMathSqrt(1 - x * x));
}
double engineMathAcos(double x){
	if(x >  1){return 0;}
	if(x < -1){return ENGINEMATH_PI;}
	if(x == 0){return ENGINEMATH_PI * 0.5;}
	return engineMathAtan1(engineMathSqrt(1 - x * x) / x);
}
double engineMathAtan2(double y, double x){
	bool isInfy = isinf(y);
	bool isInfx = isinf(x);
	if(isInfy && isInfx){return (y > 0 ? (x > 0 ? 0.25 : 0.75) : (x < 0 ? -0.75 : -0.25)) * ENGINEMATH_PI;}
	if(isInfy){return (y > 0 ? 0.5 : -0.5) * ENGINEMATH_PI;}
	if(isInfx){return (x > 0 ? 0.0 :  1.0) * ENGINEMATH_PI;}
	if(x > 0){return engineMathAtan1(y / x);}
	if(x < 0){return engineMathAtan1(y / x) + ENGINEMATH_PI;}
	return (y >= 0 ? 0.5 : -0.5) * ENGINEMATH_PI;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

