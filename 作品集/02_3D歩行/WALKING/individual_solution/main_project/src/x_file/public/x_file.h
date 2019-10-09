#pragma once

//x�t�@�C����ǂݍ���ŕ`�悵�܂�
//
//	�g����
//	
//	init()	�S�ǂݍ���	������ŏ��Ɉ��
//	
//	load_from_database()	�f�[�^�x�[�X�ɂ�����̂�ǂݍ���ł��܂�
//	
//	draw()	�`�悵�܂�
//	
//	end()	�S���	������Ō�Ɉ��
//	
//	�f�[�^�x�[�X�������Ă��ĕ`��̃T�C�N��������OK
//
//
//�쐬��		�X	�m�u
//						�ŏI�X�V		2018/11/27 (12:11)


#include "x_file_structure.h"
#include "x_file_type.h"



//x�t�@�C���g�p�J�n
extern void x_file_init(IDirect3DDevice9 *device);

//x�t�@�C���g�p�I��
extern void x_file_end();

//���f�������f�[�^�x�[�X�������Ă���
extern void x_file_t_load_from_database(x_file_t *strage, x_file_type_t type);
extern void x_file_t_load_from_database(x_file_t *strage, size_t type);

//���f����`��
extern void x_file_t_draw(IDirect3DDevice9 *device, const x_file_t *x_file);

