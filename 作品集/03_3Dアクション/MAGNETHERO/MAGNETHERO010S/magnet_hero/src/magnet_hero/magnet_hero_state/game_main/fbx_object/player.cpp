//親ヘッダー
#include "player.h"
//その他のヘッダー
#include "wrench.h"
#include "..\aiming\aiming.h"
#include "..\camera\camera_manager.h"
#include "..\sprite\sprite_integration.h"
#include "..\connecter\info_holder.h"
#include "..\stage_event\stage_data.h"
#include "..\stage_event\stage_index_saver.h"
#include "..\stage_event\stage_event.h"
#include "..\glitter\glitter_manager.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\option\option.h"
#include "..\..\..\..\calc\calc.h"
#include "..\..\..\..\input\input.h"
#include "..\..\..\..\file_reader\file_reader.h"
#include "..\..\..\..\file_data\file_data.h"
#include "..\..\..\..\sound\sound.h"
#include "..\..\..\..\shader_use\simple_shader.h"
#include "..\..\..\..\shader_use\post_wave.h"
#include "..\..\..\..\texture_manager\texture_manager.h"


//ビーム歩き
#define _beam_walk
//ビームの1フレーム目のスクリーン上での奥行き
#define _beam_start_z()				(0.9925f)
//ビームの最終フレームのスクリーン上での奥行き
#define _beam_end_z()				(0.9985f)
//プレーヤーが死亡するy座標
#define _player_dead_line()			(-400.0f)
//デバッグ用の情報を出すオプション
#define _output_debug_info			(0)

//プレイヤーの外部情報
struct player_database_fbx_file_name_t
{
	char *_base;							//基本モデルのパス
	char *_stand;							//直立モーションのパス
	char *_front_walk;						//前進モーションのパス
	char *_back_walk;						//後ろ歩きモーションのパス
	char *_side_walk1;						//横歩きモーションのパス
	char *_side_walk2;						//横歩きモーションのパス
	char *_run;								//走りモーションのパス
	char *_jump;							//ジャンプモーションのパス
	char *_damage;							//被ダメモーションのパス
	char *_fall;							//落ちモーションのパス
};
//プレイヤーの外部情報
struct player_database_status_t
{
	float _height;														//身長
	float _scaling;														//拡大倍率
	float _run_speed;													//走り速度
	float _jump_pow;													//ジャンプ力
	float _gravity;														//重力
	time_t _beam_limit_frame;											//ビームの制限時間
	float _sticking_forward_force;										//ビームの当たった位置まで向かう力
	float _magnetic_repulsion_force_x;									//磁力反発の飛び上がり力(横方向)
	float _magnetic_repulsion_force_y;									//磁力反発の飛び上がり力(縦方向)
	time_t _magnetic_repulsion_duration_frame;							//磁力反発の飛び上がりベクトルが0になるまでの時間(フレーム)
	float _qte_success_magnetic_repulsion_force_x;						//磁力反発の飛び上がり力(横方向)
	float _qte_success_magnetic_repulsion_force_y;						//磁力反発の飛び上がり力(縦方向)
	time_t _qte_success_magnetic_repulsion_duration_frame;				//磁力反発の飛び上がりベクトルが0になるまでの時間(フレーム)
	int _jump_count_max;												//ジャンプ可能な回数(1固定、2以上だと空中ジャンプができる、1未満だとジャンプできない)
	float _floating_amount_up;											//浮遊アクションで上に行く量
	time_t _floating_time;												//浮遊アクションで上に行ききるまでの時間(フレーム)
	time_t _floating_time_limit;										//浮遊アクションの制限時間(フレーム)
	float _beam_shift_x;												//原点からビーム発射位置までのベクトル(x)
	float _beam_shift_y;												//原点からビーム発射位置までのベクトル(y)
	float _beam_shift_z;												//原点からビーム発射位置までのベクトル(z)
	float _beam_thickness;												//ビームの太さ
	bool _beam_thickness_display;										//ビームの太さを照準カーソルにも反映する
	float _beam_extensive_thickness;									//エイムアシストの範囲
};
//プレイヤーの外部情報
struct player_database_t
{
	player_database_fbx_file_name_t _fbx_file_name;
	player_database_status_t _status;
};
//ファイルから読み込んだ情報が格納されてるデータベース構造体のポインタを返す
static const player_database_t *get_database()
{
	static player_database_t database;
	static bool should_read = true;
	if (should_read)
	{
		should_read = false;
		player_database_status_t *status = &database._status;
		const datum_t *data = file_data_get(fdi_player_database, nullptr);
		status->_height = (data++)->_Float;
		status->_scaling = database._status._height / 10.0f;
		status->_run_speed = (data++)->_Float;
		status->_jump_pow = (data++)->_Float;
		status->_gravity = (data++)->_Float * -1.0f;
		status->_beam_limit_frame = (data++)->_Int;
		status->_sticking_forward_force = (data++)->_Float;
		status->_magnetic_repulsion_force_x = (data++)->_Float;
		status->_magnetic_repulsion_force_y = (data++)->_Float;
		status->_magnetic_repulsion_duration_frame = (time_t)((data++)->_Int);
		status->_qte_success_magnetic_repulsion_force_x = (data++)->_Float;
		status->_qte_success_magnetic_repulsion_force_y = (data++)->_Float;
		status->_qte_success_magnetic_repulsion_duration_frame = (time_t)((data++)->_Int);
		status->_jump_count_max = (data++)->_Int;
		status->_floating_amount_up = (data++)->_Float;
		status->_floating_time = (time_t)((data++)->_Int);
		status->_floating_time_limit = (time_t)((data++)->_Int);
		status->_beam_shift_x = (data++)->_Float;
		status->_beam_shift_y = (data++)->_Float;
		status->_beam_shift_z = (data++)->_Float;
		status->_beam_thickness = (data++)->_Float;
		status->_beam_thickness_display = (data++)->_Bool;
		status->_beam_extensive_thickness = 500.0f;
	}
	return &database;
}
//ステートID
enum player_state_id_t
{
	psi_none,						//ステートなし
	psi_normal,						//通常状態(移動やジャンプができる)
	psi_sticking_migration,			//くっつき移行状態(ビ~~ムが当たった後)
	psi_sticking_stop,				//くっつき停止状態(くっつき移行状態が終わった後)
	psi_max,						//最大
};

#if _output_debug_info
//ステートごとの名前
static const char *state_name(player_state_id_t state)
{
	static const char *name_table[(size_t)(psi_max)] =
	{
		"なし",
		"通常",
		"くっつき(移行)",
		"くっつき",
	};
	return name_table[(size_t)(state)];
}
#endif

//アクションID
enum player_action_id_t
{
	pai_none,								//アクションなし
	pai_run,								//走り
	pai_jump,								//ジャンプ
	pai_beam,								//ビ~~ムを撃っている
	pai_horizontal_blow,					//横吹っ飛び
	pai_floating,							//浮遊
	pai_max,								//最大
};

//アクションごとの名前
#if _output_debug_info
static const char *action_name(player_action_id_t id)
{
	static const char *name_table[(size_t)(pai_max)] =
	{
		"none",
		"run",
		"jump",
		"beam",
		"horizontal blow",
		"floating",
	};
	return name_table[(size_t)(id)];
}
#endif

//入力情報から行動を判断するプレイヤーの脳
struct player_brain_t
{
	bool _should_run;							//走るべき
#ifdef _beam_walk
	bool _should_beam_walk;						//歩くべき
#endif
	float _input_force;							//入力の力(0~1)
	float _input_angle;							//入力されてる角度
	bool _should_jump;							//ジャンプするべき
	bool _should_beam_fire;						//ビームを発射するべき
	bool _should_beam;							//ビームを持続させるべき
	bool _should_magnetic_jump;					//磁力反発ジャンプをするべき
	bool _should_floating;						//磁力浮遊アクションをするべき
};

