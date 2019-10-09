//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	履歴
//	2019/02/04...プレーヤーが落下したときにステージを再構築するように変更
//	

//親ヘッダー
#include "game_main.h"
//その他のヘッダー
#include "sprite\sprite_integration.h"
#include "fbx_object\object_manager.h"
#include "glitter\glitter_manager.h"
#include "camera\camera_manager.h"
#include "connecter\info_holder.h"
#include "connecter\info_drawer.h"
#include "stage_event\stage_index_saver.h"
#include "stage_event\stage_setup.h"
#include "stage_event\stage_data.h"
#include "stage_event\stage_event.h"
#include "stage_event\stage_ui.h"
#include "..\..\..\mylib\mylib.h"
#include "..\..\..\calc\calc.h"
#include "..\..\..\input\input.h"
#include "..\..\..\sound\sound.h"
#include "..\..\..\texture_manager\texture_manager.h"

//ゴール後の待ち時間(フレーム)
#define _goal_wait_frame		(120)

#define _coord_continue_x		(457.0f)
#define _coord_continue_y		(230.0f)
#define _coord_title_x			(475.0f)
#define _coord_title_y			(411.0f)
#define _coord_cursor_x1		(324.0f)
#define _coord_cursor_y1		(275.0f)
#define _coord_cursor_x2		(340.0f)
#define _coord_cursor_y2		(439.0f)

//ポーズのリザルト
enum pause_result_t
{
	pr_pause,//ポーズ画面のまま続けるべき
	pr_continue,//ゲームを再開するべき
	pr_title,//タイトルに戻るべき
};

//ポーズ中のカーソル
struct cursor_t
{
	int _pos;//現在位置　0上、1下
	float _draw_x;//描画位置x
	float _draw_y;//描画位置y
	bool _moving;//移動中
	float _change_sec;//切り替えられた瞬間
	float _move_duration_sec;//移動にかかる全体の時間
};
//ポーズ中に使う変数
struct pause_t
{
	cursor_t _cursor;
	texture_id_t _continue_tex;
	texture_id_t _title_tex;
	float _cursor_draw_pos[2][2];//位置情報の集まり
};
//グローバル変数
struct global_t
{
	pause_t _pause;
	game_main_fbx_object_array_t _object_array;
	bool _pause_initialized;
	bool _pause_mode;
};

static global_t g;

//ポーズの処理
#if 1
//ポーズ初期化
static void pause_init()
{
	pause_t *pause = &g._pause;
	cursor_t *cursor = &pause->_cursor;
	cursor->_pos = 0;
	cursor->_draw_x = _coord_cursor_x1;
	cursor->_draw_y = _coord_cursor_y1;
	cursor->_moving = false;
	cursor->_change_sec = 0.0f;
	cursor->_move_duration_sec = 0.3f;
	pause->_continue_tex = ti_pause_continue_on;
	pause->_title_tex = ti_pause_title_off;
	pause->_cursor_draw_pos[0][0] = _coord_cursor_x1;
	pause->_cursor_draw_pos[0][1] = _coord_cursor_y1;
	pause->_cursor_draw_pos[1][0] = _coord_cursor_x2;
	pause->_cursor_draw_pos[1][1] = _coord_cursor_y2;
}

//ポーズ終了
static void pause_end()
{

}

//ポーズ中のみ呼ばれる更新
static pause_result_t pause_update(const virtual_input_t *trigger)
{
	pause_t *pause = &g._pause;
	cursor_t *cursor = &pause->_cursor;
	//上が押された
	if (trigger->_lkey_up || trigger->_rkey_up)
	{
		if (cursor->_pos != 0)
		{
			cursor->_moving = true;
			cursor->_change_sec = mylib_now_sec();
			cursor->_pos = 0;
			pause->_continue_tex = ti_pause_continue_on;
			pause->_title_tex = ti_pause_title_off;
		}
	}
	//下が押された
	if (trigger->_lkey_down || trigger->_rkey_down)
	{
		if (cursor->_pos != 1)
		{
			cursor->_moving = true;
			cursor->_change_sec = mylib_now_sec();
			cursor->_pos = 1;
			pause->_continue_tex = ti_pause_continue_off;
			pause->_title_tex = ti_pause_title_on;
		}
	}
	//カーソル移動中
	if (cursor->_moving)
	{
		float elapsed = mylib_now_sec() - cursor->_change_sec;
		float t = elapsed / cursor->_move_duration_sec;
		if (t >= 1.0f)
		{
			t = 1.0f;
			cursor->_moving = false;
		}
		float start_x = pause->_cursor_draw_pos[1 - cursor->_pos][0];
		float start_y = pause->_cursor_draw_pos[1 - cursor->_pos][1];
		float end_x = pause->_cursor_draw_pos[cursor->_pos][0];
		float end_y = pause->_cursor_draw_pos[cursor->_pos][1];
		cursor->_draw_x = (float)call_easing(et_EaseOutCirc, t, start_x, end_x - start_x, 1.0f);
		cursor->_draw_y = (float)call_easing(et_EaseOutCirc, t, start_y, end_y - start_y, 1.0f);
	}
	//決定キーが押された
	if (trigger->_a)
	{
		switch (cursor->_pos)
		{
		case 0:
			//ゲームを再開
			return pr_continue;
			break;
		case 1:
			//タイトルに戻る
			return pr_title;
			break;
		default:
			break;
		}
	}
	return pr_pause;
}

