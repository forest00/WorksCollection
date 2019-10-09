
//親ヘッダー
#include"..\public\camera.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\calc\public\calc.h"
#include "..\..\math_with_defines\math_with_defines.h"
#include "..\..\angle\public\angle.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\player\public\player.h"
#include "..\..\anim_models\public\anim_model_test.h"
#include "..\..\makabe_mesh\public\makabe_mesh.h"
#include "..\..\input\public\input.h"
#include "..\..\sound\public\sound.h"

#define camera_near_limit		(0.5)
#define camera_far_limit		(64.0)


/*-------------------------*/
enum camera_target_type_t
{
	camera_target_type_hubon,
	camera_target_type_golem,
	camera_target_type_max,
	camera_target_type_makabe,
};

struct camera_t
{
	vector3_t _eye_pos;
	vector3_t _look_pos;
	vector3_t _up_dir;
	angle_t _theta;
	angle_t _phi;
	angle_t _turning_force;
	double _distance;
	camera_target_type_t _target_type;
	camera_target_type_t _prev_target_type;
	double _elapsed_from_change;
};


static camera_t *camera_t_allocation();

static void camera_t_init(camera_t *camera);

static void camera_t_update(camera_t *camera, const vector3_t *target_pos);

static void camera_t_input_reception(camera_t *camera);

static void camera_t_get_eye_pos(camera_t *camera, vector3_t *position);

static void camera_t_get_look_pos(camera_t *camera, vector3_t *position);

static void camera_t_get_up_dir(camera_t *camera, vector3_t *direction);

static void camera_t_get_theta(camera_t *camera, angle_t *angle);

static void camera_t_get_phi(camera_t *camera, angle_t *angle);

static void camera_t_get_target_pos(camera_target_type_t type, vector3_t *position);
/*-------------------------*/







/*-------------------------*/
static camera_t *camera_t_allocation()
{
	void *v = malloc(sizeof(camera_t));
	assert(v != nullptr);
	return (camera_t*)(v);
}

static void camera_t_destroy(camera_t **camera)
{
	safe_free(*camera);
}

static void camera_t_init(camera_t *camera)
{
	camera->_eye_pos = { 0.0f, 5.0f, -20.0f };
	camera->_look_pos = { 0.0f, 0.0f, 0.0f };
	camera->_up_dir = { 0.0f, 1.0f, 0.0f };
	camera->_theta = -M_PI / 2.0f;
	camera->_turning_force = M_PI / 100.0;
	camera->_distance = 16.0f;
	camera->_target_type = camera_target_type_hubon;
	camera->_elapsed_from_change = 1.0;
}

static void camera_t_update(camera_t *camera, const vector3_t *target_pos)
{
	camera_t_input_reception(camera);
	vector3_t detachment;


	detachment = {
		(float)(cos(camera->_theta) * camera->_distance),
		(float)(5.0),
		(float)(sin(camera->_theta) * camera->_distance)
	};

	//単純な3D極座標変換ではうまくいかないようだ
	//detachment = {
	//	(float)(cos(camera->_theta) * cos(camera->_phi) * camera->_distance),
	//	(float)(cos(camera->_theta) * sin(camera->_phi) * camera->_distance),
	//	(float)(sin(camera->_theta) * camera->_distance)
	//};

	camera->_eye_pos = *target_pos + detachment;
	camera->_look_pos = *target_pos;
}

static void camera_t_input_reception(camera_t *camera)

{
	//入力チェック
	bool input_right = input_keyboard_repeat(DIK_D);
	bool input_left = input_keyboard_repeat(DIK_A);
	bool input_up = input_keyboard_repeat(DIK_W);
	bool input_down = input_keyboard_repeat(DIK_S);
	double input_x = 0.0;
	double input_y = 0.0;
	{
		input_x = input_controller_right_x();
		input_y = input_controller_right_y();
		{
			if (input_left)
			{
				input_x -= 1.0f;
			}
			if (input_right)
			{
				input_x += 1.0f;
			}
			if (input_up)
			{
				input_y -= 1.0f;
			}
			if (input_down)
			{
				input_y += 1.0f;
			}
		}
		input_x *= -1.0;
	}
	{
		camera->_theta += input_x * camera->_turning_force;
		camera->_phi += input_y * camera->_turning_force;


		camera->_theta = angle_pi_to_pi(camera->_theta);
		camera->_phi = angle_pi_to_pi(camera->_phi);
	}

	camera->_distance += (input_y * 0.3);
	camera->_distance = clamp(camera->_distance, camera_near_limit, camera_far_limit);
}

