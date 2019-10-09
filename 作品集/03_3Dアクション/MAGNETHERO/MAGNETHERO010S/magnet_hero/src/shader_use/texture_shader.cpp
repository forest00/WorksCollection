//親ヘッダー
#include "texture_shader.h"
//その他のヘッダー
#include "..\mylib\mylib.h"

#define _shader_file()		("res/shader/texture_shader.fx")
#define _shader_file2()		("res/shader/texture_shader2.fx")

struct global_t
{
	ID3DXEffect *_shader;
	ID3DXEffect *_shader2;
};
static global_t g;

#if 1
extern int texture_shader_init()
{
	//シェーダーを読み込む
	return mylib_shader_open(_shader_file(), &g._shader);
}
extern void texture_shader_end()
{
	_mylib_safe_release(g._shader);
}
extern void texture_shader_begin_scene(texture_shader_pass_t pass)
{
	g._shader->SetTechnique("tec");
	g._shader->Begin(nullptr, D3DXFX_DONOTSAVESTATE);
	g._shader->BeginPass((int)(pass));
}
extern void texture_shader_end_scene()
{
	g._shader->EndPass();
	g._shader->End();
}
extern void texture_shader_commit()
{
	g._shader->CommitChanges();
}
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void texture_shader_set_wvp(const D3DXMATRIX *wvp)
{
	g._shader->SetMatrix("wvp", wvp);
}
//テクスチャ
extern void texture_shader_set_texture(IDirect3DTexture9 *texture)
{
	g._shader->SetTexture("tex", texture);
}
#endif

#if 1
extern int texture_shader2_init()
{
	//シェーダーを読み込む
	return mylib_shader_open(_shader_file2(), &g._shader2);
}
extern void texture_shader2_end()
{
	_mylib_safe_release(g._shader2);
}
extern void texture_shader2_begin_scene()
{
	g._shader2->SetTechnique("tec");
	g._shader2->Begin(nullptr, D3DXFX_DONOTSAVESTATE);
	g._shader2->BeginPass(0);
}
extern void texture_shader2_end_scene()
{
	g._shader2->EndPass();
	g._shader2->End();
}
extern void texture_shader2_commit()
{
	g._shader2->CommitChanges();
}
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void texture_shader2_set_wvp(const D3DXMATRIX *wvp)
{
	g._shader2->SetMatrix("wvp", wvp);
}
//テクスチャ
extern void texture_shader2_set_texture(IDirect3DTexture9 *texture)
{
	g._shader2->SetTexture("tex", texture);
}
#endif