#pragma once
//テクスチャつき描画ができるシェーダーを使うためのファイルです
//作成者　森　仁志
//履歴
//2019/01/29...ファイルを作成
//同日...ベースとなる関数を記述



#include <d3dx9.h>

enum texture_shader_pass_t
{
	tsp_no_blend = 0,
	tsp_alpha_blend = 1,
	tsp_add_blend = 2,
};

#if 1
extern int texture_shader_init();
extern void texture_shader_end();
extern void texture_shader_begin_scene(texture_shader_pass_t pass);
extern void texture_shader_end_scene();
extern void texture_shader_commit();
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void texture_shader_set_wvp(const D3DXMATRIX *wvp);
//テクスチャ
extern void texture_shader_set_texture(IDirect3DTexture9 *texture);
#endif

#if 1
extern int texture_shader2_init();
extern void texture_shader2_end();
extern void texture_shader2_begin_scene();
extern void texture_shader2_end_scene();
extern void texture_shader2_commit();
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void texture_shader2_set_wvp(const D3DXMATRIX *wvp);
//テクスチャ
extern void texture_shader2_set_texture(IDirect3DTexture9 *texture);
#endif
