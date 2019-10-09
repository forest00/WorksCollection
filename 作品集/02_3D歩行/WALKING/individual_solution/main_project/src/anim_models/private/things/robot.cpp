//親ヘッダー
#include "robot.h"
//その他のヘッダー
#include <stdlib.h>
#include "..\..\..\calc\public\calc.h"
#include "..\..\public\anim_model_base.h"
#include "..\..\public\anim_data_loader.h"
#include "..\..\..\font\public\font.h"
#include "..\..\..\input\public\input.h"
#include "..\..\..\loop\public\loop.h"
#include "..\..\..\world_transformation\public\world_transformation.h"
//デバッグ用ヘッダー
#include "..\..\..\game_debug\public\message_box.h"



#define ROBOT_CAPACITY	16


//robot構造体
struct robot_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//robot_tの初期化処理
static void robot_t_init(robot_t *robot)
{
	robot->_is_active = false;
	robot->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	robot->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	robot->_base._scale = vector3_t(1.0f, 1.0f, 1.0f);
	anim_init(&robot->_anim);
}

//robot_tの終了処理
static void robot_t_end(robot_t *robot)
{
	anim_end(&robot->_anim);
}

//robot_tをアクティブにする
static HRESULT robot_t_to_active(robot_t *robot, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// 全メンバーに初期値を代入
	robot_t_init(robot);
	//アクティブにする
	robot->_is_active = true;
	//初期位置に動く
	robot->_base._position = *pvPosition;
	//アニメーション情報取得
	anim_setup(&robot->_anim, anim_kind);
	anim_play(&robot->_anim, 0, anim_repeat_on);
	robot->_anim._playing = true;

	return S_OK;
}


//robot_tの1フレーム分の更新
static void robot_t_update(robot_t *robot)
{
	//アクティブかチェック
	if (robot->_is_active == false)
	{
		return;
	}
	//操作しやすいようにポインタを取得しておく
	thing_parameter_t *base = &robot->_base;
	anim_parameter_t *anim = &robot->_anim;
	//アニメーション切り替え処理
	{
		if (input_keyboard_trigger(DIK_F1))
		{
			anim_play(anim, 0, anim_repeat_on);			
		}
		if (input_keyboard_trigger(DIK_F2))
		{
			anim_play(anim, 1, anim_repeat_on);
		}
	}
	//アニメーション再生と停止を制御
	{
		if (input_keyboard_trigger(DIK_NUMPAD1))
		{
			anim->_playing = true;
		}
		if (input_keyboard_trigger(DIK_NUMPAD2))
		{
			anim->_playing = false;
		}
		if (input_keyboard_trigger(DIK_NUMPAD3))
		{
			anim->_playing = bool_invers(anim->_playing);
		}
	}
	//アニメーションを進める
	{
		anim_advance(
			anim,
			loop_time(),
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);
	}
}



//robot_tを描画
static void robot_t_draw(robot_t *robot, IDirect3DDevice9 *device)
{
	if (robot->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&robot->_base._position,
		&robot->_base._angle,
		&robot->_base._scale
	);
	anim_thing_draw(device, &world, &robot->_anim);
}





//robotのハンドルとして機能する構造体
struct robot_handle_t
{
	void *_ptr;
};

//ハンドルを生成する
void robot_handle_t_create(robot_handle_t *handle, robot_t *object)
{
	handle->_ptr = (void*)(object);
}

//ハンドルから実体へのポインタを取得
robot_t *robot_handle_t_get(robot_handle_t handle)
{
	return (robot_t*)(handle._ptr);
}


//robotの集合を管理する構造体
struct robot_manager_t
{
	//とりあえず配列(長さは基本固定)
	robot_t *_array_head;
	size_t _capacity;
};




//robot_manager_tの初期化処理
void robot_manager_t_init(robot_manager_t *manager, size_t capacity)
{
	manager->_array_head = (robot_t*)malloc(sizeof(robot_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"メモリ確保に失敗しました\nメモリサイズ : %ld",
			sizeof(robot_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		robot_t_init(&manager->_array_head[i]);
	}
}

//robot_manager_tの終了処理
void robot_manager_t_end(robot_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		robot_t_end(&manager->_array_head[i]);
	}
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//robot_manager_tを通してrobot_tを1体アクティブにする
robot_handle_t robot_manager_t_add_robot(robot_manager_t *manager, D3DXVECTOR3 *position)
{
	robot_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			robot_t_to_active(&manager->_array_head[i], anim_model_kind_robot, position);
			robot_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	robot_handle_t_create(&creature, nullptr);
	return creature;
}

//robot_manager_tが管理しているすべてのrobot_tを更新
void robot_manager_t_update(robot_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		robot_t_update(&manager->_array_head[i]);
	}
}

//robot_manager_tが管理しているすべてのrobot_tを描画
void robot_manager_t_draw(robot_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		robot_t_draw(&manager->_array_head[i], device);
	}
}













static robot_manager_t *g_robot_manager;






//robot初期化 & 出現
extern void robot_init()
{
	g_robot_manager = (robot_manager_t*)malloc(sizeof(robot_manager_t));
	robot_manager_t_init(g_robot_manager, ROBOT_CAPACITY);
	//robotを一体出す
	robot_manager_t_add_robot(g_robot_manager,
		&D3DXVECTOR3(
			uniform_rand(-12.0, 0.0),
			uniform_rand(0.0, 1.0),
			uniform_rand(-8.0, 0.0)
		)
	);
}

//robot終了
extern void robot_end()
{
	robot_manager_t_end(g_robot_manager);
	safe_free(g_robot_manager);
}

//robot更新
extern void robot_update()
{
	//ランダムでrobotを一体出す
	if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	{
		robot_manager_t_add_robot(g_robot_manager,
			&D3DXVECTOR3(
				uniform_rand(-12.0, 0.0),
				uniform_rand(0.0, 1.0),
				uniform_rand(-8.0, 0.0)
			)
		);
	}
	robot_manager_t_update(g_robot_manager);
}

//robot描画
extern void robot_draw(IDirect3DDevice9 *device)
{
	robot_manager_t_draw(g_robot_manager, device);
}