//プレイヤー専用のパラメーター
struct player_parameter_t
{
	player_brain_t _brain;																	//脳
	player_database_status_t _status;														//データベース上のステータス
	player_state_id_t _now_state;															//現在のステート
	player_state_id_t _next_state;															//次回更新時にチェンジするステート
	bool _doing[(size_t)(pai_max)];															//特定のアクションを実行しているか
	time_t _doing_frame[(size_t)(pai_max)];													//特定のアクションを実行している時間(フレーム)
	int _jump_count;																		//ジャンプした回数
	float _jump_move_y;																		//ジャンプの移動量
	D3DXVECTOR3 _beam_start_position;														//ビ~~ムの発射位置
	D3DXVECTOR3 _beam_end_position;															//ビ~~ムの目標位置
	float _beam_t;																			//ビ~~ムの時間(0~1)
	D3DXVECTOR3 _sticking_position_before;													//くっつき移行前の位置
	D3DXVECTOR3 _sticking_position_after;													//くっつき移行先の位置
	float _horizontal_blow_angle;															//横吹っ飛び角度
	D3DXVECTOR2 _horizontal_blow_move_vec;													//横吹っ飛び移動ベクトル
	time_t _horizontal_blow_duration_frame;													//横吹っ飛び移動ベクトルが0になるまでの時間(フレーム)
	time_t _horizontal_blow_fire_moment;													//横吹っ飛びをした瞬間
	D3DXVECTOR3 _floating_start_position;													//浮遊開始位置
	bool _in_air;																			//空中にいる
	int _remaining_num;																		//残機
	game_main_fbx_object_handle_t *_beam_hitting_object_handle;								//ビ~~ムと当たったマップのハンドル
	bool _beam_hitting;																		//ビ~~ムとマップが衝突しているか
	int _beam_hitting_polygon_id;															//ビ~~ムと当たったマップのポリゴンID
	int _beam_hitting_mesh_id;																//ビ~~ムと当たったマップのメッシュID
	game_main_fbx_object_handle_t *_beam_hitting_prediction_object_handle;					//ビ~~ムと当たったマップのハンドル
	bool _beam_hitting_prediction;															//ビ~~ムとマップの衝突予測が成功しているか
	int _beam_hitting_prediction_polygon_id;												//ビ~~ムとマップの衝突予測に当たったマップのポリゴンID
	int _beam_hitting_prediction_mesh_id;													//ビ~~ムとマップの衝突予測に当たったマップのメッシュID
	game_main_fbx_object_handle_t *_beam_hitting_extensive_prediction_object_handle;		//ビ~~ムと当たったマップのハンドル
	bool _beam_hitting_extensive_prediction;												//ビ~~ムとマップの広範囲衝突予測が成功しているか
	int _beam_hitting_extensive_prediction_polygon_id;										//ビ~~ムとマップの広範囲衝突予測に当たったマップのポリゴンID
	int _beam_hitting_extensive_prediction_mesh_id;											//ビ~~ムとマップの広範囲衝突予測に当たったマップのメッシュID
	D3DXVECTOR3 _shadow_position;															//丸影の位置
	D3DXVECTOR3 _shadow_normal;																//丸影の法線
	D3DXVECTOR3 _wrench_position;															//レンチの位置
#ifdef _beam_walk
	bool _beam_walk_mode_on;																//ビ~~ム歩き
#endif
#if _output_debug_info
	bool _fordbg_draw_beam_ray;																//デバッグ用のビ~~ムレイを描画するかどうかのフラグ
	bool _fordbg_draw_ground_ray;															//デバッグ用の地面衝突レイを描画するかどうかのフラグ
	D3DXVECTOR3 _fordbg_ray_position;														//地面とのあたり判定に使っているレイの始点(デバッグ用)
	D3DXVECTOR3 _fordbg_ray_vector;															//地面とのあたり判定に使っているレイのベクトル(デバッグ用)
	D3DXVECTOR3 _fordbg_beam_ray_position;													//ビ~~ムのあたり判定に使っているレイの始点(デバッグ用)
	D3DXVECTOR3 _fordbg_beam_ray_vector;													//ビ~~ムのあたり判定に使っているレイのベクトル(デバッグ用)
	D3DXVECTOR3 _fordbg_beam_intersect;														//ビ~~ムのあたり判定に使っているレイと当たった奴との交点(デバッグ用)
	D3DXVECTOR3 _fordbg_beam_target_position;												//ビ~~ムの目標位置(デバッグ用)
	D3DXVECTOR3 _fordbg_ray2_position;														//2本目のレイの位置
	D3DXVECTOR3 _fordbg_ray2_vector;														//2本目のレイのベクトル
	D3DXVECTOR3 _fordbg_ray2_intersect;														//2本目のレイの交点
	float _fordbg_ray2_sub_length;															//2本目のレイの引き算した長さ
#endif
};


//コントローラーからの入力を受け取って処理
static void brain_process(player_brain_t *brain)
{
	//入力の取得
	const virtual_input_t *repeat = mylib_get_virtual_input_repeat();
	const virtual_input_t *trigger = mylib_get_virtual_input_trigger();
	//上下左右の入力ベクトルを作る(2次元)
	D3DXVECTOR2 input_vector = D3DXVECTOR2(0.0f, 0.0f);
	//x方向
	input_vector.x += repeat->_lstick_x;
	if (fabsf(input_vector.x) <= 0.0f)
	{
		input_vector.x += (float)((int)(repeat->_lkey_right));
		input_vector.x -= (float)((int)(repeat->_lkey_left));
	}
	//y方向
	input_vector.y -= repeat->_lstick_y;
	if (fabsf(input_vector.y) <= 0.0f)
	{
		input_vector.y += (float)((int)(repeat->_lkey_up));
		input_vector.y -= (float)((int)(repeat->_lkey_down));
	}
	//ジャンプの入力チェック
	brain->_should_jump = trigger->_b;
	//マグネットビ~~ムの発射をするか入力チェック
	brain->_should_beam_fire = trigger->_a;
	//マグネットビ~~ムを持続させるか入力チェック(単発押しで最後までいくように仕様変更されました)
	brain->_should_beam = true;
	//磁力反発ジャンプアクションの入力チェック
#ifndef qte_lost
	brain->_should_magnetic_jump = true;
#else
	brain->_should_magnetic_jump = trigger->_a;
#endif
	//浮遊アクションの入力チェック
	brain->_should_floating = false;
	//入力の角度
	if (input_vector.x || input_vector.y)
	{
		brain->_input_angle = atan2f(input_vector.y, -input_vector.x);
	}
	//入力ベクトルの大きさ
	float input_vector_length = sqrtf(input_vector.x * input_vector.x + input_vector.y * input_vector.y);
	//入力情報から行動を判断
	brain->_should_run = input_vector_length >= 0.1;
	brain->_input_force = input_vector_length;
#ifdef _beam_walk
	//歩くべき
	brain->_should_beam_walk = brain->_should_beam;
#endif
}

#if 1
//磁力吹っ飛びをしてからあまり時間が立っていないことを判定する関数
static bool not_much_time_has_come(const game_main_fbx_object_t *object, const player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	float Threshold = mylib_frame_to_second(param->_horizontal_blow_duration_frame, 60.0f) / 5.0f;
	return (timeGetTime() - param->_horizontal_blow_fire_moment) / 1000.0f < Threshold;
}

//走り処理
static void run_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	bool should_run = brain->_should_run;
#ifdef _beam_walk
	//歩くべきかつビ~~ムを撃っている間は走るべきではない
	if (param->_beam_walk_mode_on && param->_doing[pai_beam])
	{
		should_run = false;
	}
