#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

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

