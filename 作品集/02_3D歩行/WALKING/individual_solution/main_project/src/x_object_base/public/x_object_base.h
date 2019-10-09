#pragma once

#include "x_object_base_structure.h"


//���f���̎g�p�J�n
extern void x_object_base_init(IDirect3DDevice9 *device);

//���f���̎g�p�I��
extern void x_object_base_end();


//������
extern void x_object_base_t_init(x_object_base_t **object,
	x_file_type_t type,
	const vector3_t *position,
	const vector3_t *angle,
	const vector3_t *scale
);

//�I��
extern void x_object_base_t_end(x_object_base_t **object);

//�`��(���̎������Ă�����񂪂��̂܂ܕ`�悳���)
extern void x_object_base_t_draw(const x_object_base_t *object, IDirect3DDevice9 *device);