#endif
	//磁力吹っ飛びの少しの間は走るべきではない
	if (not_much_time_has_come(object, param))
	{
		should_run = false;
	}
	//インデックスを取得
	size_t action_index = (size_t)(pai_run);
	//カメラの横方向の角度
	float camera_phi = camera_manager_get_phi();
	//今フレームの走る方向
	float run_angle = camera_phi - brain->_input_angle - (float)(M_PI_2);
	//フラグリセット
	param->_doing[action_index] = false;
	//走るべきか
	if (should_run)
	{
		//位置更新
		object->_position.x += brain->_input_force * param->_status._run_speed * cosf(run_angle);
		object->_position.z += brain->_input_force * param->_status._run_speed * sinf(run_angle);
		//角度も更新
		object->_rotation.y = -camera_phi + brain->_input_angle - (float)(M_PI);
		//フラグセット
		param->_doing[action_index] = true;
	}
	//カウント
	if (param->_doing[action_index])
	{
		param->_doing_frame[action_index]++;
	}
	else
	{
		param->_doing_frame[action_index] = 0;
	}
}

//ジャンプできるかチェック
static bool can_jump(const player_parameter_t *param)
{
	//ジャンプ回数は適正か
	if (param->_jump_count >= param->_status._jump_count_max)
	{
		//ジャンプできない
		return false;
	}
	//ジャンプできる
	return true;
}

//ジャンプする
static void do_jump(player_parameter_t *param, float jump_pow)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_jump);
	//フラグをセット
	param->_doing[action_index] = true;
	//時間をセット
	param->_doing_frame[action_index] = 0;
	//移動量をセット
	param->_jump_move_y = jump_pow;
	//カウントアップ
	param->_jump_count++;
	//空中にいる
	param->_in_air = true;
	//時間を渡す
	player_do_jump_moment_set(mylib_now_sec());
}

//着地
static void landing(player_parameter_t *param)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_jump);
	//ジャンプの移動量はゼロ
	param->_jump_move_y = 0.0f;
	//フラグをセット
	param->_doing[action_index] = false;
	//時間をセット
	param->_doing_frame[action_index] = 0;
	//ジャンプ回数をリセット
	param->_jump_count = 0;
	//空中にいない
	param->_in_air = false;
}

//ジャンプ処理
static void jump_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(object);
	//インデックスを取得
	size_t action_index = (size_t)(pai_jump);
	//ジャンプするべきか
	if (brain->_should_jump)
	{
		//ジャンプできるか
		if (can_jump(param))
		{
			//ジャンプする
			do_jump(param, param->_status._jump_pow);
			//音を鳴らす
			sound_play(si_se_jump);
		}
	}
	//カウントアップ
	if (param->_doing[action_index])
	{
		param->_doing_frame[action_index]++;
	}
}

//横吹っ飛びをする
static void do_horizontal_blow(player_parameter_t *param, float move_pow, time_t move_duration, float move_angle)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_horizontal_blow);
	//横吹っ飛び力を求める
	D3DXVECTOR2 move_vec = D3DXVECTOR2(
		move_pow * sinf(move_angle),
		move_pow * cosf(move_angle)
	);
	//求めた横吹っ飛び力を使う
	param->_horizontal_blow_move_vec = move_vec;
	//フラグをセット
	param->_doing[action_index] = true;
	//時間をセット
	param->_doing_frame[action_index] = 0;
	param->_horizontal_blow_duration_frame = move_duration;
	param->_horizontal_blow_fire_moment = timeGetTime();
}

//横吹っ飛び中に実行する処理
static void horizontal_blow_process(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_horizontal_blow);
	//横吹っ飛び中か
	if (param->_doing[action_index])
	{
		//横吹っ飛びの処理を続行すべきか
		if (param->_doing_frame[action_index] < param->_horizontal_blow_duration_frame)
		{
			//続行すべきである
			float t =
				(float)(param->_doing_frame[action_index]) /
				(float)(param->_horizontal_blow_duration_frame);
			D3DXVECTOR2 move_vec_2d = param->_horizontal_blow_move_vec * (1.0f - t);
			D3DXVECTOR3 move_vec_3d = D3DXVECTOR3(move_vec_2d.x, 0.0f, move_vec_2d.y);
			object->_position += move_vec_3d;
			//時間を更新
			param->_doing_frame[action_index]++;
		}
		else
		{
			//もうやめるべきである
			param->_doing[action_index] = false;
			//時間をセット
			param->_doing_frame[action_index] = 0;
		}
	}
}

//浮遊を開始してからnフレーム目の浮遊開始位置から見た移動量を求める
static float floating_position(player_parameter_t *param, time_t n)
{
	//計算して返す
	//時間を0~1に変換
	float t = (float)(n) / (float)(param->_status._floating_time);
	t = min(t, 1.0f);
	return t * param->_status._floating_amount_up;
}

//浮遊を開始してからnフレーム目のy座標の移動量を求める
static float floating_move_amount(player_parameter_t *param, time_t n)
{
	//移動量 = 次のフレームの位置 - 今のフレームの位置
	return floating_position(param, n + 1) - floating_position(param, n);
}

//浮遊できるかチェック
static bool can_floating(const player_parameter_t *param)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_floating);
	//時間チェック
	if (param->_doing_frame[action_index] >= param->_status._floating_time_limit)
	{
		//浮遊できない
		return false;
	}
	//浮遊できる
	return true;
}

//浮遊する
static void do_floating(player_parameter_t *param)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_floating);
	//浮遊フラグをオン
	param->_doing[action_index] = true;
	//浮遊カウンタを更新
	param->_doing_frame[action_index]++;
	//ジャンプの移動量を打ち消す
	param->_jump_move_y = 0.0f;
	//空中にいる
	param->_in_air = true;
}

//浮遊処理
static void floating_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//磁力吹っ飛びの少しの間は浮遊するべきではない
	bool should_floating = brain->_should_floating;
	if (not_much_time_has_come(object, param))
	{
		should_floating = false;
	}
	//インデックスを取得
	size_t action_index = (size_t)(pai_floating);
	//浮遊フラグリセット
	param->_doing[action_index] = false;
	//浮遊するべきか
	if (should_floating)
	{
		//浮遊できるか
		if (can_floating(param))
		{
			//浮遊する
			do_floating(param);
			//浮遊開始の瞬間だけ音を出す
			if (param->_doing_frame[action_index] == 1)
			{
				sound_play(si_se_maghover);
			}
		}
	}
	else
	{
		//空中にいるかチェック
		if (param->_in_air == false)
		{
			//時間をリセット
			param->_doing_frame[action_index] = 0;
		}
	}
	//浮遊フラグチェック
	if (param->_doing[action_index])
	{
		//浮遊時間チェック
		if (param->_doing_frame[action_index] < param->_status._floating_time_limit)
		{
			//移動量を求める
			float move_amount = floating_move_amount(param, param->_doing_frame[pai_floating]);
			//移動量の分だけ移動
			object->_position.y += move_amount;
		}
	}
}

//重力を無視するべきか判定する
static bool ignore_gravity(player_parameter_t *param)
{
	//浮遊中
	if (param->_doing[(size_t)(pai_floating)])
	{
		//無視するべき
		return true;
	}
	//無視するべきではない
	return false;
}

//重力処理
static void gravity_process(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//重力を無視するタイミングなら何もしない
	if (ignore_gravity(param))
	{
		return;
	}
	//y座標更新
	object->_position.y += param->_jump_move_y;
	//重力分だけ加算
	param->_jump_move_y += param->_status._gravity;
}

