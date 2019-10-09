#pragma once
//色つき描画ができるシェーダーを使うためのファイルです
//作成者　森　仁志



#include <d3dx9.h>


enum diffuse_only_shader_pass_t
{
	dosp_no_blend = 0,
	dosp_alpha_blend = 1,
	dosp_add_blend = 2,
};

extern int diffuse_only_shader_init();
extern void diffuse_only_shader_end();
extern void diffuse_only_shader_begin_scene(diffuse_only_shader_pass_t pass);
extern void diffuse_only_shader_end_scene();
extern void diffuse_only_shader_commit();
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void diffuse_only_shader_set_wvp(const D3DXMATRIX *wvp);
//ディフューズ
extern void diffuse_only_shader_set_diffuse(const D3DXVECTOR4 *diffuse);
