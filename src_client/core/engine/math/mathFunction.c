#include "../engine.h"
#include <math.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

//double mathSqrt(double x){return sqrt(x);}
//double mathCos(double x){return cos(x);}
//double mathSin(double x){return sin(x);}
//double mathAcos(double x){return acos(x);}
//double mathAsin(double x){return asin(x);}
//double mathAtan2(double y, double x){return atan2(y, x);}
double mathAbs(double x){return fabs(x);}
double mathCeil(double x){return ceil(x);}
double mathRound(double x){return round(x);}
double mathFloor(double x){return floor(x);}

// 平方根を求める
double mathSqrt(double x){
	if(x < 0){return NAN;}
	if(x == 0){return 0;}
	// ニュートン法で求める
	double t = x * 0.5;
	for(uint32_t i = 1; i < 20; i++){
		t = (t + x / t) * 0.5;
	}
	return t;
}

// 三角関数を求める
double mathSin(double x){
	x -= (int32_t)(x / (2 * MATH_PI)) * (2 * MATH_PI);
	// テイラー展開を行う
	double sum = x;
	double t = x;
	for(uint32_t i = 1; i < 10; i++){
		t *= - (x * x) / ((2 * i + 1) * (2 * i));
		sum += t;
	}
	return sum;
}
double mathCos(double x){return mathSin(MATH_PI * 0.5 - x);}

// 逆三角関数を求める
static double mathAtan1(double x){
	if(x == 0){return 0;}
	if(isnan(x)){return x;}
	if(isinf(x)){return (x > 0 ? 0.5 : -0.5) * MATH_PI;}
	if(x >  1){return  MATH_PI * 0.5 - mathAtan1(1 / x);}
	if(x < -1){return -MATH_PI * 0.5 - mathAtan1(1 / x);}
	double a = 0;
	for(uint32_t i = 24; i >= 1; i--){
		a = (i * i * x * x) / (2 * i + 1 + a);
	}
	return x / (1 + a);
}
double mathAsin(double x){
	if(x >  1){return  MATH_PI * 0.5;}
	if(x < -1){return -MATH_PI * 0.5;}
	return mathAtan1(x / mathSqrt(1 - x * x));
}
double mathAcos(double x){
	if(x >  1){return 0;}
	if(x < -1){return MATH_PI;}
	if(x == 0){return MATH_PI * 0.5;}
	return mathAtan1(mathSqrt(1 - x * x) / x);
}
double mathAtan2(double y, double x){
	bool isInfy = isinf(y);
	bool isInfx = isinf(x);
	if(isInfy && isInfx){return (y > 0 ? (x > 0 ? 0.25 : 0.75) : (x < 0 ? -0.75 : -0.25)) * MATH_PI;}
	if(isInfy){return (y > 0 ? 0.5 : -0.5) * MATH_PI;}
	if(isInfx){return (x > 0 ? 0.0 :  1.0) * MATH_PI;}
	if(x > 0){return mathAtan1(y / x);}
	if(x < 0){return mathAtan1(y / x) + MATH_PI;}
	return (y >= 0 ? 0.5 : -0.5) * MATH_PI;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

