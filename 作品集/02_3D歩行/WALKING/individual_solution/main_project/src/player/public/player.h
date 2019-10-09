#pragma once

#include "test_player\test_player_manager.h"

//初期化
extern void player_init();

//更新
extern void player_update();

//描画
extern void player_draw(IDirect3DDevice9 *device);

//終了
extern void player_end();


extern test_player_manager_t *player_get_test_player_manager();

