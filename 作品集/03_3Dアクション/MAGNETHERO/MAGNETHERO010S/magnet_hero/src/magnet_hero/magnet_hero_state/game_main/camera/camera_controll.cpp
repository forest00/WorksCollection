//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	

//親ヘッダー
#include "camera_controll.h"
//その他のヘッダー
#include "..\connecter\info_holder.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\option\option.h"
#include "..\..\..\..\input\input.h"
#include "..\..\..\..\calc\calc.h"

//カメラの下限
#define _camera_lower()			(-400.0f)

static float calc_distance(float distance, float sec, float power)
{
	DWORD moment = player_beam_hit_get();
	if (moment > 0)
	{
		DWORD elapsed = timeGetTime() - moment;
		DWORD elapsed_max = (DWORD)(sec * 1000.0f);
		if (elapsed < elapsed_max)
		{
			float t = (float)(elapsed) / (float)(elapsed_max);
			float s = 1.0f - t;
			return distance + t * s * s * power;
		}
	}
	return distance;
}

extern void camera_init(game_main_camera_t *camera, const camera_database_t *database)
{
	camera->_eye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->_at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	camera->_distance = database->_stage1_default_distance;
	camera->_theta = database->_stage1_default_theta;
	camera->_phi = database->_stage1_default_phi;
	camera->_user_move_force = database->_user_move_force;
	camera->_user_close_force = database->_user_close_force;
	camera->_user_turning_force = database->_user_turning_force;
	camera->_user_move_weight = database->_user_move_weight;
	camera->_user_close_weight = database->_user_close_weight;
	camera->_user_turning_weight = database->_user_turning_weight;
	camera->_user_move_result = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->_user_close_result = 0.0f;
	camera->_user_theta_result = 0.0f;
	camera->_user_phi_result = 0.0f;
	camera->_user_move_accumulate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->_distance_upper = database->_distance_upper;
	camera->_distance_lower = database->_distance_lower;
	camera->_theta_upper = D3DXToRadian(database->_theta_upper);
	camera->_theta_lower = D3DXToRadian(database->_theta_lower);
	camera->_shift_up = database->_shift_up;
	camera->_move_time = database->_move_time;
	camera->_move_power = database->_move_power;
	camera->_jump_point = 0.0f;
}

extern void camera_end(game_main_camera_t *camera)
{
	UNREFERENCED_PARAMETER(camera);
}

