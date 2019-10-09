//親ヘッダー
#include "wrench.h"
//その他のヘッダー
#include "..\camera\camera_manager.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\calc\calc.h"
#include "..\..\..\..\fbx_table\fbx_table.h"
#include "..\..\..\..\texture_manager\texture_manager.h"

#if 0
extern void wrench_draw(const D3DXVECTOR3 *position, float scale, time_t frame, float theta, float phi)
{
	float x;
	float y;
	float z;
	to_xyz(&x, &y, &z, 1.0f, theta, -phi);
	D3DXMATRIX matrix_translation;
	D3DXMATRIX matrix_rotation;
	D3DXMATRIX matrix_world;
	D3DXMatrixTranslation(&matrix_translation, position->x, position->y, position->z);
	D3DXMatrixLookAtLH(&matrix_rotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(x, y, z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixIdentity(&matrix_world);
	D3DXMatrixMultiply(&matrix_world, &matrix_world, &matrix_rotation);
	D3DXMatrixMultiply(&matrix_world, &matrix_world, &matrix_translation);
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3;k++)
		{
			matrix_world.m[i][k] *= scale;
		}
	}
	//ブレンドステート保存
	mylib_blend_mode_t blend_mode;
	mylib_get_blend_mode(&blend_mode);
	//ブレンドステートを変える
	mylib_set_blend_mode(mbm_alpha);
	//描画
	fbx_table_draw_with_simple_shader(
		fti_wrench,
		ftmi_none,
		&matrix_world,
		0.0f,
		efmabm_alpha_blend
	);
	//ブレンドステートを戻す
	mylib_set_blend_mode(blend_mode);
}
#endif

extern void ball_draw(const D3DXVECTOR3 *position, float scale)
{
	//スケール
	D3DXVECTOR2 _scale = D3DXVECTOR2(scale, scale);
	//UV座標用意
	D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//描画
	mylib_draw_billboard(
		position,
		0.0f,
		&_scale,
		0x80ffffff,
		&uv[0],
		&uv[1],
		&uv[2],
		&uv[3],
		texture_manager_get(ti_mag_ball_in),
		mbm_add
	);
}