static void camera_t_end(camera_t *)
{
	;
}

static void camera_t_get_eye_pos(camera_t *camera, vector3_t *position)
{
	if (position)
	{
		*position = camera->_eye_pos;
	}
}

static void camera_t_get_look_pos(camera_t *camera, vector3_t *position)
{
	if (position)
	{
		*position = camera->_look_pos;
	}
}

static void camera_t_get_up_dir(camera_t *camera, vector3_t *direction)
{
	if (direction)
	{
		*direction = camera->_up_dir;
	}
}

static void camera_t_get_theta(camera_t *camera, angle_t *angle)
{
	if (angle)
	{
		*angle = camera->_theta;
	}
}

static void camera_t_get_phi(camera_t *camera, angle_t *angle)
{
	if (angle)
	{
		*angle = camera->_phi;
	}
}

static void camera_t_get_target_pos(camera_target_type_t type, vector3_t *position)
{
	if (position)
	{
		switch (type)
		{
		case camera_target_type_hubon:
		{
			test_player_manager_t * manager = player_get_test_player_manager();
			test_player_object_t **player = test_player_manager_t_search_active(manager);
			const vector3_t *player_pos = test_player_manager_t_get_position(manager, *player);
			*position = *player_pos;
		}
		break;
		case camera_target_type_golem:
			anim_model_get_the_position_of_the_model_to_use_as_a_player(position);
			break;
		case camera_target_type_makabe:
			makabe_mash_get_position(position);
			break;
		default:
			break;
		}
	}
}
/*-------------------------*/


/*-------------------------*/
static camera_t *g_camera;
/*-------------------------*/






/*-------------------------*/
extern void camera_manager_init()
{
	g_camera = camera_t_allocation();
	camera_t_init(g_camera);
}

extern void camera_manager_update()
{
	vector3_t pos;
	{
		g_camera->_elapsed_from_change += 1.0;
		if (
			input_controller_trigger(input_controller_code_x) ||
			input_controller_trigger(input_controller_code_y) ||
			input_keyboard_trigger(DIK_LSHIFT) ||
			input_keyboard_trigger(DIK_RSHIFT) ||
			input_keyboard_trigger(DIK_LCONTROL) ||
			input_keyboard_trigger(DIK_RCONTROL)
			)
		{
			g_camera->_prev_target_type = g_camera->_target_type;
			g_camera->_target_type = (camera_target_type_t)(((int)(g_camera->_target_type) + 1) % (int)(camera_target_type_max));
			g_camera->_elapsed_from_change = 0.0;
			sound_play(sound_index_se_5);
		}
		if (
			input_controller_trigger(input_controller_code_l) ||
			input_controller_trigger(input_controller_code_r)
			)
		{
			g_camera->_prev_target_type = g_camera->_target_type;
			g_camera->_target_type = camera_target_type_makabe;
			g_camera->_elapsed_from_change = 0.0;
			sound_play(sound_index_se_6);
		}
		vector3_t now_pos;
		vector3_t prev_pos;
		camera_t_get_target_pos(g_camera->_target_type, &now_pos);
		camera_t_get_target_pos(g_camera->_prev_target_type, &prev_pos);
		double t = min(g_camera->_elapsed_from_change / 16.0, 1.0);
		double tt = (1.0 - t) * (1.0 - t);
		pos.x = lerp(now_pos.x, prev_pos.x, tt);
		pos.y = lerp(now_pos.y, prev_pos.y, tt);
		pos.z = lerp(now_pos.z, prev_pos.z, tt);
	}
	camera_t_update(g_camera, &pos);
}

extern void camera_manager_end()
{
	camera_t_end(g_camera);
	camera_t_destroy(&g_camera);
}


extern void camera_get_eye_pos(vector3_t *position)
{
	camera_t_get_eye_pos(g_camera, position);
}

extern void camera_get_look_pos(vector3_t *position)
{
	camera_t_get_look_pos(g_camera, position);
}

extern void camera_get_up_dir(vector3_t *direction)
{
	camera_t_get_up_dir(g_camera, direction);
}

extern void camera_get_theta(angle_t *angle)
{
	camera_t_get_theta(g_camera, angle);
}

extern void camera_get_phi(angle_t *angle)
{
	camera_t_get_phi(g_camera, angle);
}
/*-------------------------*/


