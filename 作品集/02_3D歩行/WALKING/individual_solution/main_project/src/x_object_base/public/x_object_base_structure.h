#pragma once

#include "..\..\x_file\public\x_file.h"
#include "..\..\common_def\public\common_def.h"

//�ʒu
//��]
//�g��k��
//���b�V��
//
//�����̏��������Ă���\���̂ł�
//�`��̂��߂Ɏg���܂�
struct x_object_base_t
{
	vector3_t _position;				//�ʒu
	vector3_t _angle;					//�p�x
	vector3_t _scale;					//�g��{��
	x_file_t _x_file;					//�`��p���
};