//ポーズ中のみ呼ばれる描画
static void pause_draw()
{
	//頂点を用意
	static D3DXVECTOR3 position[4];
	//UV座標を用意
	static D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//背景描画
	{
		//頂点を用意
		position[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		position[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		position[2] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		position[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		//描画
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(ti_pause_pause),
			mbm_none
		);
	}
	//CONTINUEボタン描画
	{
		//サイズ取得
		float w = (float)texture_manager_get_width(g._pause._continue_tex);
		float h = (float)texture_manager_get_height(g._pause._continue_tex);
		//左上と右下を求める
		float left = _coord_continue_x / 1280.0f;
		float top = _coord_continue_y / 720.0f;
		float right = left + w / 1280.0f;
		float bottom = top + h / 720.0f;
		//補正
		left = left * 2.0f - 1.0f;
		top = -(top * 2.0f - 1.0f);
		right = right * 2.0f - 1.0f;
		bottom = -(bottom * 2.0f - 1.0f);
		//頂点を用意
		position[0] = D3DXVECTOR3(left, top, 0.0f);
		position[1] = D3DXVECTOR3(right, top, 0.0f);
		position[2] = D3DXVECTOR3(left, bottom, 0.0f);
		position[3] = D3DXVECTOR3(right, bottom, 0.0f);
		//描画
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(g._pause._continue_tex),
			mbm_alpha
		);
	}
	//TITLEボタン描画
	{
		//サイズ取得
		float w = (float)texture_manager_get_width(g._pause._title_tex);
		float h = (float)texture_manager_get_height(g._pause._title_tex);
		//左上と右下を求める
		float left = _coord_title_x / 1280.0f;
		float top = _coord_title_y / 720.0f;
		float right = left + w / 1280.0f;
		float bottom = top + h / 720.0f;
		//補正
		left = left * 2.0f - 1.0f;
		top = -(top * 2.0f - 1.0f);
		right = right * 2.0f - 1.0f;
		bottom = -(bottom * 2.0f - 1.0f);
		//頂点を用意
		position[0] = D3DXVECTOR3(left, top, 0.0f);
		position[1] = D3DXVECTOR3(right, top, 0.0f);
		position[2] = D3DXVECTOR3(left, bottom, 0.0f);
		position[3] = D3DXVECTOR3(right, bottom, 0.0f);
		//描画
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(g._pause._title_tex),
			mbm_alpha
		);
	}
	//カーソル描画
	{
		//サイズ取得
		float w = (float)texture_manager_get_width(ti_title_title_cursor);
		float h = (float)texture_manager_get_height(ti_title_title_cursor);
		//左上と右下を求める
		float left = g._pause._cursor._draw_x / 1280.0f;
		float top = g._pause._cursor._draw_y / 720.0f;
		float right = left + w / 1280.0f;
		float bottom = top + h / 720.0f;
		//補正
		left = left * 2.0f - 1.0f;
		top = -(top * 2.0f - 1.0f);
		right = right * 2.0f - 1.0f;
		bottom = -(bottom * 2.0f - 1.0f);
		//頂点を用意
		position[0] = D3DXVECTOR3(left, top, 0.0f);
		position[1] = D3DXVECTOR3(right, top, 0.0f);
		position[2] = D3DXVECTOR3(left, bottom, 0.0f);
		position[3] = D3DXVECTOR3(right, bottom, 0.0f);
		//描画
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(ti_title_title_cursor),
			mbm_alpha
		);
	}
}

//ポーズ画面に移行するべきならtrueを返す
static bool to_pause(const virtual_input_t *trigger)
{
	return trigger->_pause;
}

