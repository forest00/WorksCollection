#pragma once

#include "test_player\test_player_manager.h"

//������
extern void player_init();

//�X�V
extern void player_update();

//�`��
extern void player_draw(IDirect3DDevice9 *device);

//�I��
extern void player_end();


extern test_player_manager_t *player_get_test_player_manager();

