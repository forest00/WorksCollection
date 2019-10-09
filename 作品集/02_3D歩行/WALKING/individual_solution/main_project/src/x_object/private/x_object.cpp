
//�e�w�b�_�[
#include "..\public\x_object.h"
//���̑��̃w�b�_�[
#include <assert.h>
#include "..\..\x_object_base\public\x_object_base.h"


//�ėp�Q�[���I�u�W�F�N�g�^�̎g�p�J�n
extern void x_object_init(IDirect3DDevice9 *device)
{
	x_object_base_init(device);
}

//�ėp�Q�[���I�u�W�F�N�g�^�̎g�p�I��
extern void x_object_end()
{
	x_object_base_end();
}

//����������
void x_object_t_init(x_object_t **object,
	x_file_type_t type,
	const vector3_t *position,
	const vector3_t *angle,
	const vector3_t *scale
)
{
	assert(object);
	assert(position);
	assert(angle);
	assert(scale);
	*object = (x_object_t*)malloc(sizeof(x_object_t));
	assert(*object);
	x_object_base_t_init(&(*object)->_base, type, position, angle, scale);
}

//�������
void x_object_t_end(x_object_t **object)
{
	safe_free(*object);
}

//�`�悷��
void x_object_t_draw(const x_object_t *object, IDirect3DDevice9 *device)
{
	x_object_base_t_draw(object->_base, device);
}