//ステージを開始して音声も鳴らす
static void _stage_setup(stage_index_t index)
{
	stage_setup(index, &g._object_array);
	sound_play(si_se_voice_start);
}

#endif

///-----------------------この状態を制御する主要な関数------------------------------------



extern void game_main_init()
{
	//FBXオブジェクト初期化
	gmfoa_t_init(&g._object_array, 8);
	//きらきらエフェクト初期化
	glitter_manager_init();
	//カメラ初期化
	camera_manager_init();
	//スプライト初期化
	sprite_integration_init();
	//UIを初期化
	stage_ui_init();
	//音を鳴らす
	sound_play(si_bgm_bgm);
	//ステージ開始
	_stage_setup(stage_get_now());
	//イベント管理初期化
	stage_event_init();
	//最初はポーズモードにしない
	g._pause_initialized = false;
	g._pause_mode = false;
}
extern void game_main_end()
{
	//音を止める
	sound_stop(si_bgm_bgm);
	//UI終了
	stage_ui_end();
	//イベント管理終了
	stage_event_end();
	//カメラ終了
	camera_manager_end();
	//きらきらエフェクト終了
	glitter_manager_end();
	//FBXオブジェクト終了
	gmfoa_t_end(&g._object_array);
	//スプライト終了
	sprite_integration_end();
}
extern void game_main_update(magnet_hero_state_t *next_state)
{
	//入力の取得
	const virtual_input_t *trigger = mylib_get_virtual_input_trigger();
	//ポーズ中か
	if (g._pause_mode)
	{
		//ポーズ中
		if (g._pause_initialized == false)
		{
			pause_init();
			g._pause_initialized = true;
		}
		pause_result_t result = pause_update(trigger);
		switch (result)
		{
		case pr_pause:
			//何もしない
			break;
		case pr_continue:
			//ポーズ終了
			pause_end();
			g._pause_initialized = false;
			g._pause_mode = false;
			break;
		case pr_title:
			//ポーズ終了
			pause_end();
			g._pause_initialized = false;
			g._pause_mode = false;
			//とりあえずタイトルへ戻る(todo:時間待ちを実装する)
			mylib_wait_sec(0.5f);
			*next_state = mhs_title;
			break;
		default:
			break;
		}
	}
	else
	{
		//ポーズ中じゃない
		g._pause_mode = to_pause(trigger);
		//FBXオブジェクト更新
		gmfoa_t_update(&g._object_array);
		//きらきらエフェクト更新
		glitter_manager_update();
		//カメラ更新
		camera_manager_update(&g._object_array);
		//ビュー行列とプロジェクション行列を計算&セット
		camera_manager_set_view_and_projection(1.0f, 10000.0f);
		//スプライト更新
		sprite_integration_update();
		//UI更新
		stage_ui_update();
		//イベント管理更新・計算
		stage_event_update();
		//プレーヤーが落下したらステージリセット
		if (holder_falled_player_get() == timeGetTime())
		{
			stage_reset(stage_get_now());
		}
		//ゴールフラグが立っているか
		if (stage_event_get_already_goal())
		{
			//ゴールした瞬間か
			if (stage_event_get_goal_elapsed() == 1)
			{
				//ゴールした瞬間なのでゴール時の音声を鳴らす
				sound_play(si_se_voice_goal);
			}
			//演出が終わっているか
			if (stage_event_get_goal_elapsed() >= _goal_wait_frame)
			{
				//現在のステージを破棄
				stage_destroy(stage_get_now(), &g._object_array);
				//ステージを進める & 全クリ判定
				if (stage_advance())
				{
					//全クリなのでクリア情報を初期化(次に遊んだ時は最初からになってる)
					stage_clear_data_init();
					//エンディングに行く
					*next_state = mhs_ending;
				}
				else
				{
					//ステージ開始
					_stage_setup(stage_get_now());
				}
			}
		}
	}
}
extern void game_main_draw()
{
	if (g._pause_mode)
	{
		//ポーズ中
		pause_draw();
	}
	else
	{
		//管理下にあるFBXオブジェクト描画
		gmfoa_t_draw(&g._object_array);
		//きらきらエフェクト描画
		glitter_manager_draw();
		//プレーヤーの飛ばす手のオブジェ描画
		drawer_player_magnet_hand();
		//プレーヤーの影描画
		drawer_player_shadow_draw();
		//スプライト描画
		sprite_integration_draw();
		//UI描画
		stage_ui_draw(stage_event_get_goal_elapsed(), _goal_wait_frame);
	}
}
