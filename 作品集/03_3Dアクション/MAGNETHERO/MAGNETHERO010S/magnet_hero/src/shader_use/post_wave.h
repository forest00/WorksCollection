#pragma once
/*
*�Ȃ݂Ȃ݃|�X�g�G�t�F�N�g����舵�����߂̃t�@�C���ł�
*@auther �X�@�m�u
*/



#include <d3dx9.h>


extern int post_wave_init();
extern void post_wave_end();
extern void post_wave_begin_scene();
extern void post_wave_end_scene();
extern void post_wave_commit();
//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
extern void post_wave_set_wvp(const D3DXMATRIX *wvp);
//�e�N�X�`��
extern void post_wave_set_texture(IDirect3DTexture9 *texture);
//凋C�O�e�N�X�`��
extern void post_wave_set_mirage_texture(IDirect3DTexture9 *mirage_texture);
//���ݎ���
extern void post_wave_set_now(float now);
//�����̏u��
extern void post_wave_set_origin(float origin);