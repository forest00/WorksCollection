//親ヘッダー
#include "bg.h"
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\..\shader_use\simple_shader.h"

//オブジェクトをあたり判定なしの動かないマップとして初期化
extern void gmfo_t_stage_bg_init(game_main_fbx_object_t *object)
{
	//動かないから初期化処理は無い
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定なしの動かないマップとして終了処理
extern void gmfo_t_stage_bg_end(game_main_fbx_object_t *object)
{
	//動かないから終了処理は無い
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定なしの動かないマップとして更新
extern void gmfo_t_stage_bg_update(game_main_fbx_object_t *object)
{
	//動かないから更新処理は無い
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定なしの動かないマップとして描画
extern void gmfo_t_stage_bg_draw(const game_main_fbx_object_t *object)
{	
#if 1//描画したくないときはここの数値を0にする
	//カリングの設定を取得
	DWORD cull_mode;
	mylib_get_d3d_device()->GetRenderState(D3DRS_CULLMODE, &cull_mode);
	//カリングオフ
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//テクスチャをセット(真っ白)
	simple_shader_set_texture(texture_manager_get(ti_white));
	//行列を取得
	D3DXMATRIX world;
	gmfoh_t_get_world(object->_handle, &world, nullptr);
	//描画
	fbx_table_draw_with_simple_shader_max_alpha(
		object->_looks,
		object->_now_motion,
		&world,
		object->_motion_frame
	);
	//カリングを戻す
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, cull_mode);
#endif
}