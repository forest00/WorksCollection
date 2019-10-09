//親ヘッダー
#include "block_man.h"
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



#define BLOCK_MAN_CAPACITY	16


//block_man構造体
struct block_man_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//block_man_tの初期化処理
static void block_man_t_init(block_man_t *block_man)
{
	block_man->_is_active = false;
	block_man->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	block_man->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	block_man->_base._scale = vector3_t(0.01f, 0.01f, 0.01f);
	anim_init(&block_man->_anim);
}

//block_man_tの終了処理
static void block_man_t_end(block_man_t *block_man)
{
	anim_end(&block_man->_anim);
}

//block_man_tをアクティブにする
static HRESULT block_man_t_to_active(block_man_t *block_man, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// 全メンバーに初期値を代入
	block_man_t_init(block_man);
	//アクティブにする
	block_man->_is_active = true;
	//初期位置に動く
	block_man->_base._position = *pvPosition;
	//アニメーション情報取得
	anim_setup(&block_man->_anim, anim_kind);
	anim_play(&block_man->_anim, 0, anim_repeat_on);
	block_man->_anim._playing = true;

	return S_OK;
}


//block_man_tの1フレーム分の更新
static void block_man_t_update(block_man_t *block_man)
{
	//アクティブかチェック
	if (block_man->_is_active == false)
	{
		return;
	}
	//操作しやすいようにポインタを取得しておく
	thing_parameter_t *base = &block_man->_base;
	anim_parameter_t *anim = &block_man->_anim;
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
		if (input_keyboard_trigger(DIK_F5))
		{
			anim_play(anim, 4, anim_repeat_on);
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
			loop_time() / 100.0,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);
	}
}




//block_man_tを描画
static void block_man_t_draw(block_man_t *block_man, IDirect3DDevice9 *device)
{
	if (block_man->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&block_man->_base._position,
		&block_man->_base._angle,
		&block_man->_base._scale
	);
	anim_thing_draw(device, &world, &block_man->_anim);
}





//block_manのハンドルとして機能する構造体
struct block_man_handle_t
{
	void *_ptr;
};

//ハンドルを生成する
void block_man_handle_t_create(block_man_handle_t *handle, block_man_t *object)
{
	handle->_ptr = (void*)(object);
}

//ハンドルから実体へのポインタを取得
block_man_t *block_man_handle_t_get(block_man_handle_t handle)
{
	return (block_man_t*)(handle._ptr);
}


//block_manの集合を管理する構造体
struct block_man_manager_t
{
	//とりあえず配列(長さは基本固定)
	block_man_t *_array_head;
	size_t _capacity;
};




//block_man_manager_tの初期化処理
void block_man_manager_t_init(block_man_manager_t *manager, size_t capacity)
{
	manager->_array_head = (block_man_t*)malloc(sizeof(block_man_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"メモリ確保に失敗しました\nメモリサイズ : %ld",
			sizeof(block_man_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		block_man_t_init(&manager->_array_head[i]);
	}
}

//block_man_manager_tの終了処理
void block_man_manager_t_end(block_man_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		block_man_t_end(&manager->_array_head[i]);
	}
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//block_man_manager_tを通してblock_man_tを1体アクティブにする
block_man_handle_t block_man_manager_t_add_block_man(block_man_manager_t *manager, D3DXVECTOR3 *position)
{
	block_man_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			block_man_t_to_active(&manager->_array_head[i], anim_model_kind_block_man, position);
			block_man_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	block_man_handle_t_create(&creature, nullptr);
	return creature;
}

//block_man_manager_tが管理しているすべてのblock_man_tを更新
void block_man_manager_t_update(block_man_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		block_man_t_update(&manager->_array_head[i]);
	}
}

//block_man_manager_tが管理しているすべてのblock_man_tを描画
void block_man_manager_t_draw(block_man_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		block_man_t_draw(&manager->_array_head[i], device);
	}
}













static block_man_manager_t *g_block_man_manager;






//block_man初期化 & 出現
extern void block_man_init()
{
	g_block_man_manager = (block_man_manager_t*)malloc(sizeof(block_man_manager_t));
	block_man_manager_t_init(g_block_man_manager, BLOCK_MAN_CAPACITY);
	//block_manを一体出す
	block_man_manager_t_add_block_man(g_block_man_manager,
		&D3DXVECTOR3(
			uniform_rand(-12.0, 12.0),
			uniform_rand(1.0, 2.0),
			uniform_rand(-16.0, 16.0)
		)
	);
}

//block_man終了
extern void block_man_end()
{
	block_man_manager_t_end(g_block_man_manager);
	safe_free(g_block_man_manager);
}

//block_man更新
extern void block_man_update()
{
	//ランダムでblock_manを一体出す
	if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	{
		block_man_manager_t_add_block_man(g_block_man_manager,
			&D3DXVECTOR3(
				uniform_rand(-12.0, 12.0),
				uniform_rand(1.0, 2.0),
				uniform_rand(-16.0, 16.0)
			)
		);
	}
	block_man_manager_t_update(g_block_man_manager);
}

//block_man描画
extern void block_man_draw(IDirect3DDevice9 *device)
{
	block_man_manager_t_draw(g_block_man_manager, device);
}


