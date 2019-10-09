/*-----------------------------------------------------------*/
//                                                           
//                                                           
// ゲーム部分の初期化、更新、描画、終了までをサポートする場所です  
//                                                           
//                                                           
//    あらかじめシステム部分の操作をしておかないといけません      
//                                                           
//                                                           
//                                                           
//                                                           
//      作成者      森  仁志                                  
//                                                           
//                                                           
/*-----------------------------------------------------------*/


//親ヘッダー
#include "game_base.h"
//その他のヘッダー
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\d3d_device\public\d3d_device.h"
#include "..\..\player\public\player.h"
#include "..\..\x_file\public\x_file.h"
#include "..\..\sound\public\sound.h"
#include "..\..\input\public\input.h"
#include "..\..\enemy\public\enemy.h"
#include "..\..\back\public\back.h"
#include "..\..\anim_models\public\anim_model_test.h"
#include "..\..\camera\public\camera.h"
#include "..\..\font\public\font.h"
#include "..\..\flatpo\public\flatpo_manager.h"
#include "..\..\makabe_mesh\public\makabe_mesh.h"
//デバッグ用ヘッダー
#include "..\..\game_debug\public\game_debug.h"



/*-------------------------------------------------------------------*/
///ここから、ローカル関数

//ゲームの準備(初期化が終わったら実行)
static void game_base_setup()
{
	//プレイヤーを出す
	{
		test_player_manager_t *manager = player_get_test_player_manager();
		test_player_initializer_t in;
		in._position = { 0.0,0.0,0.0 };
		in._type = x_file_type_Hubon;
		test_player_manager_t_add(manager, &in);
	}
	//最初の楽曲を再生(ゲーム部分の初期化処理)
	sound_play(sound_index_bgm_1);
}


//リセットボタンが押されているか判定(ゲーム部分の更新処理)
static bool game_base_pushed_reset_button()
{
	//今だけオフ
	//return false;
	bool input_reset = false;
	input_reset |= input_keyboard_trigger(DIK_RETURN);
	input_reset |= input_keyboard_trigger(DIK_SPACE);
	input_reset |= input_controller_trigger(input_controller_code_start);
	input_reset |= input_controller_trigger(input_controller_code_back);
	return input_reset;
}

//trueが渡されたら、ゲームをリセット
static void game_base_reset_if(bool fire)
{
	if (fire)
	{
		game_base_end();

		game_base_init();
	}
}

///ここまで、ローカル関数
/*-------------------------------------------------------------------*/


//ゲーム部分の初期化
extern void game_base_init()
{
	//プレイヤーの初期化
	player_init();

	//エネミーの初期化
	enemy_init();

	//背景の初期化
	back_manager_init();

	//アニメーションモデル初期化
	anim_model_tester_all_init(d3d_device());

	//カメラの初期化
	camera_manager_init();

	//板ポリ初期化
	flatpo_manager_init(d3d_device());

	//まかべメッシュの初期化
	makabe_mash_init(d3d_device());


	//初期化が終わったのでゲームの準備をする
	game_base_setup();
}

//ゲーム部分の更新
extern void game_base_update()
{
	//敵キャラ動作テスト
	if (input_keyboard_trigger(DIK_1) || input_keyboard_trigger(DIK_2))
	{
		test_enemy_manager_t *manager = enemy_get_test_enemy_manager();
		test_enemy_initializer_t in;
		in._position = {
			(float)((double)(rand()) / (double)(RAND_MAX) * 20.0),
			(float)((double)(rand()) / (double)(RAND_MAX) + 1.0),
			(float)((double)(rand()) / (double)(RAND_MAX) * 20.0)
		};
		in._direction = 0.0;
		in._type = x_file_type_ball;
		test_enemy_manager_t_add(manager, &in);
	}
	if (input_keyboard_trigger(DIK_NUMPAD1) || input_keyboard_trigger(DIK_NUMPAD2))
	{
		test_enemy_manager_t *manager = enemy_get_test_enemy_manager();
		test_enemy_object_t **found = test_enemy_manager_t_search_active(manager);
		test_enemy_manager_t_delete(manager, found);
	}

	//プレイヤーの更新
	player_update();

	//エネミーの更新
	enemy_update();

	//背景の更新
	back_manager_update();

	//アニメーションモデル更新
	anim_model_tester_all_update();

	//カメラを更新
	camera_manager_update();

	//板ポリ更新
	flatpo_manager_update();

	//まかべメッシュの更新
	makabe_mash_update();

	{
		//マウスクリックがあれば曲をチェンジ
		{
			if (input_mouse_trigger(input_mouse_code_left))
			{
				sound_play(sound_index_bgm_1);
			}
			if (input_mouse_trigger(input_mouse_code_right))
			{
				sound_play(sound_index_bgm_2);
			}
			if (input_mouse_trigger(input_mouse_code_middle))
			{
				sound_play(sound_index_bgm_3);
			}
			if (input_mouse_wheel() > 0)
			{
				sound_play(sound_index_bgm_4);
			}
		}

		//入力があれば、ゲームをリセット
		game_base_reset_if(game_base_pushed_reset_button());
	}


	//デバッグ用情報を出す
	game_debug_infomation();

}

//ゲーム部分の描画
extern void game_base_draw()
{
	//背景の描画
	back_manager_draw(d3d_device());


	//プレイヤーの描画
	player_draw(d3d_device());

	//エネミーの描画
	enemy_draw(d3d_device());

	//アニメーションモデル描画
	anim_model_tester_all_draw(d3d_device());

	//板ポリ描画
	flatpo_manager_draw(d3d_device(), lib_get_aspect());

	//フォントを描画
	font_draw(d3d_device());

	//まかべメッシュの描画
	makabe_mash_draw(d3d_device());

	//Xキーでプレイヤーの位置にスフィア描画(テスト用)
	if (input_keyboard_repeat(DIK_X))
	{
		test_player_manager_t *manager = player_get_test_player_manager();
		test_player_object_t **object = test_player_manager_t_search_active(manager);
		const vector3_t *player_pos = test_player_manager_t_get_position(manager, *object);
		sphere_t test;
		static double rr = 1.0;
		test.center = *player_pos;
		test.radius = rr;
		sphere_draw(d3d_device(), &test);
	}
}

//ゲーム部分の終了
extern void game_base_end()
{
	//プレイヤーの終了
	player_end();

	//エネミーの終了
	enemy_end();

	//背景の終了
	back_manager_end();

	//アニメーションモデル破棄
	anim_model_tester_all_end();

	//カメラの終了
	camera_manager_end();

	//板ポリ終了
	flatpo_manager_end();

	//まかべメッシュの終了
	makabe_mash_end();
}


