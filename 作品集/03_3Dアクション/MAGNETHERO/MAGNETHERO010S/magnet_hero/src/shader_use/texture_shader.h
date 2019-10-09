#pragma once
//�e�N�X�`�����`�悪�ł���V�F�[�_�[���g�����߂̃t�@�C���ł�
//�쐬�ҁ@�X�@�m�u
//����
//2019/01/29...�t�@�C�����쐬
//����...�x�[�X�ƂȂ�֐����L�q



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
//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
extern void texture_shader_set_wvp(const D3DXMATRIX *wvp);
//�e�N�X�`��
extern void texture_shader_set_texture(IDirect3DTexture9 *texture);
#endif

#if 1
extern int texture_shader2_init();
extern void texture_shader2_end();
extern void texture_shader2_begin_scene();
extern void texture_shader2_end_scene();
extern void texture_shader2_commit();
//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
extern void texture_shader2_set_wvp(const D3DXMATRIX *wvp);
//�e�N�X�`��
extern void texture_shader2_set_texture(IDirect3DTexture9 *texture);
#endif