//磁力反発をする
static void do_magnetic_jump(game_main_fbx_object_t *object, player_parameter_t *param, time_t move_duration, float force_x, float force_y)
{
	//ジャンプ
	do_jump(param, force_y);
	//横移動
	do_horizontal_blow(param, force_x, move_duration, param->_horizontal_blow_angle);
	//角度を変える
	object->_rotation.y = param->_horizontal_blow_angle + (float)(M_PI);
	//ジャンプ回数を1にする
	param->_jump_count = 1;
	//浮遊カウンタをゼロに
	param->_doing_frame[(size_t)(pai_floating)] = 0;
}

//地面とのあたり判定&位置修正&着地処理
//同時に影の位置も計算します
static void ground_hit_process(game_main_fbx_object_t *object, const player_brain_t *brain, player_parameter_t *param)
{
	float y_shift_base = 6.0f;
	//地面のオブジェクトを見つける
	game_main_fbx_object_handle_t *map;
	//床との判定
	{
		gmfoa_t_search_kind(object->_parrent_array, gmfok_stage_ground, nullptr, &map);
		//レイ発射位置を計算
		D3DXVECTOR3 ray_position = object->_position;
		float y_shift = y_shift_base + max(-param->_jump_move_y, 0.0f);
		ray_position.y += y_shift;
		D3DXVECTOR3 intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		D3DXVECTOR3 normal = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		D3DXVECTOR3 ray_vector = D3DXVECTOR3(0.0f, -500.0f, 0.0f);
		{
			//レイを飛ばす
			bool hitting = false;
			gmfoh_t_raycast(map, &ray_position, &ray_vector, &intersect, &normal, nullptr, &hitting, nullptr);
			//飛ばしたレイがマップと当たれば位置更新
			if (hitting)
			{
				//プレーヤーが落下中かつプレーヤーの足が地面よりも下
				if ((param->_jump_move_y <= 0.0f) && (object->_position.y <= intersect.y))
				{
					//位置更新
					object->_position.y = intersect.y;
					//着地処理
					landing(param);
				}
			}
			//影の位置、法線決定
			param->_shadow_position = intersect;
			param->_shadow_normal = normal;
#if _output_debug_info
			//デバッグ用に情報をセット
			param->_fordbg_ray_position = ray_position;
			param->_fordbg_ray_vector = ray_vector;
#endif
		}
	}
	//壁との判定
	{
		//レイ発射位置を計算
		D3DXVECTOR3 ray_position = object->_position;
		ray_position.y += y_shift_base;
		//自分の位置から自分の進行方向に向かってレイを飛ばす
		float pl_angle = camera_manager_get_phi() - brain->_input_angle - (float)(M_PI_2);
		D3DXVECTOR3 ray_vector = D3DXVECTOR3(cosf(pl_angle), 0.0f, sinf(pl_angle)) * 100.0f;
		D3DXVECTOR3 intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		D3DXVECTOR3 normal = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		bool hitting = false;
		gmfoh_t_raycast(map, &ray_position, &ray_vector, &intersect, &normal, nullptr, &hitting, nullptr);
#if _output_debug_info
		//デバッグ用情報保存
		param->_fordbg_ray2_position = ray_position;
		param->_fordbg_ray2_vector = ray_vector;
#endif
		//飛ばしたレイが何かと当たったらそれは壁である
		if (hitting)
		{
			//発射位置から交点までのベクトル
			D3DXVECTOR3 sub = intersect - ray_position;
#if _output_debug_info
			param->_fordbg_ray2_intersect = intersect;
			param->_fordbg_ray2_sub_length = D3DXVec3Length(&sub);
#endif
			//その長さが一定以下であるか
			float threshold = 4.0f;
			if (D3DXVec3LengthSq(&sub) <= threshold * threshold)
			{
				//単位化
				D3DXVECTOR3 norm;
				D3DXVec3Normalize(&norm, &sub);
				//自分の位置を修正
				intersect.y -= y_shift_base;
				object->_position = intersect + norm * D3DXVec3Dot(&norm, &normal) * threshold;
			}
		}
	}
}

//地面とのあたり判定を取らず影の位置だけ計算する
static void calc_shadow_position(const game_main_fbx_object_t *object, player_parameter_t *param)
{
	//地面のオブジェクトを見つける
	game_main_fbx_object_handle_t *map;
	gmfoa_t_search_kind(object->_parrent_array, gmfok_stage_ground, nullptr, &map);
	//レイ発射位置を計算
	D3DXVECTOR3 ray_position = object->_position;
	float y_shift = 8.0f;
	ray_position.y += y_shift;
	D3DXVECTOR3 intersect = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 normal = {};
	D3DXVECTOR3 ray_vector = D3DXVECTOR3(0.0f, -500.0f, 0.0f);
	//レイを飛ばす
	bool hitting = false;
	gmfoh_t_raycast(map, &ray_position, &ray_vector, &intersect, &normal, nullptr, &hitting, nullptr);
	//影の位置、法線決定
	param->_shadow_position = intersect;
	param->_shadow_normal = normal;
#if _output_debug_info
	//デバッグ用に情報をセット
	param->_fordbg_ray_position = ray_position;
	param->_fordbg_ray_vector = ray_vector;
#endif
}

//ビ~~ムを撃つ処理
//この関数内で、今フレームのビームの発射位置と終了位置を計算します
//描画用のレンチの位置も計算します
static void beam_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//インデックスを取得
	size_t action_index = (size_t)(pai_beam);
	//ビ~~ムを撃つべきか
	if (brain->_should_beam_fire)
	{
		//現状、ビ~~ムを撃っていないか
		if (param->_doing[action_index] == false)
		{
			//フラグを切り替え
			param->_doing[action_index] = true;
			//音を出す
			sound_play(si_se_beam);
			//スプライトでびりびりを出す
			sprite_integration_bibibi_fire(10, 10);
			//シェーダーに時間を渡す
			post_wave_set_origin(mylib_frame_to_second(mylib_get_frame_count(), 60.0f));
		}
	}
	//ビ~~ムを持続させるフラグが立っているかどうか取得
	bool should_beam = brain->_should_beam;
	//ビ~~ムの制限時間を超えているか
	if (param->_doing_frame[action_index] >= param->_status._beam_limit_frame)
	{
		//ビ~~ムを持続させるべきではない
		should_beam = false;
	}
	//ビ~~ムを発射させるフラグが立っている
	if (param->_doing[action_index])
	{
		//ビ~~ムを持続させるべき
		if (should_beam)
		{
			//今フレームのビ~~ムの発射位置を計算
			float cursor_x = 0.0f;
			float cursor_y = 0.0f;
			float cursor_z = _beam_start_z();
			D3DXVECTOR3 beam_start_screen_pos = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&param->_beam_start_position, &beam_start_screen_pos);
			//今フレームのビ~~ムの終了位置を計算
			aiming_cursor_get_pos(&cursor_x, &cursor_y, nullptr);
			param->_beam_t = (float)(param->_doing_frame[action_index] + 1) / (float)(param->_status._beam_limit_frame);
			cursor_z = lerpf(_beam_start_z(), _beam_end_z(), param->_beam_t);
			D3DXVECTOR3 beam_end_screen_pos = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&param->_beam_end_position, &beam_end_screen_pos);
			{
				//カメラの角度を取得
				float camera_phi = camera_manager_get_phi();
				//float camera_theta = camera_manager_get_theta();
				//足元から発射位置までのローカルベクトルを取得
				D3DXVECTOR3 vec3 = D3DXVECTOR3(
					param->_status._beam_shift_x * param->_status._scaling,
					param->_status._beam_shift_y * param->_status._scaling,
					param->_status._beam_shift_z * param->_status._scaling
				);
				//カメラの角度によって回転させる
				D3DXVECTOR4 vec4;
				D3DXMATRIX camera_mat;
				D3DXMatrixRotationY(&camera_mat, -camera_phi);
				D3DXVec3Transform(&vec4, &vec3, &camera_mat);
				vec3 = D3DXVECTOR3(vec4.x, vec4.y, vec4.z);
				//レンチの開始位置を計算
				D3DXVECTOR3 start_pos = object->_position + vec3;
				//レンチの終了位置を計算
				D3DXVECTOR3 end_pos = param->_beam_end_position;
				//レンチの位置を計算
				D3DXVec3Lerp(&param->_wrench_position, &start_pos, &end_pos, param->_beam_t);
			}
