#pragma once

//親ヘッダー
#include "..\..\public\test_enemy\test_enemy_manager.h"
//その他のヘッダー
#include <assert.h>
#include "test_enemy_database.h"
#include "test_enemy_object.h"
#include "..\..\..\angle\public\angle.h"
#include "..\..\..\x_file\public\x_file_type.h"
#include "..\..\..\my_3d_lib\public\my_3d_lib.h"



/*------------------------------*/
enum test_enemy_manager_t_capacity_t
{
	test_enemy_manager_capacity = 64,
};

//テスト用敵キャラマネージャー
struct test_enemy_manager_t
{
	test_enemy_object_t *_objects[test_enemy_manager_capacity];
	size_t _num_of_active;
	test_enemy_database_t _database;
};
/*------------------------------*/











/*------------------------------*/
//テスト用敵キャラマネージャーを確保して初期化して返す
extern test_enemy_manager_t *test_enemy_manager_t_init()
{
	void *creature = malloc(sizeof(test_enemy_manager_t));
	assert(creature != nullptr);
	test_enemy_manager_t *manager = (test_enemy_manager_t*)(creature);
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		manager->_objects[i] = nullptr;
	}
	manager->_num_of_active = 0;
	test_enemy_database_default(&manager->_database);
	return manager;
}

//テスト用敵キャラマネージャーの更新
//管理下にある全敵キャラに対して、更新処理をする
extern void test_enemy_manager_t_update(test_enemy_manager_t *manager)
{
	if(manager){
		for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
		{
			if (manager->_objects[i])
			{
				test_enemy_object_t_update(manager->_objects[i]);
			}
		}
	}
}

//テスト用敵キャラマネージャーが管理しているテスト用敵キャラをすべて描画
extern void test_enemy_manager_t_draw(const test_enemy_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			test_enemy_object_t_draw(manager->_objects[i], device);
		}
	}
}


//テスト用敵キャラマネージャーの終了処理 & メモリ解放処理
//引数として渡したポインタにもnullが代入されます
extern void test_enemy_manager_t_end(test_enemy_manager_t **manager)
{
	if (*manager)
	{
		for (size_t i = 0; i < test_enemy_manager_t_get_capacity(*manager); i++)
		{
			test_enemy_object_t_end(&(*manager)->_objects[i]);
		}
		safe_free(*manager);
	}
}

//テスト用敵キャラマネージャーが管理しているテスト用敵キャラの数を返す
extern size_t test_enemy_manager_t_get_num_of_active(const test_enemy_manager_t *manager)
{
	return manager->_num_of_active;
}

//テスト用敵キャラマネージャーが管理出来るテスト用敵キャラの最大数を返す
extern size_t test_enemy_manager_t_get_capacity(const test_enemy_manager_t *)
{
	return (size_t)(test_enemy_manager_capacity);
}

//テスト用敵キャラを追加する
//追加したテスト用敵キャラへのポインタを返す
//何らかの原因により失敗したら、nullを返す
extern test_enemy_object_t **test_enemy_manager_t_add(test_enemy_manager_t *manager, const test_enemy_initializer_t *initializer)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i] == nullptr)
		{
			manager->_objects[i] = test_enemy_object_t_init(&manager->_database, initializer);
			manager->_num_of_active++;
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//テスト用敵キャラを削除する
extern void test_enemy_manager_t_delete(test_enemy_manager_t *manager, test_enemy_object_t **object)
{
	if (object == nullptr)
	{
		return;
	}
	if (*object == nullptr)
	{
		return;
	}
	test_enemy_object_t_end(object);
	manager->_num_of_active--;
}

//テスト用敵キャラマネージャーが管理しているオブジェクトの中で、最も取得しやすいオブジェクトへのポインタを返す
extern test_enemy_object_t **test_enemy_manager_t_search_active(test_enemy_manager_t *manager)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//テスト用敵キャラマネージャーが管理しているオブジェクトの中で、AABBと衝突しているオブジェクトへのポインタを返す
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_aabb(test_enemy_manager_t *manager, const aabb_t *aabb)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//テスト用敵キャラマネージャーが管理しているオブジェクトの中で、球体と衝突しているオブジェクトへのポインタを返す
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_sphere(test_enemy_manager_t *manager, const sphere_t *sphere)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//テスト用敵キャラの位置を得る
extern const vector3_t *test_enemy_manager_t_get_position(const test_enemy_manager_t *manager, const test_enemy_object_t *object)
{
	if (object == nullptr)
	{
		return nullptr;
	}
	return test_enemy_object_t_get_position(object);
}

//テスト用敵キャラの角度を得る
extern const angle_t *test_enemy_manager_t_get_direction(const test_enemy_manager_t *manager, const test_enemy_object_t *object)
{
	if (object == nullptr)
	{
		return nullptr;
	}
	return test_enemy_object_t_get_direction(object);
}
/*------------------------------*/


