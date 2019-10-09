#pragma once

#include "test_player\test_player_manager.h"

//‰Šú‰»
extern void player_init();

//XV
extern void player_update();

//•`‰æ
extern void player_draw(IDirect3DDevice9 *device);

//I—¹
extern void player_end();


extern test_player_manager_t *player_get_test_player_manager();

