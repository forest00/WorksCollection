//�e�w�b�_�[
#include "sky_sphare.h"
//���̑��̃w�b�_�[
#include "..\connecter\info_holder.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\shader_use\simple_shader.h"



//�I�u�W�F�N�g���X�J�C�X�t�B�A�Ƃ��ď�����
extern void gmfo_t_sky_sphere_init(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}

//�I�u�W�F�N�g���X�J�C�X�t�B�A�Ƃ��ďI������
extern void gmfo_t_sky_sphere_end(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}

//�I�u�W�F�N�g���X�J�C�X�t�B�A�Ƃ��čX�V
extern void gmfo_t_sky_sphere_update(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}

//�I�u�W�F�N�g���X�J�C�X�t�B�A�Ƃ��ĕ`��
extern void gmfo_t_sky_sphere_draw(const game_main_fbx_object_t *object)
{
#if 1
	//���C�e�B���O�I�t
	simple_shader_set_lighting(true);
	//�J�����O�̐ݒ���擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�I�t
	mylib_set_cull_mode(D3DCULL_NONE);
	//�s����擾
	D3DXMATRIX world;
	gmfoh_t_get_world(object->_handle, &world, nullptr);
	//�`��
	fbx_table_draw_with_simple_shader(
		object->_looks,
		object->_now_motion,
		&world,
		object->_motion_frame,
		efmabm_no_blend
	);
	//�J�����O��߂�
	mylib_set_cull_mode(cull_mode);
	//���C�e�B���O�I��
	simple_shader_set_lighting(true);
#endif
}




