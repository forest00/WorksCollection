//	
//	冬期制作「Magnet Hero」
//	
//	音声再生を管理するところです
//	
//	まかべ先生から配布された"dslib"を使用しています
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/07
//	
#pragma once

#include <Windows.h>
#include "detail\sound_index.h"

//初期化
extern void sound_init(HWND window_handle);

//終了
extern void sound_end();

//楽曲を再生
extern void sound_play(sound_index_t index);

//楽曲が再生中かチェック
extern bool sound_is_play(sound_index_t index);

//ストリーム再生中の楽曲を取得
extern sound_index_t sound_get_playing_stream();

//楽曲を停止
extern void sound_stop(sound_index_t index);

