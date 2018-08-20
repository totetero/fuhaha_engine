#include "../../library.h"
#include "../../engine/engineUtil/engineUtil.h"
#include "../../game.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	struct{
		bool isInit;
		bool isStart;
		int timeList[100];
		int timeTotal;
		int timeIndex;
		long long int timePrev;
	} frameRate;
	int frameRepeatStep;
	int frameCount;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フレームレート計算
double engineUtilFrameRateCalc(void){
	int timeNum = (int)(sizeof(localGlobal.frameRate.timeList) / sizeof(*localGlobal.frameRate.timeList));
	long long int timeCurr = engineUtilTimeGet();
	if(!localGlobal.frameRate.isInit){
		localGlobal.frameRate.isInit = true;
		localGlobal.frameRate.isStart = false;
		localGlobal.frameRate.timeTotal = 0;
		localGlobal.frameRate.timeIndex = 0;
		localGlobal.frameRate.timePrev = timeCurr;
	}
	int timeOld = localGlobal.frameRate.isStart ? localGlobal.frameRate.timeList[localGlobal.frameRate.timeIndex] : 0;
	int timeNew = (int)(timeCurr - localGlobal.frameRate.timePrev);
	localGlobal.frameRate.timeList[localGlobal.frameRate.timeIndex] = timeNew;
	localGlobal.frameRate.timeTotal += timeNew - timeOld;
	localGlobal.frameRate.timeIndex++;
	if(localGlobal.frameRate.timeIndex >= timeNum){
		localGlobal.frameRate.isStart = true;
		localGlobal.frameRate.timeIndex = 0;
	}
	localGlobal.frameRate.timePrev = timeCurr;
	return localGlobal.frameRate.isStart ? ((double)1000 * timeNum / localGlobal.frameRate.timeTotal) : -1;
}

// フレームレートから処理回数を求める
int engineUtilFrameRepeatCalc(double frameRate){
	// 対数グラフ上での中点で比較して近いFPSを求めてFPSによって定められた計算回数を返す
	// fpsの計測準備が整っていないときはとりあえず1を返す
	if(frameRate < 0){
		return 1;
	}else if(frameRate <  20 +  4.495){
		return 3;
	}else if(frameRate <  30 + 12.426){
		return 2;
	}else if(frameRate <  60 + 24.853){
		return 1;
	}else if(frameRate < 120 + 26.969){
		return (localGlobal.frameRepeatStep++ % 2 == 0) ? 1 : 0;
	}else{
		return (localGlobal.frameRepeatStep++ % 3 == 0) ? 1 : 0;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// フレームカウント取得
int engineUtilFrameCountGet(void){
	return localGlobal.frameCount;
}

// フレームカウント計算
void engineUtilFrameCountCalc(void){
	localGlobal.frameCount++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

