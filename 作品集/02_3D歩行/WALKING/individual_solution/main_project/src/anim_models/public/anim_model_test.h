//	
//	�A�j���[�V�����t�����f��(�ȉ����f��)�̓���e�X�g�p�̃t�@�C���ł�
//	
//	�����ɂ���֐��͂��ׂ�"anim_model_"�L�[���[�h����n�܂�܂�
//	
//	�쐬��			�X	�m�u
//	�쐬��			2018/12/03
//	�ŏI�X�V��		2018/12/03
//	

#pragma once

#include <d3dx9.h>

//X�t�@�C����ǂݍ��݁A�S�Ẵ��f�������������A���߂��ɂ������o��������
extern void anim_model_tester_all_init(IDirect3DDevice9 *device);

//�S�Ẵ��f����j������
extern void anim_model_tester_all_end();

//�S�Ẵ��f�����X�V����
extern void anim_model_tester_all_update();

//�S�Ẵ��f����`�悷��
extern void anim_model_tester_all_draw(IDirect3DDevice9 *device);



//�v���C���[�Ƃ��Ďg�����f���̈ʒu�𓾂�
void anim_model_get_the_position_of_the_model_to_use_as_a_player(D3DXVECTOR3 *out);
