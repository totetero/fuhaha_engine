#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// BGM識別子
typedef uint32_t pluginSoundBgmId;
// SE識別子
typedef uint32_t pluginSoundSeId;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にゲーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- ゲーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にゲーム側から呼び出す

// BGM読込
void platformPluginSoundBgmLoad(pluginSoundBgmId bgmId, char *src);
// BGM再生
void platformPluginSoundBgmPlay(pluginSoundBgmId bgmId);
// BGM音量
void platformPluginSoundBgmVolume(double volume);
// BGMトーンダウン
void platformPluginSoundBgmToneDown(double volume);

// SE読込
void platformPluginSoundSeLoad(pluginSoundSeId seId, char *src);
// SE再生
void platformPluginSoundSePlay(pluginSoundSeId seId);
// SE音量
void platformPluginSoundSeVolume(double volume);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- プラットフォーム側で実装 主にプラットフォーム側から呼び出す

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

