//�e�w�b�_�[
#include "bg.h"
//���̑��̃w�b�_�[
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\..\shader_use\simple_shader.h"

//�I�u�W�F�N�g�������蔻��Ȃ��̓����Ȃ��}�b�v�Ƃ��ď�����
extern void gmfo_t_stage_bg_init(game_main_fbx_object_t *object)
{
	//�����Ȃ����珉���������͖���
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��Ȃ��̓����Ȃ��}�b�v�Ƃ��ďI������
extern void gmfo_t_stage_bg_end(game_main_fbx_object_t *object)
{
	//�����Ȃ�����I�������͖���
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��Ȃ��̓����Ȃ��}�b�v�Ƃ��čX�V
extern void gmfo_t_stage_bg_update(game_main_fbx_object_t *object)
{
	//�����Ȃ�����X�V�����͖���
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��Ȃ��̓����Ȃ��}�b�v�Ƃ��ĕ`��
extern void gmfo_t_stage_bg_draw(const game_main_fbx_object_t *object)
{	
#if 1//�`�悵�����Ȃ��Ƃ��͂����̐��l��0�ɂ���
	//�J�����O�̐ݒ���擾
	DWORD cull_mode;
	mylib_get_d3d_device()->GetRenderState(D3DRS_CULLMODE, &cull_mode);
	//�J�����O�I�t
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//�e�N�X�`�����Z�b�g(�^����)
	simple_shader_set_texture(texture_manager_get(ti_white));
	//�s����擾
	D3DXMATRIX world;
	gmfoh_t_get_world(object->_handle, &world, nullptr);
	//�`��
	fbx_table_draw_with_simple_shader_max_alpha(
		object->_looks,
		object->_now_motion,
		&world,
		object->_motion_frame
	);
	//�J�����O��߂�
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, cull_mode);
#endif
}