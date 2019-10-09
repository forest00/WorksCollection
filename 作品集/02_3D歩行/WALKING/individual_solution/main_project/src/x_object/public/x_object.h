#pragma once

#include <time.h>
#include "..\..\x_file\public\x_file_type.h"
#include "..\..\common_def\public\common_def.h"
#include "..\..\x_object_base\public\x_object_base_structure.h"


//�Q�[�����̕��̂Ȃǂ��i��^�ł�
//�����ėp�Q�[���I�u�W�F�N�g�^�ƌĂԂ��Ƃɂ��܂�
struct x_object_t
{
	//type var;		//�t��������
	x_object_base_t *_base;					//�ʒu+��]+�g��k��+���b�V��
};


//�ėp�Q�[���I�u�W�F�N�g�^�̎g�p�J�n
extern void x_object_init(IDirect3DDevice9 *device);

//�ėp�Q�[���I�u�W�F�N�g�^�̎g�p�I��
extern void x_object_end();

//����������
void x_object_t_init(x_object_t **object,
	x_file_type_t type,
	const vector3_t *position,
	const vector3_t *angle,
	const vector3_t *scale
);

//�������
void x_object_t_end(x_object_t **object);

//�`�悷��
void x_object_t_draw(const x_object_t *object, IDirect3DDevice9 *device);




