
//�e�w�b�_�[
#include "flatpo_drawer.h"


//DirectX�ɂǂ̒��_����g���Ă���̂���m�点�邽�߂̒��_���t���O��ݒ肵�܂��B
#define fvf_flatpo_option		( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


//FVF�ɂ����āA�ϐ��̏����Ƃ��̌^�͌����Ɍ��܂��Ă��܂��B��������ׂ��ׂɂ���Ƃ܂����������܂���̂Œ��ӂ��K�v�ł��B
//���Z��rhw�̓p�C�v���C���̍Ō�ɍs���X�P�[���ϊ��̒l�ŁA���ꂪ�\���̂̒��ɂ���Ɓu���_���ϊ��ς݂ł���v�Ɣ��f����܂��̂ŁA�K�������K�v������܂��B
struct fvf_flatpo_t
{
	D3DXVECTOR3 _position;			//���_���W
	float _rhw;						//���Z��
	DWORD _color;					//���_�̐F
	float _u;						//�e�N�X�`�����WU
	float _v;						//�e�N�X�`�����WV
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





//�|���V�[���J�n
extern void flatpo_begin_scene(IDirect3DDevice9 *device, float aspect)
{
	//���炩���ߏ�Ԃ�ۑ����Ă���
	//�f�o�C�X���ۑ����Ƃ�
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



	// Z�o�b�t�@�[�����𖳌�
	device->SetRenderState(D3DRS_ZENABLE, false);
	// �J�����O���I�t
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ���C�g�𖳌�
	device->SetRenderState(D3DRS_LIGHTING, false);
	// �A���r�G���g���C�g�i�����j��ݒ肷��
	device->SetRenderState(D3DRS_AMBIENT, 0);
	// �X�y�L�����i���ʔ��ˁj�𖳌�
	device->SetRenderState(D3DRS_SPECULARENABLE, false);
	//�A���t�@�u�����f�B���O��ݒ肷��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	// �e�N�X�`���̃u�����f�B���O���@���`����
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}

//�|���V�[���I��
extern void flatpo_end_scene()
{
	IDirect3DDevice9 *device = g_render_state_saver._device;
	//�ۑ����Ă�������Ԃɖ߂�
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

//�|���̕`��
extern void flatpo_draw(flatpo_t *flatpo)
{
	IDirect3DDevice9 *device = g_render_state_saver._device;
	//�f�o�C�X�����݂��Ă��Ȃ���Ή������Ȃ�
	if (device == nullptr)
	{
		MessageBox(0, "�f�o�C�X��������܂���", "�z��O�̎���", MB_OK);
		return;
	}
	//��Ɨp�̔z��
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