#if _output_debug_info
			//デバッグ用に情報保存
			param->_fordbg_beam_target_position = param->_beam_end_position;
#endif
		}
		else
		{
			//ビームを持続するべきではないので、フラグを立てない
			param->_doing[action_index] = false;
		}
	}
	//ビ~~ムカウンタを更新
	if (param->_doing[action_index])
	{
		param->_doing_frame[action_index]++;
	}
	else
	{
		param->_doing_frame[action_index] = 0;
	}
}

#ifdef _beam_walk
static void change_beam_walk_mode(player_parameter_t *param, const player_brain_t *brain)
{
	param->_beam_walk_mode_on = brain->_should_beam_walk;
}

static void beam_walk_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//ビ~~ム歩きモードがオフなら何もしない
	if (param->_beam_walk_mode_on == false)
	{
		return;
	}
	//ビームを撃っていなければ何もしない
	if (param->_doing[(size_t)(pai_beam)] == false)
	{
		return;
	}
	//カメラの横方向の角度
	float camera_phi = camera_manager_get_phi();
	//プレイヤーが歩くべき方向を求める
	float walk_angle = camera_phi - brain->_input_angle - (float)(M_PI_2);
	//歩くべき方向に歩く
	object->_position.x += brain->_input_force * cos(walk_angle) * param->_status._run_speed / 2.5f;
	object->_position.z += brain->_input_force * sin(walk_angle) * param->_status._run_speed / 2.5f;
	//角度を変える
	object->_rotation.y = -camera_phi - (float)(M_PI_2);
}
#endif

//ビ~~ムとマップのあたり判定
static void check_beam_hit_map(
	game_main_fbx_object_t *object,
	D3DXVECTOR3 *start_pos,
	D3DXVECTOR3 *end_pos,
	float thickness,
	game_main_fbx_object_handle_t **map,
	bool *hitting,
	int *polygon_id,
	int *mesh_id,
	D3DXVECTOR3 *intersect,
#if _output_debug_info
	D3DXVECTOR3 *normal,
	D3DXVECTOR3 *dbg_ray_position, D3DXVECTOR3 *dbg_ray_vector, D3DXVECTOR3 *dbg_intersect
#else
	D3DXVECTOR3 *normal
#endif
)
{
	//レイの開始位置から終了位置までのベクトル
	D3DXVECTOR3 ray_vector = *end_pos - *start_pos;
	//相手を探す
	if (gmfoa_t_search_kind(object->_parrent_array, gmfok_stage_magnet, nullptr, map))
	{
		if (normal)
		{
			gmfoh_t_sphere_cast(*map, start_pos, &ray_vector, thickness, intersect, normal, polygon_id, hitting, mesh_id);
		}
		else
		{
			D3DXVECTOR3 _normal = {};
			gmfoh_t_sphere_cast(*map, start_pos, &ray_vector, thickness, intersect, &_normal, polygon_id, hitting, mesh_id);
		}
	}
#if _output_debug_info
	//デバッグ用に情報をセット
	if (dbg_ray_position)
	{
		*dbg_ray_position = *start_pos;
	}
	if (dbg_ray_vector)
	{
		*dbg_ray_vector = ray_vector;
	}
	if (dbg_intersect)
	{
		*dbg_intersect = *intersect;
	}
#endif
}

//ビ~~ムとマップのあたり判定を取るべきか返す
static bool should_check_beam_hit_map(const player_parameter_t *param)
{
	return param->_doing[(size_t)(pai_beam)];
}

//くっつき移行処理
static void sticking_migration_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(brain);
	//移動量を求める
	D3DXVECTOR3 normalized;
	D3DXVECTOR3 not_normalized = D3DXVECTOR3(param->_sticking_position_after - object->_position);
	D3DXVec3Normalize(&normalized, &not_normalized);
	D3DXVECTOR3 movevec = normalized * param->_status._sticking_forward_force;
	//位置更新
	object->_position += movevec;
}

//くっつき移行処理が終わったことを判定
static bool sticking_migration_ended(game_main_fbx_object_t *object, player_parameter_t *param)
{
	D3DXVECTOR3 vec1 = D3DXVECTOR3(object->_position - param->_sticking_position_before);
	D3DXVECTOR3 vec2 = D3DXVECTOR3(param->_sticking_position_after - param->_sticking_position_before);
	float t = D3DXVec3LengthSq(&vec1);
	float threshold = D3DXVec3LengthSq(&vec2);
	return t > threshold;
}

//くっつき停止処理
static void sticking_stop_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(param);
	//回転
	if (brain->_should_run)
	{
		object->_rotation.y = -camera_manager_get_phi() + brain->_input_angle - (float)(M_PI);
	}
}

//プレイヤーが落っこちたことを判定する関数
static bool is_fell(const game_main_fbx_object_t *object)
{
	return object->_position.y < -1000.0f;
}

//落っこちたら復活する処理
static void respawn_if(game_main_fbx_object_t *object, player_parameter_t *param, bool fire)
{
	if (fire)
	{
		//復活位置を計算
		D3DXVECTOR3 respawn_position;
		stage_data_get_start_point(stage_get_now(), &respawn_position);
		//復活位置に動かす
		object->_position = respawn_position;
		//ジャンプの回数をリセット
		param->_jump_count = min(param->_jump_count, 1);
		//ジャンプの移動量をリセット
		param->_jump_move_y = 0.0f;
		//状態をリセット
		param->_next_state = psi_normal;
		//残機を減らす
		param->_remaining_num--;
		//落下したことを伝える
		holder_falled_player_record();
	}
}

//きらきらエフェクトをn個出す
static void generate_glitter(const game_main_fbx_object_t *object, const player_parameter_t *param, int n)
{
	for (int i = 0; i < n; i++)
	{
		//移動量
		D3DXVECTOR3 v(
			uniform_randf(-1.0f, 1.0f),
			uniform_randf(-1.0f, 1.0f),
			uniform_randf(-1.0f, 1.0f)
		);
		v *= 50.0f;
		//開始位置
		D3DXVECTOR3 pos = object->_position;
		pos.y += param->_jump_move_y + 8.0f;
		//終了位置
		D3DXVECTOR3 end_pos = pos + v;
		//きらきらエフェクトを出すための情報をまとめる
		const float scale_max = 25.0f;
		const float scale_min = 1.0f;
		const float life_sec_max = 3.5f;
		const float life_sec_min = 0.5f;
		const float scale_base = uniform_randf(0.0f, 1.0f);
		const float scale = scale_base * (scale_max - scale_min) + scale_min;
		const float life_sec = (1.0f - scale_base) * (life_sec_max - life_sec_min) + life_sec_min;
		float point = 2.0f;
		const float red = uniform_randf(0.0f, 1.0f);
		point -= red;
		const float green = uniform_randf(0.0f, 1.0f);
		point -= green;
		point = min(point, 1.0f);
		const float blue = uniform_randf(0.0f, point);
		const glitter_add_t info = {
			gk_random(),
			life_sec,
			scale,
			et_EaseOutRandom(ioel_include_linear),
			pos,
			end_pos,
			red,
			green,
			blue
		};
		//きらきらエフェクトを出す
		glitter_manager_add(&info);
	}
}

