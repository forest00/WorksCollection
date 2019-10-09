//親ヘッダー
#include "diffuse_only_shader.h"
//その他のヘッダー
#include "..\mylib\mylib.h"

#define _shader_file()		("res/shader/diffuse_only_shader.fx")

struct global_t
{
	ID3DXEffect *_shader;
};

static global_t g;

extern int diffuse_only_shader_init()
{
	//シェーダーを読み込む
	return mylib_shader_open(_shader_file(), &g._shader);
}
extern void diffuse_only_shader_end()
{
	_mylib_safe_release(g._shader);
}
extern void diffuse_only_shader_begin_scene(diffuse_only_shader_pass_t pass)
{
	g._shader->SetTechnique("tec");
	g._shader->Begin(nullptr, D3DXFX_DONOTSAVESTATE);
	g._shader->BeginPass((int)(pass));
}
extern void diffuse_only_shader_end_scene()
{
	g._shader->EndPass();
	g._shader->End();
}
extern void diffuse_only_shader_commit()
{
	g._shader->CommitChanges();
}
//ワールドから→ビュー→プロジェクションまでの座標変換行列　
extern void diffuse_only_shader_set_wvp(const D3DXMATRIX *wvp)
{
	g._shader->SetMatrix("wvp", wvp);
}
//ディフューズ
extern void diffuse_only_shader_set_diffuse(const D3DXVECTOR4 *diffuse)
{
	g._shader->SetVector("diffuse", diffuse);
}