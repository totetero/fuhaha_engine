#include "../engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	uint64_t x;
	uint64_t y;
	uint64_t z;
	uint64_t w;
} localGlobal = {0};

// 乱数初期化
void mathRandomInit(uint64_t seed){
	localGlobal.x = 123456789;
	localGlobal.y = 362436069;
	localGlobal.z = 521288629;
	localGlobal.w = 88675123;
	localGlobal.w ^= seed;
}

// 乱数獲得
uint64_t mathRandomGet(){
	uint64_t x = localGlobal.x;
	uint64_t w = localGlobal.w;
	uint64_t t = (x ^ (x << 11));
	localGlobal.x = localGlobal.y;
	localGlobal.y = localGlobal.z;
	localGlobal.z = localGlobal.w;
	localGlobal.w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return localGlobal.w;
}

// 乱数種取得
void mathRandomGetSeed(uint64_t seed[]){
	seed[0] = localGlobal.x;
	seed[1] = localGlobal.y;
	seed[2] = localGlobal.z;
	seed[3] = localGlobal.w;
}

// 乱数種設定
void mathRandomSetSeed(uint64_t seed[]){
	localGlobal.x = seed[0];
	localGlobal.y = seed[1];
	localGlobal.z = seed[2];
	localGlobal.w = seed[3];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

