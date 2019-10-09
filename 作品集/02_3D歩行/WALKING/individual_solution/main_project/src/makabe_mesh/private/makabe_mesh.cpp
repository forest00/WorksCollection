//親ヘッダー
#include "..\public\makabe_mesh.h"
//その他のヘッダー
#include <time.h>
#include "..\..\math_with_defines\math_with_defines.h"
#include "..\public\makabe_mesh_creator.h"
#include "..\..\world_transformation\public\world_transformation.h"
#include "..\..\input\public\input.h"
#include "..\..\camera\public\camera.h"
#include "..\..\calc\public\calc.h"
#include "..\..\sound\public\sound.h"


struct makabe_box_t
{
	bool _is_active;
	double _time;
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _angle;
	D3DXVECTOR3 _scale;
};


static makabe_mesh_t *_makabe_mesh;
static makabe_box_t g_box = {};
#define MAX 99
static makabe_box_t g_addition[MAX] = {};



extern void makabe_mash_init(IDirect3DDevice9 *device)
{
	makabe_mesh_creator_start_up(device);
	g_box._is_active = true;
	g_box._time = 0.0;
	g_box._position = D3DXVECTOR3(-20.0f, 2.0f, 10.0f);
	g_box._angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_box._scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	makabe_mesh_creator_create(&_makabe_mesh, device);
	for (int i = 0 ; i < MAX ;i++)
	{
		g_addition[i]._is_active = false;
	}
}

extern void makabe_mash_end()
{
	makabe_mesh_creator_end();
}

extern void makabe_mash_update()
{
	g_box._angle.x = g_box._time;
	g_box._angle.y = g_box._time / 2.0;
	g_box._angle.z = g_box._time / 8.0;
	g_box._time += 0.016;
	//入力により動く
	{
		double input_x = input_controller_left_x() + input_keyboard_repeat(DIK_RIGHT) - input_keyboard_repeat(DIK_LEFT);
		double input_y = input_controller_left_y() + input_keyboard_repeat(DIK_DOWN) - input_keyboard_repeat(DIK_UP);
		bool input_on = (fabs(input_x) > 0.1) || (fabs(input_y) > 0.1);
		double input_angle = atan2(input_y, input_x);
		angle_t camera_theta = 0.0;
		camera_get_theta(&camera_theta);
		double go_angle = input_angle - camera_theta;
		if (input_on)
		{
			g_box._position.x -= cos(go_angle + M_PI_2) / 16.0;
			g_box._position.z += sin(go_angle + M_PI_2) / 16.0;
		}
	}
	if (input_controller_trigger(input_controller_code_left_stick_push) || input_keyboard_trigger(DIK_F10))
	{
		for (int i = 0; i < MAX;i++)
		{
			if (g_addition[i]._is_active == false)
			{
				sound_play(sound_index_se_2);
				g_addition[i]._is_active = true;
				g_addition[i]._time = 0.0;
				g_addition[i]._position = D3DXVECTOR3(uniform_rand(-15.0, 15.0), uniform_rand(0.0, 8.0), uniform_rand(5.0, 30.0));
				g_addition[i]._angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_addition[i]._scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				break;
			}
		}
	}
	for (int i = 0; i < MAX;i++)
	{
		if (g_addition[i]._is_active)
		{
			g_addition[i]._angle.x = g_addition[i]._time;
			g_addition[i]._angle.y = g_addition[i]._time / 2.0;
			g_addition[i]._angle.z = g_addition[i]._time / 8.0;
			g_addition[i]._time += 0.016;
		}
	}
}

extern void makabe_mash_draw(IDirect3DDevice9 *device)
{
	D3DXMATRIX world;
	world_transformation(&world, &g_box._position, &g_box._angle, &g_box._scale);
	makabe_mesh_creator_draw(_makabe_mesh, device, &world);
	for (int i = 0; i < MAX;i++)
	{
		if (g_addition[i]._is_active)
		{
			world_transformation(&world, &g_addition[i]._position, &g_addition[i]._angle, &g_addition[i]._scale);
			makabe_mesh_creator_draw(_makabe_mesh, device, &world);
		}
	}
}

extern void makabe_mash_get_position(D3DXVECTOR3 *position)
{
	if (position)
	{
		*position = g_box._position;
	}
}
