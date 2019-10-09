#pragma once
//�F���`�悪�ł���V�F�[�_�[���g�����߂̃t�@�C���ł�
//�쐬�ҁ@�X�@�m�u



#include <d3dx9.h>


enum diffuse_only_shader_pass_t
{
	dosp_no_blend = 0,
	dosp_alpha_blend = 1,
	dosp_add_blend = 2,
};

extern int diffuse_only_shader_init();
extern void diffuse_only_shader_end();
extern void diffuse_only_shader_begin_scene(diffuse_only_shader_pass_t pass);
extern void diffuse_only_shader_end_scene();
extern void diffuse_only_shader_commit();
//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
extern void diffuse_only_shader_set_wvp(const D3DXMATRIX *wvp);
//�f�B�t���[�Y
extern void diffuse_only_shader_set_diffuse(const D3DXVECTOR4 *diffuse);
