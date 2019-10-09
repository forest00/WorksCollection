//親ヘッダー
#include "diffuse_only_shader.h"
//その他のヘッダー
#include "..\mylib\mylib.h"

#define _shader_file()		("res/shader/post_wave.fx")

struct global_t
{
	ID3DXEffect *_shader;
};
static global_t g;

extern int post_wave_init()
{
	//シェーダーを読み込む
	return mylib_shader_open(_shader_file(), &g._shader);
}
extern void post_wave_end()
{
	//シェーダーを解放
	_mylib_safe_release(g._shader);
}
extern void post_wave_begin_scene()
{
	g._shader->SetTechnique("tec");
	g._shader->Begin(nullptr, D3DXFX_DONOTSAVESTATE);
	g._shader->BeginPass(0);
}
extern void post_wave_end_scene()
{
	g._shader->EndPass();
	g._shader->End();
}
extern void post_wave_commit()
{
	g._shader->CommitChanges();
}
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void post_wave_set_wvp(const D3DXMATRIX *wvp)
{
	g._shader->SetMatrix("wvp", wvp);
}
//テクスチャ
extern void post_wave_set_texture(IDirect3DTexture9 *texture)
{
	g._shader->SetTexture("tex", texture);
}
//蜃気楼テクスチャ
extern void post_wave_set_mirage_texture(IDirect3DTexture9 *mirage_texture)
{
	g._shader->SetTexture("mirage_tex", mirage_texture);
}
//現在時間
extern void post_wave_set_now(float now)
{
	g._shader->SetFloat("now", now);
}
//発生の瞬間
extern void post_wave_set_origin(float origin)
{
	g._shader->SetFloat("origin", origin);
}