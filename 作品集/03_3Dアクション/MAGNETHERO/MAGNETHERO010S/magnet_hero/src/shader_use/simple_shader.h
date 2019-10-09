//シンプルシェーダーを使うための場所です
//シンプルシェーダーは、プリミティブを陰影付き、テクスチャ付きで描画できるシェーダーです
//作成者　森　仁志
//更新履歴
//2019/01/29...ファイルを作成
#pragma once


#include <d3dx9.h>

enum simple_shader_pass_t
{
	ssp_no_blend = 0,
	ssp_alpha_blend = 1,
	ssp_add_blend = 2,
};

#if 1
extern int simple_shader_init();
extern void simple_shader_end();
extern void simple_shader_begin_scene(simple_shader_pass_t pass);
extern void simple_shader_end_scene();
extern void simple_shader_commit();
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void simple_shader_set_wvp(const D3DXMATRIX *wvp);
//ワールド行列の逆行列の転置行列
extern void simple_shader_set_wit(const D3DXMATRIX *wit);
//ライトの方向ベクトル
extern void simple_shader_set_light_dir(const D3DXVECTOR4 *light_dir);
//入射光の強度
extern void simple_shader_set_light_intensity(const D3DXVECTOR4 *light_intensity);
//アンビエント
extern void simple_shader_set_ambient(const D3DXVECTOR4 *ambient);
//ディフューズ
extern void simple_shader_set_diffuse(const D3DXVECTOR4 *diffuse);
//テクスチャ
extern void simple_shader_set_texture(IDirect3DTexture9 *texture);
//ライトオンオフ
extern void simple_shader_set_lighting(bool lighting);
//時間
extern void simple_shader_set_time(float time);
//近クリップ面と遠クリップ面
extern void simple_shader_set_near_far_clip(float near_clip, float far_clip);
#endif


#if 1
extern int simple_shader_max_alpha_init();
extern void simple_shader_max_alpha_end();
extern void simple_shader_max_alpha_begin_scene();
extern void simple_shader_max_alpha_end_scene();
extern void simple_shader_max_alpha_commit();
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void simple_shader_max_alpha_set_wvp(const D3DXMATRIX *wvp);
//ワールド行列の逆行列の転置行列
extern void simple_shader_max_alpha_set_wit(const D3DXMATRIX *wit);
//ライトの方向ベクトル
extern void simple_shader_max_alpha_set_light_dir(const D3DXVECTOR4 *light_dir);
//入射光の強度
extern void simple_shader_max_alpha_set_light_intensity(const D3DXVECTOR4 *light_intensity);
//アンビエント
extern void simple_shader_max_alpha_set_ambient(const D3DXVECTOR4 *ambient);
//ディフューズ
extern void simple_shader_max_alpha_set_diffuse(const D3DXVECTOR4 *diffuse);
//テクスチャ
extern void simple_shader_max_alpha_set_texture(IDirect3DTexture9 *texture);
//ライトオンオフ
extern void simple_shader_max_alpha_set_lighting(bool lighting);
//時間
extern void simple_shader_max_alpha_set_time(float time);
//近クリップ面と遠クリップ面
extern void simple_shader_max_alpha_set_near_far_clip(float near_clip, float far_clip);
#endif
