#pragma once

//親ヘッダー
#include "..\public\enemy.h"
//その他のヘッダー
#include "..\public\test_enemy\test_enemy_manager.h"
#include "..\..\input\public\input.h"


struct enemy_manager_t
{
	test_enemy_manager_t *_test_enemy_manager;
};


static enemy_manager_t g_enemy_manager = {};


extern void enemy_init()
{
	test_enemy_manager_t_end(&g_enemy_manager._test_enemy_manager);
	g_enemy_manager._test_enemy_manager = test_enemy_manager_t_init();
}

extern void enemy_update()
{
	test_enemy_manager_t_update(g_enemy_manager._test_enemy_manager);
}

extern void enemy_draw(IDirect3DDevice9 *device)
{
	test_enemy_manager_t_draw(g_enemy_manager._test_enemy_manager, device);
}

extern void enemy_end()
{
	test_enemy_manager_t_end(&g_enemy_manager._test_enemy_manager);
}

extern test_enemy_manager_t *enemy_get_test_enemy_manager()
{
	return g_enemy_manager._test_enemy_manager;
}
