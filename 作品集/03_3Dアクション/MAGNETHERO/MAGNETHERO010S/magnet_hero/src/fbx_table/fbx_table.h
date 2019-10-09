#pragma once
//FBX�t�@�C����ǂݍ���ŕ`�悵�A�j������Ƃ���ł�
//�쐬�ҁ@�X�m�u


#include <d3dx9.h>
#include "..\amlib\ExFbxMeshAmg.h"

//���ۂɓǂݍ��ނƂ������g���C���f�b�N�X
enum fbx_read_index_t
{
	fri_player,
	fri_stage1,
	fri_stage2,
	fri_stage3,
	//fri_stage4,
	fri_sky_sphere,
	fri_wrench_and_bibibi,
	fri_max,
};

//�e�[�u����̓����FBX���w�肷��ID
enum fbx_table_id_t
{
	fti_none,							//����
	fti_player,							//�v���[���[
	fti_stage1_base,					//�X�e�[�W1�̌����ڗp�I�u�W�F�N�g
	fti_stage1_magobj,					//�X�e�[�W1�̎��΂����蔻��p�I�u�W�F�N�g
	fti_stage1_nomagobj,				//�X�e�[�W1�̒n�ʂ����蔻��p�I�u�W�F�N�g
	fti_stage2_base,					//�X�e�[�W2�̌����ڗp�I�u�W�F�N�g
	fti_stage2_magobj,					//�X�e�[�W2�̎��΂����蔻��p�I�u�W�F�N�g
	fti_stage2_nomagobj,				//�X�e�[�W2�̒n�ʂ����蔻��p�I�u�W�F�N�g
	fti_stage3_base,					//�X�e�[�W3�̌����ڗp�I�u�W�F�N�g
	fti_stage3_magobj,					//�X�e�[�W3�̎��΂����蔻��p�I�u�W�F�N�g
	fti_stage3_nomagobj,				//�X�e�[�W3�̒n�ʂ����蔻��p�I�u�W�F�N�g
	fti_stage4_base,					//�X�e�[�W4�̌����ڗp�I�u�W�F�N�g
	fti_stage4_magobj,					//�X�e�[�W4�̎��΂����蔻��p�I�u�W�F�N�g
	fti_stage4_nomagobj,				//�X�e�[�W4�̒n�ʂ����蔻��p�I�u�W�F�N�g
	fti_sky,							//�X�J�C�X�t�B�A1
	fti_sky_sphere,						//�X�J�C�X�t�B�A2
	fti_wrench,							//�����`
	fti_bibibi_red_3d,					//�r���r���I�u�W�F�N�g
	fti_max,							//�ő�
};

//���[�V�����t��FBX�̃��[�V�������w�肷��ID
enum fbx_table_motion_id_t
{
	ftmi_none,							//����
	ftmi_player_data_start,				//�v���[���[:���[�V�����f�[�^�J�n
	ftmi_player_stand,					//�v���[���[:�������܂�
	ftmi_player_front_walk,				//�v���[���[:�O����
	ftmi_player_back_walk,				//�v���[���[:������
	ftmi_player_side_walk1,				//�v���[���[:�E����
	ftmi_player_side_walk2,				//�v���[���[:������
	ftmi_player_run,					//�v���[���[:����
	ftmi_player_jump,					//�v���[���[:�W�����v
	ftmi_player_damage,					//�v���[���[:�_���[�W
	ftmi_player_fall,					//�v���[���[:������
	ftmi_player_round,					//�v���[���[:���邮��
	ftmi_player_data_end,				//�v���[���[:���[�V�����f�[�^�I���
	ftmi_max,							//�ő�
};

//FBX�̃e�[�u����������
//���������ꂽ�i�K�ł͂܂��`��ł��Ȃ��̂ŁA��ɓǂݍ��ݏ��������悤
extern void fbx_table_init();

//FBX�̃e�[�u�����I��
//���̊֐����Ă񂾂��Ƃ�FBX�͎g���Ȃ��Ȃ��
extern void fbx_table_end();

//FBX��ǂݍ���
extern void fbx_table_load(fbx_read_index_t index);

//�ǂݍ���FBX��j������
extern void fbx_table_destroy();

//�ǂݍ���FBX��`�悷��
extern void fbx_table_draw(fbx_table_id_t looks, fbx_table_motion_id_t motion, const D3DXMATRIX *world, float motion_frame);

//�ǂݍ���FBX���V�F�[�_�[�ŕ`�悷��
extern void fbx_table_draw_with_simple_shader(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	ex_fbx_mesh_amg_blend_mode_t blend_mode
);

//�ǂݍ���FBX���V�F�[�_�[�ŃA���t�@�l�ő�ɂ��������ŕ`�悷��
extern void fbx_table_draw_with_simple_shader_max_alpha(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame
);

//�����FBX�Ɍ������ă��C���΂�
extern void fbx_table_raycast(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//�����FBX�Ɍ������đ������C���΂�
extern void fbx_table_sphere_cast(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	float radius,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//�w���FBX�̎w��̎p���ŁA�w��̃|���S���̏d�S�𓾂�
extern void fbx_table_get_polygon_position(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int polygon_id,
	D3DXVECTOR3 *pos
);

//�w���FBX�̎w��̎p���ŁA�w��͈̔͂̃|���S���̒��S�ʒu�𓾂�
extern void fbx_table_get_polygon_position_range(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int min_polygon_id,
	int max_polygon_id,
	D3DXVECTOR3 *pos
);

//�w���FBX�̎w��̎p���ŁA�w��̃��b�V���̒��S�ʒu�𓾂�
extern void fbx_table_get_mesh_position(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int mesh_id,
	D3DXVECTOR3 *pos
);

//���b�V���𖳎�����t���O���Z�b�g����
extern void fbx_table_ignore(fbx_table_id_t looks, int mesh_id, bool do_flag);

//���b�V���𖳎�����t���O�����Z�b�g����
extern void fbx_table_ignore_reset(fbx_table_id_t looks);

//���[�V����1�Đ��ɂ����鎞��(�t���[��)���擾
extern float fbx_table_motion_frame(fbx_table_motion_id_t motion);
