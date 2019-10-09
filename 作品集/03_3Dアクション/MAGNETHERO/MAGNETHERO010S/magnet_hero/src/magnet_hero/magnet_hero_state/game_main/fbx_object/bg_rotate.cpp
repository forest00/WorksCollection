//親ヘッダー
#include "bg_rotate.h"
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"

//オブジェクトをあたり判定なしの回転し続けるマップとして初期化
extern void gmfo_t_stage_bg_rotate_init(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定なしの回転し続けるマップとして終了処理
extern void gmfo_t_stage_bg_rotate_end(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}
//オブジェクトをあたり判定なしの回転し続けるマップとして更新
extern void gmfo_t_stage_bg_rotate_update(game_main_fbx_object_t *object)
{
	object->_rotation.y += D3DXToRadian(0.5f);
}
//オブジェクトをあたり判定なしの回転し続けるマップとして描画
extern void gmfo_t_stage_bg_rotate_draw(const game_main_fbx_object_t *object)
{
	//カリング設定を取得
	DWORD now_cullmode = 0;
	mylib_get_d3d_device()->GetRenderState(D3DRS_CULLMODE, &now_cullmode);
	//カリングをオフにする
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//テクスチャ
	mylib_get_d3d_device()->SetTexture(0, texture_manager_get(ti_string_0));
	//描画
	D3DXMATRIX world;
	gmfoh_t_get_world(object->_handle, &world, nullptr);
	fbx_table_draw(object->_looks, object->_now_motion, &world, object->_motion_frame);
	//カリングを戻す
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, now_cullmode);
}