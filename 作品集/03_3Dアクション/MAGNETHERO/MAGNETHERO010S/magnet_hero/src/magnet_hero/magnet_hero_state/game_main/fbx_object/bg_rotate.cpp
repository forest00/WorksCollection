//�e�w�b�_�[
#include "bg_rotate.h"
//���̑��̃w�b�_�[
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"

//�I�u�W�F�N�g�������蔻��Ȃ��̉�]��������}�b�v�Ƃ��ď�����
extern void gmfo_t_stage_bg_rotate_init(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��Ȃ��̉�]��������}�b�v�Ƃ��ďI������
extern void gmfo_t_stage_bg_rotate_end(game_main_fbx_object_t *object)
{
	UNREFERENCED_PARAMETER(object);
}
//�I�u�W�F�N�g�������蔻��Ȃ��̉�]��������}�b�v�Ƃ��čX�V
extern void gmfo_t_stage_bg_rotate_update(game_main_fbx_object_t *object)
{
	object->_rotation.y += D3DXToRadian(0.5f);
}
//�I�u�W�F�N�g�������蔻��Ȃ��̉�]��������}�b�v�Ƃ��ĕ`��
extern void gmfo_t_stage_bg_rotate_draw(const game_main_fbx_object_t *object)
{
	//�J�����O�ݒ���擾
	DWORD now_cullmode = 0;
	mylib_get_d3d_device()->GetRenderState(D3DRS_CULLMODE, &now_cullmode);
	//�J�����O���I�t�ɂ���
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//�e�N�X�`��
	mylib_get_d3d_device()->SetTexture(0, texture_manager_get(ti_string_0));
	//�`��
	D3DXMATRIX world;
	gmfoh_t_get_world(object->_handle, &world, nullptr);
	fbx_table_draw(object->_looks, object->_now_motion, &world, object->_motion_frame);
	//�J�����O��߂�
	mylib_get_d3d_device()->SetRenderState(D3DRS_CULLMODE, now_cullmode);
}