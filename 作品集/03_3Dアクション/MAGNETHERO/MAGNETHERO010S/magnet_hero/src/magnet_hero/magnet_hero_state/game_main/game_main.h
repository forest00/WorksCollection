#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	チームロゴを出します
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	
//	更新履歴
//	2018/12/10...ファイルを作成
//	

#include "..\..\magnet_hero_state.h"

extern void game_main_init();
extern void game_main_end();
extern void game_main_update(magnet_hero_state_t *next_state);
extern void game_main_draw();