//磁力命中時の音声を流す
static void sound_play_hit_beam()
{
	sound_index_t index = si_se_voice_magjump;
	int index_add = rand() % 2;
	index = (sound_index_t)((int)(index)+index_add);
	sound_play(index);
}
#endif

//全状態において行われる事前処理(ステートごとの更新前に呼ばれる)
static void state_common_pre_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(brain);
	//'S'、'K'、'I'を同時押しでゴールにワープ(デバッグ用の機能)
	if (
		input_keyboard_repeat(DIK_S) &&
		input_keyboard_repeat(DIK_K) &&
		input_keyboard_repeat(DIK_I)
		)
	{
		stage_data_get_goal_point(stage_get_now(), &object->_position);
		object->_position.y += 0.5f;
	}
	//ビームの衝突予測
	{
		bool beam_hitting_prediction;
		float cursor_x = 0.0f;
		float cursor_y = 0.0f;
		float cursor_z = _beam_start_z();
		D3DXVECTOR3 beam_intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		game_main_fbx_object_handle_t *map = nullptr;
		int polygon_id = -1;
		int mesh_id = -1;
		D3DXVECTOR3 start_position;
		D3DXVECTOR3 beam_screen_pos_start = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
		mylib_to_world_pos(&start_position, &beam_screen_pos_start);
		D3DXVECTOR3 end_position;
		aiming_cursor_get_pos(&cursor_x, &cursor_y, nullptr);
		cursor_z = _beam_end_z();
		D3DXVECTOR3 beam_screen_pos_end = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
		mylib_to_world_pos(&end_position, &beam_screen_pos_end);
		float thickness = 0.0;
		if (param->_status._beam_thickness_display)
		{
			thickness = param->_status._beam_thickness;
		}
		check_beam_hit_map(
			object,
			&start_position,
			&end_position,
			thickness,
			&map,
			&beam_hitting_prediction,
			&polygon_id,
			&mesh_id,
			&beam_intersect,
#if _output_debug_info
			nullptr,
			nullptr,
			nullptr,
			nullptr
#else
			nullptr
#endif
		);
		if (beam_hitting_prediction)
		{
			param->_beam_hitting_prediction_object_handle = map;
			param->_beam_hitting_prediction = true;
			param->_beam_hitting_prediction_polygon_id = polygon_id;
			param->_beam_hitting_prediction_mesh_id = mesh_id;
		}
		else
		{
			param->_beam_hitting_prediction_object_handle = nullptr;
			param->_beam_hitting_prediction = false;
			param->_beam_hitting_prediction_polygon_id = -1;
			param->_beam_hitting_prediction_mesh_id = -1;
		}
		//照準カーソルをオレンジ色にする
		player_beam_hit_object_set(beam_hitting_prediction);
	}
	//ビームの広範囲衝突予測
	{
		bool beam_hitting_extensive_prediction = false;
		float cursor_x = 0.0f;
		float cursor_y = 0.0f;
		float cursor_z = _beam_start_z();
		D3DXVECTOR3 beam_intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		game_main_fbx_object_handle_t *map = nullptr;
		int polygon_id = -1;
		int mesh_id = -1;
		//ビームの広範囲衝突予測をするか
		if (option_get_do_aim_assist())
		{
			D3DXVECTOR3 start_position;
			D3DXVECTOR3 beam_screen_pos_start = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&start_position, &beam_screen_pos_start);
			D3DXVECTOR3 end_position;
			cursor_x = 0.0f;
			cursor_y = 0.0f;
			cursor_z = _beam_end_z();
			D3DXVECTOR3 beam_screen_pos_end = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&end_position, &beam_screen_pos_end);
			float thickness = 0.0;
			thickness = param->_status._beam_extensive_thickness;
			check_beam_hit_map(
				object,
				&start_position,
				&end_position,
				thickness,
				&map,
				&beam_hitting_extensive_prediction,
				&polygon_id,
				&mesh_id,
				&beam_intersect,
#if _output_debug_info
				nullptr,
				nullptr,
				nullptr,
				nullptr
#else
				nullptr
#endif
			);
		}
		if (beam_hitting_extensive_prediction)
		{
			param->_beam_hitting_extensive_prediction_object_handle = map;
			param->_beam_hitting_extensive_prediction = true;
			param->_beam_hitting_extensive_prediction_polygon_id = polygon_id;
			param->_beam_hitting_extensive_prediction_mesh_id = mesh_id;
		}
		else
		{
			param->_beam_hitting_extensive_prediction_object_handle = nullptr;
			param->_beam_hitting_extensive_prediction = false;
			param->_beam_hitting_extensive_prediction_polygon_id = -1;
			param->_beam_hitting_extensive_prediction_mesh_id = -1;
		}
	}
	//現在、磁力移行状態か教える
	ask_player_state_is_sticking_migration(param->_now_state == psi_sticking_migration);
}


//全状態において行われる事後処理(ステートごとの更新後に呼ばれる)
static void state_common_post_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(brain);
	//ジャンプ中か、情報を渡す
	player_is_jumping_set(param->_jump_count > 0);
	//この時点でのY移動量を渡す
	player_jump_move_y_set(param->_jump_move_y);
	//照準カーソル更新
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(NAN, NAN, NAN);
		game_main_fbx_object_handle_t *map = nullptr;
		int polygon_id = -1;
		if (param->_beam_hitting_extensive_prediction)
		{
			//広範囲予測に成功している
			if (param->_beam_hitting_prediction)
			{
				//普通の予測でも成功している
				map = param->_beam_hitting_prediction_object_handle;
				polygon_id = param->_beam_hitting_prediction_polygon_id;
			}
			else
			{
				//普通の予測では成功していない
				map = param->_beam_hitting_extensive_prediction_object_handle;
				polygon_id = param->_beam_hitting_extensive_prediction_polygon_id;
			}
		}
		int min_polygon_id = ((int)(polygon_id) / 12) * 12;
		int max_polygon_id = min_polygon_id + 12;
		D3DXVECTOR3 screen_pos = D3DXVECTOR3(invalid_aiming_cursor_pos(), invalid_aiming_cursor_pos(), invalid_aiming_cursor_pos());
		if (map)
		{
			game_main_fbx_object_t *map_object = to_object(map);
			D3DXMATRIX world;
			gmfoh_t_get_world(map, &world, nullptr);
			fbx_table_get_polygon_position_range(
				map_object->_looks,
				map_object->_now_motion,
				&world,
				map_object->_motion_frame,
				min_polygon_id,
				max_polygon_id,
				&pos
			);
			mylib_to_screen_pos(&screen_pos, &pos);
		}
		//照準カーソル位置更新
		aiming_cursor_pos_update(screen_pos.x, screen_pos.y, screen_pos.z);
#if _output_debug_info
		mylib_register_debug_string("hit polygon ID : %d", polygon_id);
		mylib_register_debug_string("hit min / max polygon ID : (%d, %d)", min_polygon_id, max_polygon_id);
		mylib_register_debug_string("hit mesh ID : %d", param->_beam_hitting_mesh_id);
		mylib_register_debug_string("hit pos (x : %f, y : %f, z : %f)", pos.x, pos.y, pos.z);
		mylib_register_debug_string("hit screen pos (x : %f, y : %f, z : %f)", screen_pos.x, screen_pos.y, screen_pos.z);
#endif
	}
}