extern void camera_update(game_main_fbx_object_array_t *object_array, game_main_camera_t *camera)
{
	const float level = lerpf(10.0f, 1.0f, option_get_mouse_sensitivity());
	const virtual_input_t *repeat = mylib_get_virtual_input_repeat();
	camera->_user_close_result += camera->_user_close_force * input_controller_ZL_ZR();
	camera->_user_phi_result -= camera->_user_turning_force * repeat->_rstick_x * (fabsf(repeat->_rstick_x) > 0.2f);
	camera->_user_phi_result -= camera->_user_turning_force * (float)(input_mouse_move_x()) / level;
	camera->_user_theta_result -= camera->_user_turning_force * repeat->_rstick_y * (fabsf(repeat->_rstick_y) > 0.2f);
	camera->_user_theta_result -= camera->_user_turning_force * (float)(input_mouse_move_y()) / level;
	if (input_keyboard_repeat(DIK_HOME))
	{
		camera->_user_move_result.x += camera->_user_move_force * cosf(camera->_phi + (float)(M_PI));
		camera->_user_move_result.z += camera->_user_move_force * sinf(camera->_phi + (float)(M_PI));
	}
	if (input_keyboard_repeat(DIK_END))
	{
		camera->_user_move_result.x += camera->_user_move_force * cosf(camera->_phi);
		camera->_user_move_result.z += camera->_user_move_force * sinf(camera->_phi);
	}
	if (input_keyboard_repeat(DIK_DELETE))
	{
		camera->_user_move_result.x += camera->_user_move_force * cosf(camera->_phi - (float)(M_PI_2));
		camera->_user_move_result.z += camera->_user_move_force * sinf(camera->_phi - (float)(M_PI_2));
	}
	if (input_keyboard_repeat(DIK_PGDN))
	{
		camera->_user_move_result.x += camera->_user_move_force * cosf(camera->_phi + (float)(M_PI_2));
		camera->_user_move_result.z += camera->_user_move_force * sinf(camera->_phi + (float)(M_PI_2));
	}
	if (input_keyboard_repeat(DIK_INSERT))
	{
		camera->_user_move_result.y += camera->_user_move_force;
	}
	if (input_keyboard_repeat(DIK_PGUP))
	{
		camera->_user_move_result.y -= camera->_user_move_force;
	}
	if (input_keyboard_repeat(DIK_Q))
	{
		camera->_user_close_result += camera->_user_close_force;
	}
	if (input_keyboard_repeat(DIK_E))
	{
		camera->_user_close_result -= camera->_user_close_force;
	}
	if (input_keyboard_repeat(DIK_LEFTARROW))
	{
		camera->_user_phi_result += camera->_user_turning_force;
	}
	if (input_keyboard_repeat(DIK_RIGHTARROW))
	{
		camera->_user_phi_result -= camera->_user_turning_force;
	}
	if (input_keyboard_repeat(DIK_UPARROW))
	{
		camera->_user_theta_result += camera->_user_turning_force;
	}
	if (input_keyboard_repeat(DIK_DOWNARROW))
	{
		camera->_user_theta_result -= camera->_user_turning_force;
	}
	camera->_user_move_accumulate += camera->_user_move_result;
	//プレーヤーの識別用ハンドルを得る
	game_main_fbx_object_handle_t *player_handle = nullptr;
	gmfoa_t_search_kind(object_array, gmfok_player, nullptr, &player_handle);
	//プレーヤーの位置を得る
	D3DXVECTOR3 player_position = {};
	gmfoh_t_get_position(player_handle, &player_position);
	camera->_at = player_position + camera->_user_move_accumulate;
	camera->_distance += camera->_user_close_result;
	camera->_theta += camera->_user_theta_result;
	camera->_phi += camera->_user_phi_result;
	camera->_distance = clampf(camera->_distance, camera->_distance_lower, camera->_distance_upper);
	camera->_theta = clampf(camera->_theta, camera->_theta_lower, camera->_theta_upper);
	camera->_phi = angle_normalizef(camera->_phi);
	camera->_user_move_result *= camera->_user_move_weight;
	camera->_user_close_result *= camera->_user_close_weight;
	camera->_user_theta_result *= camera->_user_turning_weight;
	camera->_user_phi_result *= camera->_user_turning_weight;
	float real_distance = calc_distance(camera->_distance, camera->_move_time, camera->_move_power);
	//ポイントによって離す
	real_distance *= lerpf(1.0f, 1.75f, camera->_jump_point);
	camera->_eye.x = real_distance * sinf(camera->_theta) * cosf(camera->_phi);
	camera->_eye.y = real_distance * cosf(camera->_theta);
	camera->_eye.z = real_distance * sinf(camera->_theta) * sinf(camera->_phi);
	camera->_eye += camera->_at;
	camera->_at.y += camera->_shift_up;
	camera->_eye.y += camera->_shift_up;
	{
		camera->_true_eye = camera->_eye;
		//地面の識別用ハンドルを得る
		game_main_fbx_object_handle_t *ground_handle = nullptr;
		gmfoa_t_search_kind(object_array, gmfok_stage_ground, nullptr, &ground_handle);
		//カメラの注視点からカメラの位置までレイを飛ばす
		D3DXVECTOR3 ray_start_pos = camera->_at;
		D3DXVECTOR3 ray_end_pos = camera->_eye;
		D3DXVECTOR3 ray_vector = ray_end_pos - ray_start_pos;
		D3DXVECTOR3 intersection = D3DXVECTOR3(NAN, NAN, NAN);
		D3DXVECTOR3 normal = D3DXVECTOR3(NAN, NAN, NAN);
		bool hitting = false;
		gmfoh_t_raycast(ground_handle, &ray_start_pos, &ray_vector, &intersection, &normal, nullptr, &hitting, nullptr);
		if (hitting)
		{
			//カメラの位置を直す
			camera->_true_eye = intersection;
		}
	}
	//プレーヤーがジャンプ中か
	if (player_is_jumping_get())
	{
		//ポイントを増やす
		camera->_jump_point = lerpf(camera->_jump_point, 1.0f, 0.02f);
	}
	else
	{
		//ポイントを減らす
		camera->_jump_point = lerpf(camera->_jump_point, 0.0f, 0.10f);
	}
	//ポイントによって上にずらす
	camera->_true_eye.y += camera->_jump_point * 100.0f;
	//カメラはこれ以上、下に行けない
	if (camera->_true_eye.y < _camera_lower())
	{
		float sub = camera->_true_eye.y - _camera_lower();
		camera->_true_eye.y = _camera_lower() + sub / 10.0f;
	}
}

extern void camera_set_view_and_projection(game_main_camera_t *camera, float near_clip, float far_clip)
{
	//ビュー行列
	D3DXMATRIX view;
	mylib_calc_view(&view, &camera->_true_eye, &camera->_at, &camera->_up);
	mylib_set_view(&view);
	//プロジェクション行列
	D3DXMATRIX projection;
	mylib_calc_projection(&projection, (float)(M_PI_4), near_clip, far_clip);
	mylib_set_projection(&projection);
}