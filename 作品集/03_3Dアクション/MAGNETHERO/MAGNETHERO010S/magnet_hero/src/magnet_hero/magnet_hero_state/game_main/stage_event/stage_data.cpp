//�e�w�b�_�[
#include "stage_data.h"
//���̑��̃w�b�_�[
#include "..\..\..\..\file_data\file_data.h"


struct stage_specific_info_t
{
	D3DXVECTOR3 _start_position;
	float _start_direcrtion;
	float _start_camera_phi;
	float _start_camera_theta;
	float _goal_threshold;
	D3DXVECTOR3 _goal_position;
};

//�O���[�o���ϐ�
struct global_t
{
	stage_specific_info_t _stage_info[stage_max];
};

static global_t g;

//�X�e�[�W����̓ǂݍ��݊֐�
static void read(file_data_id_t file_id, stage_specific_info_t *info)
{
	const datum_t *data = file_data_get(file_id, nullptr);
	info->_start_position.x = (data++)->_Float;
	info->_start_position.y = (data++)->_Float;
	info->_start_position.z = (data++)->_Float;
	info->_start_direcrtion = D3DXToRadian((data++)->_Float);
	info->_start_camera_phi = D3DXToRadian((data++)->_Float);
	info->_start_camera_theta = D3DXToRadian((data++)->_Float);
	info->_goal_threshold = (data++)->_Float;
	info->_goal_position.x = (data++)->_Float;
	info->_goal_position.y = (data++)->_Float;
	info->_goal_position.z = (data++)->_Float;
}

//������
extern void stage_data_init()
{
	//stage1�̃f�[�^�ǂݍ���
	read(fdi_stage1_event, &g._stage_info[(size_t)(stage_1)]);
	//stage2�̃f�[�^�ǂݍ���
	read(fdi_stage2_event, &g._stage_info[(size_t)(stage_2)]);
	//stage3�̃f�[�^�ǂݍ���
	read(fdi_stage3_event, &g._stage_info[(size_t)(stage_3)]);
	////stage4�̃f�[�^�ǂݍ���
	//read(fdi_stage4_event, &g._stage_info[(size_t)(stage_4)]);
}

//�X�e�[�WN�̃X�^�[�g�n�_
extern void stage_data_get_start_point(stage_index_t n, D3DXVECTOR3 *position)
{
	*position = g._stage_info[(size_t)(n)]._start_position;
}

//�X�e�[�WN�̃X�^�[�g���̃v���[���[�̌���
extern void stage_data_get_start_player_direction(stage_index_t n, float *direction)
{
	*direction = g._stage_info[(size_t)(n)]._start_direcrtion;
}

//�X�e�[�WN�X�^�[�g���̃J�����̌���
extern void stage_data_get_start_camera_angle(stage_index_t n, float *phi, float *theta)
{
	*phi = g._stage_info[(size_t)(n)]._start_camera_phi;
	*theta = g._stage_info[(size_t)(n)]._start_camera_theta;
}

//�X�e�[�WN�łǂꂾ���S�[���ɋ߂Â�����S�[���ɂ��邩
extern float stage_data_get_goal_goal_threshold(stage_index_t n)
{
	return g._stage_info[(size_t)(n)]._goal_threshold;
}

//�X�e�[�WN�̃S�[���n�_
extern void stage_data_get_goal_point(stage_index_t n, D3DXVECTOR3 *position)
{
	*position = g._stage_info[(size_t)(n)]._goal_position;
}