//通常状態のモーション切り替え処理
static void change_motion_normal(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//実行時間が最も短いアクションを探す
	time_t shortest_action_frame = 0xffffffff;
	player_action_id_t shortest_action = pai_none;
	for (size_t i = 0; i < pai_max; i++)
	{
		if (param->_doing_frame[i])
		{
			if (param->_doing_frame[i] < shortest_action_frame)
			{
				shortest_action_frame = param->_doing_frame[i];
				shortest_action = (player_action_id_t)(i);
			}
		}
	}
	//もはや問答無用の処理
	if (param->_doing[(size_t)(pai_horizontal_blow)])
	{
		shortest_action = pai_horizontal_blow;
	}
	switch (shortest_action)
	{
	case pai_none:
		object->_now_motion = ftmi_player_stand;
		break;
	case pai_run:
		object->_now_motion = ftmi_player_run;
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_jump:
		object->_now_motion = ftmi_player_fall;
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_beam:
		object->_now_motion = ftmi_player_front_walk;
#ifdef _beam_walk
		if (param->_beam_walk_mode_on)
		{
			//プレーヤーの角度を調べる
			float player_angle = camera_manager_get_phi() - brain->_input_angle - (float)(M_PI_2);
			//ビームの角度を調べる
			D3DXVECTOR3 beam_vector = param->_beam_end_position - param->_beam_start_position;
			float beam_angle = atan2f(beam_vector.z, beam_vector.x);
			//引き算する
			float sub = player_angle - beam_angle;
			//デグリー
			int deg = (int)(D3DXToDegree(sub));
			//45°だけ回す
			int deg45 = deg + 45;
			//さらに360*3°回す
			int deg45360 = deg45 + 360 * 3;
			//90で割り算して剰余を取る
			int _0123 = (deg45360 / 90) % 4;
			//アニメーションチェンジ
			switch (_0123)
			{
			case 0:
				object->_now_motion = ftmi_player_front_walk;
				break;
			case 1:
				object->_now_motion = ftmi_player_side_walk1;
				break;
			case 2:
				object->_now_motion = ftmi_player_back_walk;
				break;
			case 3:
				object->_now_motion = ftmi_player_side_walk2;
				break;
			default:
				break;
			}
		}
#endif
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_horizontal_blow:
		object->_now_motion = ftmi_player_round;
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_floating:
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_max:
		break;
	default:
		break;
	}
}
//通常状態の開始処理
static void state_normal_start(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
}
//通常状態の更新処理
static void state_normal_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
#ifdef _beam_walk
	//ビ~~ム歩きモードきりかえ
	change_beam_walk_mode(param, brain);
#endif
	//走る
	run_process(object, param, brain);
	//ジャンプ
	jump_process(object, param, brain);
	//横吹っ飛びを反映させる
	horizontal_blow_process(object, param);
	//浮遊処理
	floating_process(object, param, brain);
	//重力処理
	gravity_process(object, param);
	//地面とのあたり判定
	ground_hit_process(object, brain, param);
	//ビ~~ムを撃つ
	beam_process(object, param, brain);
#ifdef _beam_walk
	//ビ~~ムを撃っている間の歩き処理
	beam_walk_process(object, param, brain);
#endif
	//ビ~~ムとマップのあたり判定をとる準備
	game_main_fbx_object_handle_t *map = nullptr;
	bool beam_hitting = false;
	int polygon_id = -1;
	int mesh_id = -1;
	D3DXVECTOR3 beam_intersect = D3DXVECTOR3(NAN, NAN, NAN);
	//ビ~~ムとマップのあたり判定(ビームを撃っている間のみ)
	if (should_check_beam_hit_map(param))
	{
		//あたり判定を取る
		check_beam_hit_map(
			object,
			&param->_beam_start_position,
			&param->_beam_end_position,
			param->_status._beam_thickness,
			&map,
			&beam_hitting,
			&polygon_id,
			&mesh_id,
			&beam_intersect,
#if _output_debug_info
			nullptr,
			&param->_fordbg_beam_ray_position,
			&param->_fordbg_beam_ray_vector,
			&param->_fordbg_beam_intersect
#else
			nullptr
#endif
		);
	}
	if (beam_hitting)
	{
		param->_beam_hitting_object_handle = map;
		param->_beam_hitting = true;
		param->_beam_hitting_polygon_id = polygon_id;
		param->_beam_hitting_mesh_id = mesh_id;
	}
	else
	{
		param->_beam_hitting_object_handle = nullptr;
		param->_beam_hitting = false;
		param->_beam_hitting_polygon_id = -1;
		param->_beam_hitting_mesh_id = -1;
	}
	//ビ~~ムとマップが当たったか
	if (beam_hitting)
	{
		//この時点でくっつき移行状態になるべきである
		//くっつき移行前の位置を覚える
		param->_sticking_position_before = object->_position;
		//くっつき移行先の位置を覚える
		param->_sticking_position_after = beam_intersect;
		//くっつき移行状態になる
		param->_next_state = psi_sticking_migration;
		//時間を記録
		player_beam_hit_record();
	}
	//ゴール地点に到達したかチェック
	if (stage_event_get_player_near_goal(stage_data_get_goal_goal_threshold(stage_get_now())))
	{
		//ゴールしたことにする
		stage_event_stage_goal();
	}
	//落ちたら復活する
	respawn_if(object, param, is_fell(object));
	//モーションチェンジ
	change_motion_normal(object, param, brain);
	//現在ぐるぐる中か
	if (param->_doing[(size_t)(pai_horizontal_blow)])
	{
		//きらきらエフェクトを出す
		generate_glitter(object, param, 2);
	}
}
//通常状態の終了処理
static void state_normal_end(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
	//音を出す
	sound_play(si_se_maghit);
}



//くっつき移行状態の開始処理
static void state_sticking_migration_start(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//音を出す
	sound_play(si_se_magfly);
	sound_play_hit_beam();
	//横吹っ飛びの角度を決定
	param->_horizontal_blow_angle = object->_rotation.y + (float)(M_PI);
	//ジャンプ回数を0にする
	param->_jump_count = 0;
}

//くっつき移行状態の更新処理
static void state_sticking_migration_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//照準カーソルをオレンジにする
	player_beam_hit_object_set(true);
	//くっつき移行時の移動処理
	sticking_migration_process(object, param, brain);
#if 0
	//浮遊アクションをしようとしているか
	if (brain->_should_floating)
	{
		//フラグを切り替え
		param->_doing[(size_t)(pai_beam)] = false;
		//通常状態に戻る
		param->_next_state = psi_normal;
		return;
	}
#endif //2019/01/30 この機能は不要になった
	//影の位置を計算
	calc_shadow_position(object, param);
	//移行状態が終わったか
	if (sticking_migration_ended(object, param))
	{
		//フラグを切り替え
		param->_doing[(size_t)(pai_beam)] = false;
		//くっつき停止状態になる
		param->_next_state = psi_sticking_stop;
		//マップを消す(2019/02/05この処理は不要になった)
		//gmfoh_t_set_ignore_flag(param->_beam_hit_map_object, param->_beam_hitting_mesh_id, true);
		return;
	}
	param->_next_state = psi_sticking_migration;
}

//くっつき移行状態の終了処理
static void state_sticking_migration_end(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
}


//くっつき停止状態の開始処理
static void state_sticking_stop_start(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
	//QTE開始
	sprite_integration_qte_start();
}

//くっつき停止状態の更新処理
static void state_sticking_stop_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//照準カーソルをオレンジにする
	player_beam_hit_object_set(true);
	//くっつき停止時の処理
	sticking_stop_process(object, param, brain);
	//影の位置を計算
	calc_shadow_position(object, param);
	//磁力反発ジャンプをするべきか
	bool should_magnetic_jump = false;
	should_magnetic_jump |= brain->_should_magnetic_jump;
	should_magnetic_jump |= sprite_integration_qte_timeout();
	//磁力反発ジャンプをする
	if (should_magnetic_jump)
	{
		//QTE成功か
		if (sprite_integration_qte_judge() == true)
		{
			///QTE成功なので強いジャンプをする
			//磁力反発ジャンプをする
			do_magnetic_jump(
				object,
				param,
				param->_status._qte_success_magnetic_repulsion_duration_frame,
				param->_status._qte_success_magnetic_repulsion_force_x,
				param->_status._qte_success_magnetic_repulsion_force_y
			);
		}
		else
		{
			///QTE失敗なので普通のジャンプをする
			//磁力反発ジャンプをする
			do_magnetic_jump(
				object,
				param,
				param->_status._magnetic_repulsion_duration_frame,
				param->_status._magnetic_repulsion_force_x,
				param->_status._magnetic_repulsion_force_y
			);
		}
		//音を出す
		sound_play(si_se_magarrive);
		//普通の状態に戻る
		param->_next_state = psi_normal;
		return;
	}
	param->_next_state = psi_sticking_stop;
}

