//�e�w�b�_�[
#include "magnet.h"
//���̑��̃w�b�_�[
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\calc\calc.h"
#include "..\..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\..\shader_use\simple_shader.h"
#include "..\stage_event\stage_index_saver.h"


//�I�u�W�F�N�g�������蔻��t���}�b�v�Ƃ��ď�����
extern void gmfo_t_magnet_init(game_main_fbx_object_t *object)
{
	//�����ɏ���������������
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��t���}�b�v�Ƃ��ďI������
extern void gmfo_t_magnet_end(game_main_fbx_object_t *object)
{
	//�����ɏI������������
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��t���}�b�v�Ƃ��čX�V
extern void gmfo_t_magnet_update(game_main_fbx_object_t *object)
{
	//�����ɍX�V����������
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��t���}�b�v�Ƃ��ĕ`��
extern void gmfo_t_magnet_draw(const game_main_fbx_object_t *object)
{
#if 0//�܂��A�����蔻��p�̃I�u�W�F�N�g�Ȃ�ĕ`�悵�Ȃ���ˁA�͂͂�
	//�J�����O�̐ݒ���擾
	DWORD cull_mode;
	mylib_get_d3d_device()->GetRenderState(D3DRS_CULLMODE, &cull_mode);
	//�J�����O�I�t
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//�e�N�X�`�����Z�b�g(�}�[�u��)
	simple_shader_set_texture(texture_manager_get(ti_marble_0));
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
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, cull_mode);
#else
	UNREFERENCED_PARAMETER(object);
#endif
}
