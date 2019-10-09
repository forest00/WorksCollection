
//親ヘッダー
#include "flatpo_drawer.h"


//DirectXにどの頂点上を使っているのかを知らせるための頂点情報フラグを設定します。
#define fvf_flatpo_option		( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


//FVFにおいて、変数の順序とその型は厳密に決まっています。これをあべこべにするとまったく動きませんので注意が必要です。
//除算数rhwはパイプラインの最後に行うスケール変換の値で、これが構造体の中にあると「頂点が変換済みである」と判断されますので、必ず入れる必要があります。
struct fvf_flatpo_t
{
	D3DXVECTOR3 _position;			//頂点座標
	float _rhw;						//除算数
	DWORD _color;					//頂点の色
	float _u;						//テクスチャ座標U
	float _v;						//テクスチャ座標V
};


struct render_state_saver_t
{
	DWORD _z_enable;
	DWORD _cull_mode;
	DWORD _lighting;
	DWORD _ambient;
	DWORD _specular_enable;
	DWORD _alpha_blend_enable;
	DWORD _src_blend;
	DWORD _dest_blend;
	DWORD _alpha_test_enable;
	DWORD _texture_alphaop;
	DWORD _texture_colorarg1;
	DWORD _texture_colorarg2;
	IDirect3DDevice9 *_device;
};





static render_state_saver_t g_render_state_saver = {};


static inline int _color_clamp(int n)
{
	return n < 0 ? 0 : n > 255 ? 255 : n;
}





//板ポリシーン開始
extern void flatpo_begin_scene(IDirect3DDevice9 *device, float aspect)
{
	//あらかじめ状態を保存しておく
	//デバイスも保存しとく
	device->GetRenderState(D3DRS_ZENABLE, &g_render_state_saver._z_enable);
	device->GetRenderState(D3DRS_CULLMODE, &g_render_state_saver._cull_mode);
	device->GetRenderState(D3DRS_LIGHTING, &g_render_state_saver._lighting);
	device->GetRenderState(D3DRS_AMBIENT, &g_render_state_saver._ambient);
	device->GetRenderState(D3DRS_SPECULARENABLE, &g_render_state_saver._specular_enable);
	device->GetRenderState(D3DRS_ALPHABLENDENABLE, &g_render_state_saver._alpha_blend_enable);
	device->GetRenderState(D3DRS_SRCBLEND, &g_render_state_saver._src_blend);
	device->GetRenderState(D3DRS_DESTBLEND, &g_render_state_saver._dest_blend);
	device->GetRenderState(D3DRS_ALPHATESTENABLE, &g_render_state_saver._alpha_blend_enable);
	device->GetTextureStageState(0, D3DTSS_ALPHAOP, &g_render_state_saver._texture_alphaop);
	device->GetTextureStageState(0, D3DTSS_COLORARG1, &g_render_state_saver._texture_colorarg1);
	device->GetTextureStageState(0, D3DTSS_COLORARG2, &g_render_state_saver._texture_colorarg2);
	g_render_state_saver._device = device;



	// Zバッファー処理を無効
	device->SetRenderState(D3DRS_ZENABLE, false);
	// カリングをオフ
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ライトを無効
	device->SetRenderState(D3DRS_LIGHTING, false);
	// アンビエントライト（環境光）を設定する
	device->SetRenderState(D3DRS_AMBIENT, 0);
	// スペキュラ（鏡面反射）を無効
	device->SetRenderState(D3DRS_SPECULARENABLE, false);
	//アルファブレンディングを設定する
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	// テクスチャのブレンディング方法を定義する
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}

//板ポリシーン終了
extern void flatpo_end_scene()
{
	IDirect3DDevice9 *device = g_render_state_saver._device;
	//保存しておいた状態に戻す
	device->SetRenderState(D3DRS_ZENABLE, g_render_state_saver._z_enable);
	device->SetRenderState(D3DRS_CULLMODE, g_render_state_saver._cull_mode);
	device->SetRenderState(D3DRS_LIGHTING, g_render_state_saver._lighting);
	device->SetRenderState(D3DRS_AMBIENT, g_render_state_saver._ambient);
	device->SetRenderState(D3DRS_SPECULARENABLE, g_render_state_saver._specular_enable);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, g_render_state_saver._alpha_blend_enable);
	device->SetRenderState(D3DRS_SRCBLEND, g_render_state_saver._src_blend);
	device->SetRenderState(D3DRS_DESTBLEND, g_render_state_saver._dest_blend);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, g_render_state_saver._alpha_blend_enable);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, g_render_state_saver._texture_alphaop);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, g_render_state_saver._texture_colorarg1);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, g_render_state_saver._texture_colorarg2);
	g_render_state_saver._device = nullptr;
}

//板ポリの描画
extern void flatpo_draw(flatpo_t *flatpo)
{
	IDirect3DDevice9 *device = g_render_state_saver._device;
	//デバイスが存在していなければ何もしない
	if (device == nullptr)
	{
		MessageBox(0, "デバイスが見つかりません", "想定外の事態", MB_OK);
		return;
	}
	//作業用の配列
	static fvf_flatpo_t vertices[4];
	for (size_t i = 0; i < 4; i++)
	{
		vertices[i]._position = flatpo->_vertices[i]._position;
		vertices[i]._rhw = 1.0f;
		int a = _color_clamp(flatpo->_vertices[i]._alpha);
		int r = _color_clamp(flatpo->_vertices[i]._red);
		int g = _color_clamp(flatpo->_vertices[i]._green);
		int b = _color_clamp(flatpo->_vertices[i]._blue);
		vertices[i]._color = D3DCOLOR_ARGB(a, r, g, b);
		vertices[i]._u = flatpo->_vertices[i]._u;
		vertices[i]._v = flatpo->_vertices[i]._v;
	}
	device->SetTexture(0, flatpo_texture_loader_get(flatpo->_texture_id));
	device->SetFVF(fvf_flatpo_option);
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(fvf_flatpo_t));
}





