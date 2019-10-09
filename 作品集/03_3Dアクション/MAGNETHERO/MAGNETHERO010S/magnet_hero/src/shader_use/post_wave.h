#pragma once
/*
*なみなみポストエフェクトを取り扱うためのファイルです
*@auther 森　仁志
*/



#include <d3dx9.h>


extern int post_wave_init();
extern void post_wave_end();
extern void post_wave_begin_scene();
extern void post_wave_end_scene();
extern void post_wave_commit();
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void post_wave_set_wvp(const D3DXMATRIX *wvp);
//テクスチャ
extern void post_wave_set_texture(IDirect3DTexture9 *texture);
//蜃気楼テクスチャ
extern void post_wave_set_mirage_texture(IDirect3DTexture9 *mirage_texture);
//現在時間
extern void post_wave_set_now(float now);
//発生の瞬間
extern void post_wave_set_origin(float origin);