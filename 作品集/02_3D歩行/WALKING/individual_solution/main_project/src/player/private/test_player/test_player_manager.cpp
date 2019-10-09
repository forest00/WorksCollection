
//親ヘッダー
#include "..\..\public\test_player\test_player_manager.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "test_player_brain.h"
#include "test_player_database.h"
#include "test_player_object.h"
#include "..\..\..\x_file\public\x_file.h"
#include "..\..\..\sound\public\sound.h"
#include "..\..\..\input\public\input.h"
#include "..\..\..\camera\public\camera.h"



/*------------------------------*/
enum test_player_manager_capacity_t
{
	test_player_manager_capacity = 1,
};

//テスト用プレイヤーマネージャー
struct test_player_manager_t
{
	test_player_object_t *_objects[test_player_manager_capacity];
	size_t _num_of_active;
	test_player_database_t _database;
};
/*------------------------------*/



/*---------------------------------------------------------------------------*/
extern test_player_manager_t *test_player_manager_t_init()
{
	void *creature = malloc(sizeof(test_player_manager_t));
	assert(creature != nullptr);
	test_player_manager_t *manager = (test_player_manager_t*)(creature);
	for (size_t i = 0; i < test_player_manager_t_get_capacity(manager); i++)
	{
		manager->_objects[i] = nullptr;
	}
	manager->_num_of_active = 0;
	test_player_database_t_default(&manager->_database);
	return manager;
}

extern void test_player_manager_t_update(test_player_manager_t *manager)
{
	if (manager)
	{
		for (size_t i = 0; i < test_player_manager_t_get_capacity(manager); i++)
		{
			test_player_object_t_update(manager->_objects[i]);
		}
	}
}

extern void test_player_manager_t_draw(const test_player_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < test_player_manager_t_get_capacity(manager); i++)
	{
		test_player_object_t_draw(manager->_objects[i], device);
	}
}

extern void test_player_manager_t_end(test_player_manager_t **manager)
{
	safe_free(*manager);
}

extern size_t test_player_manager_t_get_num_of_active(const test_player_manager_t *manager)
{
	return manager->_num_of_active;
}

extern size_t test_player_manager_t_get_capacity(const test_player_manager_t *)
{
	return (size_t)(test_player_manager_capacity);
}

extern test_player_object_t **test_player_manager_t_add(test_player_manager_t *manager, const test_player_initializer_t *initializer)
{
	for (size_t i = 0; i < test_player_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i] == nullptr)
		{
			manager->_objects[i] = test_player_object_t_init(initializer, &manager->_database);
			manager->_num_of_active++;
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

extern test_player_object_t **test_player_manager_t_search_active(test_player_manager_t *manager)
{
	for (size_t i = 0; i < test_player_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

extern void test_player_manager_t_delete(test_player_manager_t *manager, test_player_object_t **object)
{
	if (object == nullptr)
	{
		return;
	}
	if (*object == nullptr)
	{
		return;
	}
	test_player_object_t_end(object);
	manager->_num_of_active--;
}

extern const vector3_t *test_player_manager_t_get_position(const test_player_manager_t *, const test_player_object_t *object)
{
	if (object == nullptr)
	{
		return nullptr;
	}
	return test_player_object_t_get_position(object);
}

/*---------------------------------------------------------------------------*/




