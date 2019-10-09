//親ヘッダー
#include "golem.h"
//その他のヘッダー
#include <stdlib.h>
#include "..\..\..\calc\public\calc.h"
#include "..\..\public\anim_model_base.h"
#include "..\..\public\anim_data_loader.h"
#include "..\..\..\font\public\font.h"
#include "..\..\..\input\public\input.h"
#include "..\..\..\loop\public\loop.h"
#include "..\..\..\world_transformation\public\world_transformation.h"
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "..\..\..\camera\public\camera.h"
#include "..\..\..\sound\public\sound.h"
//デバッグ用ヘッダー
#include "..\..\..\game_debug\public\message_box.h"



#define GOLEM_CAPACITY	16


//golem構造体
struct golem_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//golem_tの初期化処理
static void golem_t_init(golem_t *golem)
{
	golem->_is_active = false;
	golem->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	golem->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	golem->_base._scale = vector3_t(0.01f, 0.01f, 0.01f);
	anim_init(&golem->_anim);
}

//golem_tの終了処理
static void golem_t_end(golem_t *golem)
{
	anim_end(&golem->_anim);
}

//golem_tをアクティブにする
static HRESULT golem_t_to_active(golem_t *golem, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// 全メンバーに初期値を代入
	golem_t_init(golem);
	//アクティブにする
	golem->_is_active = true;
	//初期位置に動く
	golem->_base._position = *pvPosition;
	//アニメーション情報取得
	anim_setup(&golem->_anim, anim_kind);
	anim_play(&golem->_anim, 0, anim_repeat_off);
	golem->_anim._playing = true;

	return S_OK;
}


