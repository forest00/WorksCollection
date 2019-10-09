//親ヘッダー
#include "ken.h"
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



#define KEN_CAPACITY	16


//ken構造体(kenとは...?)
struct ken_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//ken_tの初期化処理
static void ken_t_init(ken_t *ken)
{
	ken->_is_active = false;
	ken->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	ken->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	ken->_base._scale = vector3_t(1.0f, 1.0f, 1.0f);
	anim_init(&ken->_anim);
}

//ken_tの終了処理
static void ken_t_end(ken_t *ken)
{
	anim_end(&ken->_anim);
}

//ken_tをアクティブにする
static HRESULT ken_t_to_active(ken_t *ken, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// 全メンバーに初期値を代入
	ken_t_init(ken);
	//アクティブにする
	ken->_is_active = true;
	//初期位置に動く
	ken->_base._position = *pvPosition;
	//アニメーション情報取得
	anim_setup(&ken->_anim, anim_kind);
	anim_play(&ken->_anim, 0, anim_repeat_off);
	ken->_anim._playing = true;

	return S_OK;
}


//ken_tの1フレーム分の更新
static void ken_t_update(ken_t *ken)
{
	//アクティブかチェック
	if (ken->_is_active == false)
	{
		return;
	}
	//操作しやすいようにポインタを取得しておく
	thing_parameter_t *base = &ken->_base;
	anim_parameter_t *anim = &ken->_anim;
	//アニメーション切り替え処理
	{
		if (input_keyboard_trigger(DIK_F1))
		{
			anim_play(anim, 0, anim_repeat_off);
		}
		if (input_keyboard_trigger(DIK_F2))
		{
			anim_play(anim, 1, anim_repeat_off);
		}
		if (input_keyboard_trigger(DIK_F3))
		{
			anim_play(anim, 2, anim_repeat_off);
		}
		if (input_keyboard_trigger(DIK_F4))
		{
			anim_play(anim, 3, anim_repeat_on);
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



//ken_tを描画
static void ken_t_draw(ken_t *ken, IDirect3DDevice9 *device)
{
	if (ken->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&ken->_base._position,
		&ken->_base._angle,
		&ken->_base._scale
	);
	anim_thing_draw(device, &world, &ken->_anim);
}





//kenのハンドルとして機能する構造体
struct ken_handle_t
{
	void *_ptr;
};

//ハンドルを生成する
void ken_handle_t_create(ken_handle_t *handle, ken_t *object)
{
	handle->_ptr = (void*)(object);
}

//ハンドルから実体へのポインタを取得
ken_t *ken_handle_t_get(ken_handle_t handle)
{
	return (ken_t*)(handle._ptr);
}


//kenの集合を管理する構造体
struct ken_manager_t
{
	//とりあえず配列(長さは基本固定)
	ken_t *_array_head;
	size_t _capacity;
};




//ken_manager_tの初期化処理
void ken_manager_t_init(ken_manager_t *manager, size_t capacity)
{
	manager->_array_head = (ken_t*)malloc(sizeof(ken_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"メモリ確保に失敗しました\nメモリサイズ : %ld",
			sizeof(ken_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		ken_t_init(&manager->_array_head[i]);
	}
}

//ken_manager_tの終了処理
void ken_manager_t_end(ken_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		ken_t_end(&manager->_array_head[i]);
	}
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//ken_manager_tを通してken_tを1体アクティブにする
ken_handle_t ken_manager_t_add_ken(ken_manager_t *manager, D3DXVECTOR3 *position)
{
	ken_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			ken_t_to_active(&manager->_array_head[i], anim_model_kind_ken, position);
			ken_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	ken_handle_t_create(&creature, nullptr);
	return creature;
}

//ken_manager_tが管理しているすべてのken_tを更新
void ken_manager_t_update(ken_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		ken_t_update(&manager->_array_head[i]);
	}
}

//ken_manager_tが管理しているすべてのken_tを描画
void ken_manager_t_draw(ken_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		ken_t_draw(&manager->_array_head[i], device);
	}
}













static ken_manager_t *g_ken_manager;






//ken初期化 & 出現
extern void ken_init()
{
	g_ken_manager = (ken_manager_t*)malloc(sizeof(ken_manager_t));
	ken_manager_t_init(g_ken_manager, KEN_CAPACITY);
	//kenを一体出す
	ken_manager_t_add_ken(g_ken_manager,
		&D3DXVECTOR3(
			uniform_rand(0.0, 12.0),
			uniform_rand(0.0, 1.0),
			uniform_rand(0.0, 8.0)
		)
	);
}

//ken終了
extern void ken_end()
{
	ken_manager_t_end(g_ken_manager);
	safe_free(g_ken_manager);
}

//ken更新
extern void ken_update()
{
	//ランダムでkenを一体出す
	if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	{
		ken_manager_t_add_ken(g_ken_manager,
			&D3DXVECTOR3(
				uniform_rand(0.0, 12.0),
				uniform_rand(0.0, 1.0),
				uniform_rand(0.0, 8.0)
			)
		);
	}
	ken_manager_t_update(g_ken_manager);
}

//ken描画
extern void ken_draw(IDirect3DDevice9 *device)
{
	ken_manager_t_draw(g_ken_manager, device);
}