//くっつき停止状態の終了処理
static void state_sticking_stop_end(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
}


//ステートごとの開始処理をする
static void state_start(game_main_fbx_object_t *object, player_parameter_t *param, player_state_id_t state)
{
	switch (state)
	{
	case psi_none:
		break;
	case psi_normal:
		state_normal_start(object, param);
		break;
	case psi_sticking_migration:
		state_sticking_migration_start(object, param);
		break;
	case psi_sticking_stop:
		state_sticking_stop_start(object, param);
		break;
	case psi_max:
		break;
	default:
		break;
	}
}

//ステートごとの更新処理をする
static void state_process(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//入力を行動指令へと変換
	brain_process(&param->_brain);
	//共通事前処理
	state_common_pre_process(object, param, &param->_brain);
	//ステートごとの処理
	switch (param->_now_state)
	{
	case psi_none:
		param->_next_state = psi_normal;
		break;
	case psi_normal:
		state_normal_process(object, param, &param->_brain);
		break;
	case psi_sticking_migration:
		state_sticking_migration_process(object, param, &param->_brain);
		break;
	case psi_sticking_stop:
		state_sticking_stop_process(object, param, &param->_brain);
		break;
	case psi_max:
		param->_next_state = psi_normal;
		break;
	default:
		param->_next_state = psi_normal;
		break;
	}
	//共通事後処理
	state_common_post_process(object, param, &param->_brain);
	//モーション進行
	object->_motion_frame += 1.0f;
#if _output_debug_info
	//デバッグ用レイ表示フラグをセット
	param->_fordbg_draw_ground_ray = true;
	param->_fordbg_draw_beam_ray = param->_doing[(size_t)(pai_beam)];
#endif
}

//ステートごとの終了処理をする
static void state_end(game_main_fbx_object_t *object, player_parameter_t *param, player_state_id_t state)
{
	switch (state)
	{
	case psi_none:
		break;
	case psi_normal:
		state_normal_end(object, param);
		break;
	case psi_sticking_migration:
		state_sticking_migration_end(object, param);
		break;
	case psi_sticking_stop:
		state_sticking_stop_end(object, param);
		break;
	case psi_max:
		break;
	default:
		break;
	}
}

//ステートを切り替える処理
static void state_change(game_main_fbx_object_t *object, player_parameter_t *param)
{
	if (param->_now_state != param->_next_state)
	{
		state_end(object, param, param->_now_state);
		param->_now_state = param->_next_state;
		state_start(object, param, param->_now_state);
	}
}

//オブジェクトをプレイヤーとして初期化
extern void gmfo_t_player_init(game_main_fbx_object_t *object)
{
	//外部情報を読み込む
	const player_database_t *database = get_database();
	//メモリ確保
	object->_kind_parameter = malloc(sizeof(player_parameter_t));
	if (object->_kind_parameter == nullptr)
	{
		MessageBox(nullptr, "メモリ確保に失敗しました", "エラー", MB_OK);
		exit(EXIT_FAILURE);
	}
	ZeroMemory(object->_kind_parameter, sizeof(player_parameter_t));
	//プレイヤー専用の情報をセット
	player_parameter_t *param = (player_parameter_t*)(object->_kind_parameter);
	param->_status = database->_status;									//データベースからコピー
	param->_now_state = psi_none;										//現在のステート
	param->_next_state = param->_now_state;								//次回更新時にチェンジするステート
	//ステージの開始位置に動く
	stage_data_get_start_point(stage_get_now(), &object->_position);
	//拡大倍率を変更
	object->_scale = D3DXVECTOR3(database->_status._scaling, database->_status._scaling, database->_status._scaling);
	//照準カーソル初期化
	aiming_cursor_pos_init();
}

//オブジェクトをプレイヤーとして終了処理
extern void gmfo_t_player_end(game_main_fbx_object_t *object)
{
	//メモリ解放
	_mylib_safe_free(object->_kind_parameter);
}

//オブジェクトをプレイヤーとして更新
extern void gmfo_t_player_update(game_main_fbx_object_t *object)
{
	player_parameter_t *param = (player_parameter_t*)(object->_kind_parameter);
	//ステートを切り替える
	state_change(object, param);
	//更新前の位置を取得
	float object_prev_y = object->_position.y;
	//更新
	state_process(object, param);
	//更新後の位置を取得
	float object_updated_y = object->_position.y;
	//プレーヤーは死んだか
	if (object_updated_y < _player_dead_line())
	{
		//死んだ瞬間か(更新前に死んでいなかったか)
		if (object_prev_y > _player_dead_line())
		{
			//落下サウンドを鳴らす
			sound_play(si_se_voice_miss);
		}
	}
#if _output_debug_info
	mylib_register_debug_string("pl state : %s", state_name(param->_now_state));
	mylib_register_debug_string("pl pos (x : %f, y : %f, z : %f)", object->_position.x, object->_position.y, object->_position.z);
	mylib_register_debug_string("pl jump move y : %f", param->_jump_move_y);
	//
	for (int i = 0; i < pai_max;i++)
	{
		mylib_register_debug_string("%s, %d", action_name(player_action_id_t(i)), param->_doing_frame[player_action_id_t(i)]);
	}
#endif
}

//オブジェクトをプレイヤーとして描画
extern void gmfo_t_player_draw(const game_main_fbx_object_t *object)
{
	player_parameter_t *param = (player_parameter_t*)(object->_kind_parameter);
	//行列を取得
	D3DXMATRIX world;
	gmfoh_t_get_world(object->_handle, &world, nullptr);
	//描画
	fbx_table_draw_with_simple_shader(
		object->_looks,
		object->_now_motion,
		&world,
		object->_motion_frame,
		efmabm_no_blend
	);
	//プレーヤーの位置
	holder_player_position_set(&object->_position);
	//アームの位置を覚える
	holder_player_magnet_hand_position_set(param->_doing[(size_t)(pai_beam)], param->_doing_frame[(size_t)(pai_beam)], &param->_wrench_position);
	//プレーヤの影の位置を覚える
	holder_player_shadow_position_set(&param->_shadow_position);
	//プレーヤの影の法線を覚える
	holder_player_shadow_normal_set(&param->_shadow_normal);
#if _output_debug_info
	//デバッグ用描画
	if (param->_fordbg_draw_ground_ray)
	{
		mylib_draw_ray(&param->_fordbg_ray_position, &param->_fordbg_ray_vector, D3DCOLOR_XRGB(0, 255, 0));
		mylib_draw_sphere(&param->_shadow_position, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
	if (param->_fordbg_draw_beam_ray)
	{
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.0f, 0.0f, 0.0f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.1f, 0.0f, 0.0f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.0f, 0.1f, 0.0f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.0f, 0.0f, 0.1f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_sphere(&param->_fordbg_beam_intersect, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
	}
	mylib_draw_ray(&param->_fordbg_ray2_position, &param->_fordbg_ray2_vector, D3DCOLOR_XRGB(0, 255, 0));
	mylib_draw_sphere(&param->_fordbg_ray2_intersect, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
#endif
}