//golem_tの1フレーム分の更新
static void golem_t_update(golem_t *golem)
{
	//アクティブかチェック
	if (golem->_is_active == false)
	{
		return;
	}
	//操作しやすいようにポインタを取得しておく
	thing_parameter_t *base = &golem->_base;
	anim_parameter_t *anim = &golem->_anim;
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



//golem_tの1フレーム分の更新(プレイヤー用)
static void golem_t_update_as_player(golem_t *golem)
{
	//アクティブかチェック
	if (golem->_is_active == false)
	{
		return;
	}
	//操作しやすいようにポインタを取得しておく
	thing_parameter_t *base = &golem->_base;
	anim_parameter_t *anim = &golem->_anim;
	//入力により動く
	{
		double input_x = input_controller_left_x() + input_keyboard_repeat(DIK_RIGHT) - input_keyboard_repeat(DIK_LEFT);
		double input_y = input_controller_left_y() + input_keyboard_repeat(DIK_DOWN) - input_keyboard_repeat(DIK_UP);
		bool input_fire = 
			input_controller_trigger(input_controller_code_a) ||
			input_controller_trigger(input_controller_code_b) || 
			input_keyboard_trigger(DIK_Z);
		bool input_on = (fabs(input_x) > 0.1) || (fabs(input_y) > 0.1);
		double input_angle = atan2(input_y, input_x);
		angle_t camera_theta = 0.0;
		camera_get_theta(&camera_theta);
		double go_angle = input_angle - camera_theta;
		if (anim->_playing_index != 2)
		{
			if (input_on)
			{
				base->_position.x -= cos(go_angle + M_PI_2) / 8.0;
				base->_position.z += sin(go_angle + M_PI_2) / 8.0;
				base->_angle.y = go_angle;
				if (anim->_playing_index != 3)
				{
					sound_play(sound_index_se_3);
					anim_play(anim, 3, anim_repeat_on);
				}
			}
			else
			{
				if (anim->_playing_index != 4)
				{
					anim_play(anim, 4, anim_repeat_on);
				}
			}
			if (input_fire)
			{
				sound_play(sound_index_se_4);
				anim_play(anim, 2, anim_repeat_off);
			}
		}
	}
	//アニメーションを進める
	{
		bool ended = false;
		anim_advance(
			anim,
			loop_time() * 2.0,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			&ended
		);
		if (anim->_playing_index == 2 && ended)
		{
			anim_play(anim, 3, anim_repeat_on);
		}
	}
}



//golem_tを描画
static void golem_t_draw(golem_t *golem, IDirect3DDevice9 *device)
{
	if (golem->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&golem->_base._position,
		&golem->_base._angle,
		&golem->_base._scale
	);
	anim_thing_draw(device, &world, &golem->_anim);
}





//golemのハンドルとして機能する構造体
struct golem_handle_t
{
	void *_ptr;
};

//ハンドルを生成する
void golem_handle_t_create(golem_handle_t *handle, golem_t *object)
{
	handle->_ptr = (void*)(object);
}

//ハンドルから実体へのポインタを取得
golem_t *golem_handle_t_get(golem_handle_t handle)
{
	return (golem_t*)(handle._ptr);
}


//golemの集合を管理する構造体
struct golem_manager_t
{
	//とりあえず配列(長さは基本固定)
	golem_t *_array_head;
	size_t _capacity;
	//プレイヤー
	golem_t _player_golem;
};




//golem_manager_tの初期化処理
void golem_manager_t_init(golem_manager_t *manager, size_t capacity)
{
	manager->_array_head = (golem_t*)malloc(sizeof(golem_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"メモリ確保に失敗しました\nメモリサイズ : %ld",
			sizeof(golem_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		golem_t_init(&manager->_array_head[i]);
	}
	//プレイヤー
	golem_t_init(&manager->_player_golem);
	golem_t_to_active(&manager->_player_golem, anim_model_kind_golem, &D3DXVECTOR3(30.0f, 0.0f, -30.0f));
}

//golem_manager_tの終了処理
void golem_manager_t_end(golem_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		golem_t_end(&manager->_array_head[i]);
	}
	//プレイヤー
	golem_t_end(&manager->_player_golem);
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//golem_manager_tを通してgolem_tを1体アクティブにする
golem_handle_t golem_manager_t_add_golem(golem_manager_t *manager, D3DXVECTOR3 *position)
{
	golem_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			golem_t_to_active(&manager->_array_head[i], anim_model_kind_golem, position);
			golem_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	golem_handle_t_create(&creature, nullptr);
	return creature;
}

//golem_manager_tが管理しているすべてのgolem_tを更新
void golem_manager_t_update(golem_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		golem_t_update(&manager->_array_head[i]);
	}
	//プレイヤー
	golem_t_update_as_player(&manager->_player_golem);
}

//golem_manager_tが管理しているすべてのgolem_tを描画
void golem_manager_t_draw(golem_manager_t *manager, IDirect3DDevice9 *device)
{
	///設定を保存しとく
	static DWORD option[7];
	device->GetRenderState(D3DRS_ALPHABLENDENABLE, &option[0]);
	device->GetRenderState(D3DRS_SRCBLEND, &option[1]);
	device->GetRenderState(D3DRS_DESTBLEND, &option[2]);
	device->GetRenderState(D3DRS_ALPHATESTENABLE, &option[3]);
	device->GetTextureStageState(0, D3DTSS_ALPHAOP, &option[4]);
	device->GetTextureStageState(0, D3DTSS_COLORARG1, &option[5]);
	device->GetTextureStageState(0, D3DTSS_COLORARG2, &option[6]);
	///設定を変える
	//アルファブレンディングを設定する
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	// テクスチャのブレンディング方法を定義する
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//メイン描画
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		golem_t_draw(&manager->_array_head[i], device);
	}
	//プレイヤー
	golem_t_draw(&manager->_player_golem, device);
	///設定を元に戻す
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, option[0]);
	device->SetRenderState(D3DRS_SRCBLEND, option[1]);
	device->SetRenderState(D3DRS_DESTBLEND, option[2]);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, option[3]);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, option[4]);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, option[5]);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, option[6]);
}













static golem_manager_t *g_golem_manager;






//golem初期化 & 出現
extern void golem_init()
{
	g_golem_manager = (golem_manager_t*)malloc(sizeof(golem_manager_t));
	golem_manager_t_init(g_golem_manager, GOLEM_CAPACITY);
	//golemを一体出す
	//golem_manager_t_add_golem(g_golem_manager,
	//	&D3DXVECTOR3(
	//		uniform_rand(-20.0, 0.0),
	//		uniform_rand(0.0, 1.0),
	//		uniform_rand(0.0, 8.0)
	//	)
	//);
}

//golem終了
extern void golem_end()
{
	golem_manager_t_end(g_golem_manager);
	safe_free(g_golem_manager);
}

//golem更新
extern void golem_update()
{
	//ランダムでgolemを一体出す
	//if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	//{
	//	golem_manager_t_add_golem(g_golem_manager,
	//		&D3DXVECTOR3(
	//			uniform_rand(-20.0, 0.0),
	//			uniform_rand(0.0, 1.0),
	//			uniform_rand(0.0, 8.0)
	//		)
	//	);
	//}
	golem_manager_t_update(g_golem_manager);
}

//golem描画
extern void golem_draw(IDirect3DDevice9 *device)
{
	golem_manager_t_draw(g_golem_manager, device);
}

//入力に応じて動くゴーレムの位置を得る
void golem_get_position_special(D3DXVECTOR3 *out)
{
	*out = g_golem_manager->_player_golem._base._position;
}

