//�V���v���V�F�[�_�[���g�����߂̏ꏊ�ł�
//�V���v���V�F�[�_�[�́A�v���~�e�B�u���A�e�t���A�e�N�X�`���t���ŕ`��ł���V�F�[�_�[�ł�
//�쐬�ҁ@�X�@�m�u
//�X�V����
//2019/01/29...�t�@�C�����쐬
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
//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
extern void simple_shader_set_wvp(const D3DXMATRIX *wvp);
//���[���h�s��̋t�s��̓]�u�s��
extern void simple_shader_set_wit(const D3DXMATRIX *wit);
//���C�g�̕����x�N�g��
extern void simple_shader_set_light_dir(const D3DXVECTOR4 *light_dir);
//���ˌ��̋��x
extern void simple_shader_set_light_intensity(const D3DXVECTOR4 *light_intensity);
//�A���r�G���g
extern void simple_shader_set_ambient(const D3DXVECTOR4 *ambient);
//�f�B�t���[�Y
extern void simple_shader_set_diffuse(const D3DXVECTOR4 *diffuse);
//�e�N�X�`��
extern void simple_shader_set_texture(IDirect3DTexture9 *texture);
//���C�g�I���I�t
extern void simple_shader_set_lighting(bool lighting);
//����
extern void simple_shader_set_time(float time);
//�߃N���b�v�ʂƉ��N���b�v��
extern void simple_shader_set_near_far_clip(float near_clip, float far_clip);
#endif


#if 1
extern int simple_shader_max_alpha_init();
extern void simple_shader_max_alpha_end();
extern void simple_shader_max_alpha_begin_scene();
extern void simple_shader_max_alpha_end_scene();
extern void simple_shader_max_alpha_commit();
//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
extern void simple_shader_max_alpha_set_wvp(const D3DXMATRIX *wvp);
//���[���h�s��̋t�s��̓]�u�s��
extern void simple_shader_max_alpha_set_wit(const D3DXMATRIX *wit);
//���C�g�̕����x�N�g��
extern void simple_shader_max_alpha_set_light_dir(const D3DXVECTOR4 *light_dir);
//���ˌ��̋��x
extern void simple_shader_max_alpha_set_light_intensity(const D3DXVECTOR4 *light_intensity);
//�A���r�G���g
extern void simple_shader_max_alpha_set_ambient(const D3DXVECTOR4 *ambient);
//�f�B�t���[�Y
extern void simple_shader_max_alpha_set_diffuse(const D3DXVECTOR4 *diffuse);
//�e�N�X�`��
extern void simple_shader_max_alpha_set_texture(IDirect3DTexture9 *texture);
//���C�g�I���I�t
extern void simple_shader_max_alpha_set_lighting(bool lighting);
//����
extern void simple_shader_max_alpha_set_time(float time);
//�߃N���b�v�ʂƉ��N���b�v��
extern void simple_shader_max_alpha_set_near_far_clip(float near_clip, float far_clip);
#endif
