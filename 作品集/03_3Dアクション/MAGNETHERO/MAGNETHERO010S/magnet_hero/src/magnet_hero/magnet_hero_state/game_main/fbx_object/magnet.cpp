//親ヘッダー
#include "magnet.h"
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\calc\calc.h"
#include "..\..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\..\shader_use\simple_shader.h"
#include "..\stage_event\stage_index_saver.h"


//オブジェクトをあたり判定付きマップとして初期化
extern void gmfo_t_magnet_init(game_main_fbx_object_t *object)
{
	//ここに初期化処理を書く
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定付きマップとして終了処理
extern void gmfo_t_magnet_end(game_main_fbx_object_t *object)
{
	//ここに終了処理を書く
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定付きマップとして更新
extern void gmfo_t_magnet_update(game_main_fbx_object_t *object)
{
	//ここに更新処理を書く
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定付きマップとして描画
extern void gmfo_t_magnet_draw(const game_main_fbx_object_t *object)
{
#if 0//まぁ、当たり判定用のオブジェクトなんて描画しないよね、ははは
	//カリングの設定を取得
	DWORD cull_mode;
	mylib_get_d3d_device()->GetRenderState(D3DRS_CULLMODE, &cull_mode);
	//カリングオフ
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//テクスチャをセット(マーブル)
	simple_shader_set_texture(texture_manager_get(ti_marble_0));
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
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, cull_mode);
#else
	UNREFERENCED_PARAMETER(object);
#endif
}
