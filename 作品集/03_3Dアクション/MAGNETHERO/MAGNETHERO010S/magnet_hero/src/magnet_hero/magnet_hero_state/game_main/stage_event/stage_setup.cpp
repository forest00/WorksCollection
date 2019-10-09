//�e�w�b�_�[
#include "stage_setup.h"
//���̑��̃w�b�_�[
#include "stage_data.h"
#include "..\camera\camera_manager.h"
#include "..\connecter\info_holder.h"
#include "..\stage_event\stage_event.h"



//stage1
#if 1
//�X�e�[�W1�̃Z�b�g�A�b�v������
static void stage1_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBX��ǂݍ���
	fbx_table_load(fri_stage1);
	//�ꎞ�I�ɃN���G�C�g����L���������̎󂯎��M��p��
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//�v���[���[���N���G�C�g
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�����ڗp�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage1_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//���΂̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage1_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�n�ʂ̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage1_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�X�J�C�X�t�B�A���N���G�C�g
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.05f, 0.05f, 0.05f)
	);
	//�N���G�C�g�����L�������g��
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//�X�e�[�W1�̃��Z�b�g������(2019/02/05���݂͉������Ȃ�)
extern void stage1_reset()
{
	//fbx_table_ignore_reset(fti_stage1_base);
}

//�X�e�[�W1�Ɏg���Ă���I�u�W�F�N�g�����ׂĔj������
static void stage1_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif

//stage2
#if 1
//�X�e�[�W2�̃Z�b�g�A�b�v������
static void stage2_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBX��ǂݍ���
	fbx_table_load(fri_stage2);
	//�ꎞ�I�ɃN���G�C�g����L���������̎󂯎��M��p��
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//�v���[���[���N���G�C�g
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�����ڗp�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage2_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//���΂̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage2_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�n�ʂ̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage2_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�X�J�C�X�t�B�A���N���G�C�g
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.05f, 0.05f, 0.05f)
	);
	//�N���G�C�g�����L�������g��
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//�X�e�[�W2�̃��Z�b�g������(2019/02/05���݂͉������Ȃ�)
extern void stage2_reset()
{
	//fbx_table_ignore_reset(fti_stage2_base);
}

//�X�e�[�W2�Ɏg���Ă���I�u�W�F�N�g�����ׂĔj������
static void stage2_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif


//stage3
#if 1
//�X�e�[�W3�̃Z�b�g�A�b�v������
static void stage3_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBX��ǂݍ���
	fbx_table_load(fri_stage3);
	//�ꎞ�I�ɃN���G�C�g����L���������̎󂯎��M��p��
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//�v���[���[���N���G�C�g
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�����ڗp�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage3_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//���΂̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage3_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�n�ʂ̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage3_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�X�J�C�X�t�B�A���N���G�C�g
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.06f, 0.06f, 0.06f)
	);
	//�N���G�C�g�����L�������g��
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//�X�e�[�W3�̃��Z�b�g������(2019/02/05���݂͉������Ȃ�)
extern void stage3_reset()
{
	//fbx_table_ignore_reset(fti_stage3_base);
}

//�X�e�[�W3�Ɏg���Ă���I�u�W�F�N�g�����ׂĔj������
static void stage3_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif


//stage4
#if 0
//�X�e�[�W4�̃Z�b�g�A�b�v������
static void stage4_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBX��ǂݍ���
	fbx_table_load(fri_stage4);
	//�ꎞ�I�ɃN���G�C�g����L���������̎󂯎��M��p��
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//�v���[���[���N���G�C�g
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�����ڗp�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage4_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//���΂̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage4_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�n�ʂ̂����蔻��p�I�u�W�F�N�g���N���G�C�g
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage4_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//�X�J�C�X�t�B�A���N���G�C�g
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.05f, 0.05f, 0.05f)
	);
	//�N���G�C�g�����L�������g��
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//�X�e�[�W4�̃��Z�b�g������(2019/02/05���݂͉������Ȃ�)
extern void stage4_reset()
{
	//fbx_table_ignore_reset(fti_stage_base);
}

//�X�e�[�W4�Ɏg���Ă���I�u�W�F�N�g�����ׂĔj������
static void stage4_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif



//N�Ԗڂ̃X�e�[�W�̃Z�b�g�A�b�v���s���֐��ł�
//�������ς݂̃J�����ƁA�������ς݂�FBX�I�u�W�F�N�g�z��Ǘ��p�\���̂��K�v�ł�
extern void stage_setup(stage_index_t n, game_main_fbx_object_array_t *obj_array)
{
	switch (n)
	{
	case stage_none:
		break;
	case stage_1:
		stage1_setup(obj_array);
		break;
	case stage_2:
		stage2_setup(obj_array);
		break;
	case stage_3:
		stage3_setup(obj_array);
		break;
	//case stage_4:
	//	stage4_setup(obj_array);
		break;
	case stage_max:
		break;
	default:
		break;
	}
	stage_event_stage_start();
	float start_camera_phi;
	float start_camera_theta;
	stage_data_get_start_camera_angle(n, &start_camera_phi, &start_camera_theta);
	camera_manager_set_phi(start_camera_phi);
	camera_manager_set_theta(start_camera_theta);
}

//N�Ԗڂ̃X�e�[�W�����Z�b�g����֐��ł�
extern void stage_reset(stage_index_t n)
{
	switch (n)
	{
	case stage_none:
		break;
	case stage_1:
		stage1_reset();
		break;
	case stage_2:
		stage2_reset();
		break;
	case stage_3:
		stage3_reset();
		break;
	//case stage_4:
	//	stage4_reset();
		break;
	case stage_max:
		break;
	default:
		break;
	}
	float start_camera_phi;
	float start_camera_theta;
	stage_data_get_start_camera_angle(n, &start_camera_phi, &start_camera_theta);
	camera_manager_set_phi(start_camera_phi);
	camera_manager_set_theta(start_camera_theta);
}

//N�Ԗڂ̃X�e�[�W��j������֐��ł�
extern void stage_destroy(stage_index_t n, game_main_fbx_object_array_t *obj_array)
{
	switch (n)
	{
	case stage_none:
		break;
	case stage_1:
		stage1_destroy(obj_array);
		break;
	case stage_2:
		stage2_destroy(obj_array);
		break;
	case stage_3:
		stage3_destroy(obj_array);
		break;
	//case stage_4:
	//	stage4_destroy(obj_array);
		break;
	case stage_max:
		break;
	default:
		break;
	}
}
