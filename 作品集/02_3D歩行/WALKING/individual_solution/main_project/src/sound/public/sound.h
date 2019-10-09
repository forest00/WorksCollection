#pragma once

#include <Windows.h>
#include "sound_index.h"

//デバッグ中は音を消す(うるさいから)
#ifdef _DEBUG
#define SOUND_OFF
#endif

//初期化
extern int sound_manager_init(HWND window_handle);

//終了
extern void sound_manager_end();

//楽曲を再生
extern void sound_play(sound_index_t index);

