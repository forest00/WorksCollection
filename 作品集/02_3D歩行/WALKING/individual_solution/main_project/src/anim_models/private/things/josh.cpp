//親ヘッダー
#include "josh.h"
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



#define JOSH_CAPACITY	16


//josh構造体
struct josh_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//josh_tの初期化処理
static void josh_t_init(josh_t *josh)
{
	josh->_is_active = false;
	josh->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	josh->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	josh->_base._scale = vector3_t(1.0f, 1.0f, 1.0f);
	anim_init(&josh->_anim);
}

//josh_tの終了処理
static void josh_t_end(josh_t *josh)
{
	anim_end(&josh->_anim);
}

//josh_tをアクティブにする
static HRESULT josh_t_to_active(josh_t *josh, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// 全メンバーに初期値を代入
	josh_t_init(josh);
	//アクティブにする
	josh->_is_active = true;
	//初期位置に動く
	josh->_base._position = *pvPosition;
	//アニメーション情報取得
	anim_setup(&josh->_anim, anim_kind);
	anim_play(&josh->_anim, 0, anim_repeat_on);
	josh->_anim._playing = true;

	return S_OK;
}


//josh_tの1フレーム分の更新
static void josh_t_update(josh_t *josh)
{
	//アクティブかチェック
	if (josh->_is_active == false)
	{
		return;
	}
	//操作しやすいようにポインタを取得しておく
	thing_parameter_t *base = &josh->_base;
	anim_parameter_t *anim = &josh->_anim;
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




//josh_tを描画
static void josh_t_draw(josh_t *josh, IDirect3DDevice9 *device)
{
	if (josh->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&josh->_base._position,
		&josh->_base._angle,
		&josh->_base._scale
	);
	anim_thing_draw(device, &world, &josh->_anim);
}





//joshのハンドルとして機能する構造体
struct josh_handle_t
{
	void *_ptr;
};

//ハンドルを生成する
void josh_handle_t_create(josh_handle_t *handle, josh_t *object)
{
	handle->_ptr = (void*)(object);
}

//ハンドルから実体へのポインタを取得
josh_t *josh_handle_t_get(josh_handle_t handle)
{
	return (josh_t*)(handle._ptr);
}


//joshの集合を管理する構造体
struct josh_manager_t
{
	//とりあえず配列(長さは基本固定)
	josh_t *_array_head;
	size_t _capacity;
};




//josh_manager_tの初期化処理
void josh_manager_t_init(josh_manager_t *manager, size_t capacity)
{
	manager->_array_head = (josh_t*)malloc(sizeof(josh_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"メモリ確保に失敗しました\nメモリサイズ : %ld",
			sizeof(josh_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		josh_t_init(&manager->_array_head[i]);
	}
}

//josh_manager_tの終了処理
void josh_manager_t_end(josh_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		josh_t_end(&manager->_array_head[i]);
	}
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//josh_manager_tを通してjosh_tを1体アクティブにする
josh_handle_t josh_manager_t_add_josh(josh_manager_t *manager, D3DXVECTOR3 *position)
{
	josh_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			josh_t_to_active(&manager->_array_head[i], anim_model_kind_josh, position);
			josh_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	josh_handle_t_create(&creature, nullptr);
	return creature;
}

//josh_manager_tが管理しているすべてのjosh_tを更新
void josh_manager_t_update(josh_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		josh_t_update(&manager->_array_head[i]);
	}
}

//josh_manager_tが管理しているすべてのjosh_tを描画
void josh_manager_t_draw(josh_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		josh_t_draw(&manager->_array_head[i], device);
	}
}













static josh_manager_t *g_josh_manager;






//josh初期化 & 出現
extern void josh_init()
{
	g_josh_manager = (josh_manager_t*)malloc(sizeof(josh_manager_t));
	josh_manager_t_init(g_josh_manager, JOSH_CAPACITY);
	//joshを一体出す
	josh_manager_t_add_josh(g_josh_manager,
		&D3DXVECTOR3(
			uniform_rand(0.0, 12.0),
			uniform_rand(1.0, 2.0),
			uniform_rand(8.0, 16.0)
		)
	);
}

//josh終了
extern void josh_end()
{
	josh_manager_t_end(g_josh_manager);
	safe_free(g_josh_manager);
}

//josh更新
extern void josh_update()
{
	//ランダムでjoshを一体出す
	if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	{
		josh_manager_t_add_josh(g_josh_manager,
			&D3DXVECTOR3(
				uniform_rand(0.0, 12.0),
				uniform_rand(1.0, 2.0),
				uniform_rand(8.0, 16.0)
			)
		);
	}
	josh_manager_t_update(g_josh_manager);
}

//josh描画
extern void josh_draw(IDirect3DDevice9 *device)
{
	josh_manager_t_draw(g_josh_manager, device);
}


