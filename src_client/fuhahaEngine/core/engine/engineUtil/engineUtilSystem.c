#include "../../library.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../game.h"
#include <time.h>

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 時間取得 (ミリ秒単位)
long long int engineUtilTimeGet(void){
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	return (long long int)spec.tv_sec * 1000 + (long long int)spec.tv_nsec / (1000 * 1000);
}

// ゲーム終了
void engineUtilSystemExit(void){
	platformMainSurfaceExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

