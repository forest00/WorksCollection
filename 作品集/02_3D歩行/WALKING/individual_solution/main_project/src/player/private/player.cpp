
//親ヘッダー
#include "..\public\player.h"
//その他のヘッダー


struct player_manager_t
{
	test_player_manager_t *test_player_manager;
};

static player_manager_t g_player_manager = {};

extern void player_init()
{
	g_player_manager.test_player_manager = test_player_manager_t_init();
}

extern void player_update()
{
	test_player_manager_t_update(g_player_manager.test_player_manager);
}

extern void player_draw(IDirect3DDevice9 *device)
{
	test_player_manager_t_draw(g_player_manager.test_player_manager, device);
}

extern void player_end()
{
	test_player_manager_t_end(&g_player_manager.test_player_manager);
}

extern test_player_manager_t *player_get_test_player_manager()
{
	return g_player_manager.test_player_manager;
}
