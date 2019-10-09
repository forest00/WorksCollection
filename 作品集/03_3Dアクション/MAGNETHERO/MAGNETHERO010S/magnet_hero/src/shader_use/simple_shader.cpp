//親ヘッダー
#include "simple_shader.h"
//その他のヘッダー
#include "..\mylib\mylib.h"

#define _shader_file()		("res/shader/simple_shader.fx")
#define _shader_max_alpha_file()		("res/shader/simple_shader_max_alpha.fx")

struct global_t
{
	ID3DXEffect *_shader;
	ID3DXEffect *_shader_max_alpha;
};
static global_t g;

#if 1
extern int simple_shader_init()
{
	//シェーダーを読み込む
	return mylib_shader_open(_shader_file(), &g._shader);
}
extern void simple_shader_end()
{
	_mylib_safe_release(g._shader);
}
extern void simple_shader_begin_scene(simple_shader_pass_t pass)
{
	g._shader->SetTechnique("tecMinimum");
	g._shader->Begin(nullptr, D3DXFX_DONOTSAVESTATE);
	g._shader->BeginPass((int)(pass));
}
extern void simple_shader_end_scene()
{
	g._shader->EndPass();
	g._shader->End();
}
extern void simple_shader_commit()
{
	g._shader->CommitChanges();
}
extern void simple_shader_set_wvp(const D3DXMATRIX *wvp)
{
	g._shader->SetMatrix("wvp", wvp);
}
extern void simple_shader_set_wit(const D3DXMATRIX *wit)
{
	g._shader->SetMatrix("wit", wit);
}
extern void simple_shader_set_light_dir(const D3DXVECTOR4 *light_dir)
{
	g._shader->SetVector("light_dir", light_dir);
}
extern void simple_shader_set_light_intensity(const D3DXVECTOR4 *light_intensity)
{
	g._shader->SetVector("light_intensity", light_intensity);
}
extern void simple_shader_set_ambient(const D3DXVECTOR4 *ambient)
{
	g._shader->SetVector("ambient", ambient);
}
extern void simple_shader_set_diffuse(const D3DXVECTOR4 *diffuse)
{
	g._shader->SetVector("diffuse", diffuse);
}
extern void simple_shader_set_texture(IDirect3DTexture9 *texture)
{
	g._shader->SetTexture("tex", texture);
}
extern void simple_shader_set_lighting(bool lighting)
{
	if (lighting)
	{
		D3DXVECTOR4 v = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		g._shader->SetVector("Ambient2", &v);
	}
	else
	{
		D3DXVECTOR4 v = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		g._shader->SetVector("Ambient2", &v);
	}
}
extern void simple_shader_set_time(float time)
{
	g._shader->SetFloat("time", time);
}
extern void simple_shader_set_near_far_clip(float near_clip, float far_clip)
{
	g._shader->SetFloat("near_clip", near_clip);
	g._shader->SetFloat("far_clip", far_clip);
}

#endif



#if 1
extern int simple_shader_max_alpha_init()
{
	//シェーダーを読み込む
	return mylib_shader_open(_shader_max_alpha_file(), &g._shader_max_alpha);
}
extern void simple_shader_max_alpha_end()
{
	_mylib_safe_release(g._shader_max_alpha);
}
extern void simple_shader_max_alpha_begin_scene()
{
	g._shader_max_alpha->SetTechnique("tecMinimum");
	g._shader_max_alpha->Begin(nullptr, D3DXFX_DONOTSAVESTATE);
	g._shader_max_alpha->BeginPass(0);
}
extern void simple_shader_max_alpha_end_scene()
{
	g._shader_max_alpha->EndPass();
	g._shader_max_alpha->End();
}
extern void simple_shader_max_alpha_commit()
{
	g._shader_max_alpha->CommitChanges();
}
extern void simple_shader_max_alpha_set_wvp(const D3DXMATRIX *wvp)
{
	g._shader_max_alpha->SetMatrix("wvp", wvp);
}
extern void simple_shader_max_alpha_set_wit(const D3DXMATRIX *wit)
{
	g._shader_max_alpha->SetMatrix("wit", wit);
}
extern void simple_shader_max_alpha_set_light_dir(const D3DXVECTOR4 *light_dir)
{
	g._shader_max_alpha->SetVector("light_dir", light_dir);
}
extern void simple_shader_max_alpha_set_light_intensity(const D3DXVECTOR4 *light_intensity)
{
	g._shader_max_alpha->SetVector("light_intensity", light_intensity);
}
extern void simple_shader_max_alpha_set_ambient(const D3DXVECTOR4 *ambient)
{
	g._shader_max_alpha->SetVector("ambient", ambient);
}
extern void simple_shader_max_alpha_set_diffuse(const D3DXVECTOR4 *diffuse)
{
	g._shader_max_alpha->SetVector("diffuse", diffuse);
}
extern void simple_shader_max_alpha_set_texture(IDirect3DTexture9 *texture)
{
	g._shader_max_alpha->SetTexture("tex", texture);
}
extern void simple_shader_max_alpha_set_lighting(bool lighting)
{
	if (lighting)
	{
		D3DXVECTOR4 v = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		g._shader_max_alpha->SetVector("Ambient2", &v);
	}
	else
	{
		D3DXVECTOR4 v = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		g._shader_max_alpha->SetVector("Ambient2", &v);
	}
}
extern void simple_shader_max_alpha_set_time(float time)
{
	g._shader_max_alpha->SetFloat("time", time);
}
extern void simple_shader_max_alpha_set_near_far_clip(float near_clip, float far_clip)
{
	g._shader_max_alpha->SetFloat("near_clip", near_clip);
	g._shader_max_alpha->SetFloat("far_clip", far_clip);
}

#endif
