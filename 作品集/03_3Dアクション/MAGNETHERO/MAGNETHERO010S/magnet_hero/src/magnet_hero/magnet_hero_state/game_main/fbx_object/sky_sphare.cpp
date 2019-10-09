//親ヘッダー
#include "sky_sphare.h"
//その他のヘッダー
#include "..\connecter\info_holder.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\shader_use\simple_shader.h"



//オブジェクトをスカイスフィアとして初期化
extern void gmfo_t_sky_sphere_init(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}

//オブジェクトをスカイスフィアとして終了処理
extern void gmfo_t_sky_sphere_end(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}

//オブジェクトをスカイスフィアとして更新
extern void gmfo_t_sky_sphere_update(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}

//オブジェクトをスカイスフィアとして描画
extern void gmfo_t_sky_sphere_draw(const game_main_fbx_object_t *object)
{
#if 1
	//ライティングオフ
	simple_shader_set_lighting(true);
	//カリングの設定を取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリングオフ
	mylib_set_cull_mode(D3DCULL_NONE);
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
	//カリングを戻す
	mylib_set_cull_mode(cull_mode);
	//ライティングオン
	simple_shader_set_lighting(true);
#endif
}




