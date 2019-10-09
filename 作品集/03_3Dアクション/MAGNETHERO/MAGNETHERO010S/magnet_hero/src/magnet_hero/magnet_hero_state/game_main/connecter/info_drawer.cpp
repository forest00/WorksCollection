//親ヘッダー
#include "info_drawer.h"
//その他のヘッダー
#include "info_holder.h"
#include "..\fbx_object\wrench.h"
#include "..\camera\camera_manager.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"


//プレーヤーの影を描画する
extern void drawer_player_shadow_draw()
{
	//影の位置取得
	D3DXVECTOR3 position;
	holder_player_shadow_position_get(&position);
	//ちょっとずらす
	position.y += 0.05f;
	//影の法線取得
	D3DXVECTOR3 normal;
	holder_player_shadow_normal_get(&normal);
	if (normal == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		normal.y = 1.0f;
	}
	//ワールド行列作成
	D3DXMATRIX matrix_scale;			//スケーリング行列
	D3DXMATRIX matrix_rotation;			//回転行列
	D3DXMATRIX matrix_translation;		//平行移動行列
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&matrix_translation, position.x, position.y, position.z);
	//D3DXMatrixLookAtLH(&matrix_rotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixRotationX(&matrix_rotation, (float)(M_PI_2));
	D3DXMatrixScaling(&matrix_scale, 8.0f, 8.0f, 0.0f);
	D3DXMatrixMultiply(&world, &world, &matrix_scale);
	D3DXMatrixMultiply(&world, &world, &matrix_rotation);
	D3DXMatrixMultiply(&world, &world, &matrix_translation);
	//UV座標用意
	D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//描画情報登録
	mylib_draw_board_3d(
		&world,
		&uv[0], 0xffffffff,
		&uv[1], 0xffffffff,
		&uv[2], 0xffffffff,
		&uv[3], 0xffffffff,
		texture_manager_get(ti_shade), mbm_alpha
	);
}

extern void drawer_player_magnet_hand()
{
	bool do_draw;
	time_t frame;
	D3DXVECTOR3 position;
	holder_player_magnet_hand_position_get(&do_draw, &frame, &position);
	if (do_draw)
	{
#if 0
		wrench_draw(
			&position,
			1.0f,
			frame,
			camera_manager_get_theta(),
			camera_manager_get_phi()
		);
#else
		ball_draw(
			&position,
			50.0f
		);
#endif
	}
}
