#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	FBX�I�u�W�F�N�g
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	


#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include "..\..\..\..\amlib\FbxMeshAmg.h"
#include "..\..\..\..\fbx_table\fbx_table.h"

//����̃I�u�W�F�N�g���w���������߂̃n���h��
struct game_main_fbx_object_handle_t;

//�I�u�W�F�N�g�̔z��
struct game_main_fbx_object_array_t;

//�I�u�W�F�N�g�̎��
enum game_main_fbx_object_kind_t
{
	gmfok_none,						//����
	gmfok_player,					//�v���[���[
	gmfok_stage_ground,				//�n��
	gmfok_stage_magnet,				//���͂ł���������
	gmfok_bg,						//�����蔻��̂Ȃ�����
	gmfok_bg_rotate,				//�����蔻��̂Ȃ�����(��]����)
	gmfok_sky_sphere,				//�X�J�C�X�t�B�A
	gmfok_max,						//�ő�
};

//�I�u�W�F�N�g
struct game_main_fbx_object_t
{
	/*�z��̗v�f�Ƃ��ĕK�v�ȏ��*/
	game_main_fbx_object_array_t *_parrent_array;											//�����������Ă���z��
	game_main_fbx_object_handle_t *_handle;													//�������w�������n���h��
	size_t _index;																			//�����̗v�f�ԍ�
	/*�I�u�W�F�N�g�Ƃ��Ă̏��*/
	game_main_fbx_object_kind_t _kind;														//�����̎��
	fbx_table_id_t _looks;																	//�����̌�����(�ǂ�FBX���g����)
	fbx_table_motion_id_t _now_motion;														//���݂̃��[�V����
	float _motion_frame;																	//���݂̃��[�V�����o�ߎ���(�t���[��)
	D3DXVECTOR3 _position;																	//�ʒu
	D3DXVECTOR3 _rotation;																	//�p�x
	D3DXVECTOR3 _scale;																		//�g��{��
	void *_kind_parameter;																	//��ނ��Ƃ̃p�����[�^�[
	/*�e����Ɏg���֐�*/
	void(*_init)(game_main_fbx_object_t *object);											//����������֐�
	void(*_end)(game_main_fbx_object_t *object);											//�I������������֐�
	void(*_update)(game_main_fbx_object_t *object);											//�X�V����֐�
	void(*_draw)(const game_main_fbx_object_t *object);										//�`�悷��֐�
};

//�I�u�W�F�N�g�̔z��
struct game_main_fbx_object_array_t
{
	size_t _capacity;								//���E��
	game_main_fbx_object_t *_elements;				//�v�f
};


//�I�u�W�F�N�g����n���h���ɂ���
extern game_main_fbx_object_handle_t *to_handle(game_main_fbx_object_t *object);

//�n���h������I�u�W�F�N�g�ɖ߂�
extern game_main_fbx_object_t *to_object(game_main_fbx_object_handle_t *handle);

//�P�̂�������
extern void gmfo_t_init(game_main_fbx_object_t *object, game_main_fbx_object_array_t *parrent_array, size_t index);

//�P�̂𐶐�(�����Ƃ����Ă��������m�ۂ��s�����̂ł͂Ȃ��P���ɑ���݂̂�����)
extern void gmfo_t_create(
	game_main_fbx_object_t *object,
	game_main_fbx_object_kind_t kind,
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion_id,
	float motion_frame,
	const D3DXVECTOR3 *position,
	const D3DXVECTOR3 *rotation,
	const D3DXVECTOR3 *scale
);

//�P�̂��I��
extern void gmfo_t_end(game_main_fbx_object_t *object);

//�P�̂��X�V
extern void gmfo_t_update(game_main_fbx_object_t *object);

//�P�̂�`��
extern void gmfo_t_draw(game_main_fbx_object_t *object);

//�n���h�����擾
extern void gmfo_t_get_handle(const game_main_fbx_object_t *object, game_main_fbx_object_handle_t **getter);




//�z��̏�����
extern void gmfoa_t_init(game_main_fbx_object_array_t *object_array, size_t capacity);

//�z��̉��
extern void gmfoa_t_end(game_main_fbx_object_array_t *object_array);

//�z��̒��g��S���X�V
extern void gmfoa_t_update(game_main_fbx_object_array_t *object_array);

//�z��̒��g��S���`��
extern void gmfoa_t_draw(game_main_fbx_object_array_t *object_array);

//�z��̗v�f�ǉ�
extern void gmfoa_t_add(game_main_fbx_object_array_t *object_array, const game_main_fbx_object_t *obj, game_main_fbx_object_handle_t **added);

//�z��̗v�f�폜
extern void gmfoa_t_delete(game_main_fbx_object_array_t *object_array, game_main_fbx_object_handle_t **handle);

//�z��̗v�f�S�폜
extern void gmfoa_t_clear(game_main_fbx_object_array_t *object_array);

/*�z��̒�����w�肳�ꂽ��ނ̃I�u�W�F�N�g��T���ăn���h�����擾
**@param object_array		�Ώۂ̔z��
**@param kind				�Ώۂ̎��
**@param offset				�������J�n����ʒu�܂ł̃I�t�Z�b�g�Anull���w�肷��Ƃ��ׂĂ̗v�f�̒����猟������
**@param target				�������n���h�����Ԃ����A������Ȃ����null���Z�b�g�����
**@return ���������ꍇ�Atrue
*/
extern bool gmfoa_t_search_kind(game_main_fbx_object_array_t *object_array, game_main_fbx_object_kind_t kind, game_main_fbx_object_t *offset, game_main_fbx_object_handle_t **target);

//�I�u�W�F�N�g�̈ʒu���擾
extern void gmfoh_t_get_position(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *position);

//�I�u�W�F�N�g�̊p�x���擾
extern void gmfoh_t_get_rotation(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *rotation);

//�I�u�W�F�N�g�̊g��{�����擾
extern void gmfoh_t_get_scale(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *scale);

//�I�u�W�F�N�g�̈ʒu�A�p�x�A�g��{���ɂ�胏�[���h�s��𐶐�
//inverse_world��null����Ȃ��ꍇ�A�����������[���h�s��̋t�s�񂪓���
//transpose_inverse_world��null����Ȃ��ꍇ�A�����������[���h�s��̋t�s��̓]�u�s�񂪓���
extern void gmfoh_t_get_world(game_main_fbx_object_handle_t *handle, D3DXMATRIX *world, D3DXMATRIX *inverse_world);


//�I�u�W�F�N�g�Ƀ��C�𓖂Ă�
extern void gmfoh_t_raycast(
	game_main_fbx_object_handle_t *handle,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//�I�u�W�F�N�g�ɑ������C�𓖂Ă�
extern void gmfoh_t_sphere_cast(
	game_main_fbx_object_handle_t *handle,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	float radius,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//�I�u�W�F�N�g�Ƀ��b�V���𖳎�����t���O���Z�b�g
extern void gmfoh_t_set_ignore_flag(game_main_fbx_object_handle_t *handle, int mesh_id, bool value);
