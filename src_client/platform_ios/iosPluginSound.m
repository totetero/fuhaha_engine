#import "fuhaha-Swift.h"
#include "library.h"
#include "native.h"
#include "pluginSound.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// BGM読込
void platformPluginSoundBgmLoad(pluginSoundBgmId bgmId, char *src){
	NSString *nsstr = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginSound platformPluginSoundBgmLoad: bgmId src: nsstr];
}

// BGM再生
void platformPluginSoundBgmPlay(pluginSoundBgmId bgmId){
	[IosPluginSound platformPluginSoundBgmPlay: bgmId];
}

// BGMトーンダウン
void platformPluginSoundBgmToneDown(double volume){
	[IosPluginSound platformPluginSoundBgmToneDown: volume];
}

// BGM設定音量
void platformPluginSoundBgmVolume(double volume){
	[IosPluginSound platformPluginSoundBgmVolume: volume];
}

// ----------------------------------------------------------------

// SE読込
void platformPluginSoundSeLoad(pluginSoundSeId seId, char *src){
	NSString *nsstr = [NSString stringWithCString: src encoding: NSUTF8StringEncoding];
	[IosPluginSound platformPluginSoundSeLoad: seId src: nsstr];
}

// SE再生
void platformPluginSoundSePlay(pluginSoundSeId seId){
	[IosPluginSound platformPluginSoundSePlay: seId];
}

// SE設定音量
void platformPluginSoundSeVolume(double volume){
	[IosPluginSound platformPluginSoundSeVolume: volume];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

