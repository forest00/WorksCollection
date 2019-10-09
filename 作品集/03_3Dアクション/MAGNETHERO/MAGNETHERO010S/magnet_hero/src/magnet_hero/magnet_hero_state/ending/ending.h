#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	エンディングを流します
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	

#include "..\..\magnet_hero_state.h"

extern void ending_init();
extern void ending_end();
extern void ending_update(magnet_hero_state_t *next_state);
extern void ending_draw();
