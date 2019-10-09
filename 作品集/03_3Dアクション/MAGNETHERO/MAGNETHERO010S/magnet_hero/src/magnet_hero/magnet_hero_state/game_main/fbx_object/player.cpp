//�e�w�b�_�[
#include "player.h"
//���̑��̃w�b�_�[
#include "wrench.h"
#include "..\aiming\aiming.h"
#include "..\camera\camera_manager.h"
#include "..\sprite\sprite_integration.h"
#include "..\connecter\info_holder.h"
#include "..\stage_event\stage_data.h"
#include "..\stage_event\stage_index_saver.h"
#include "..\stage_event\stage_event.h"
#include "..\glitter\glitter_manager.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\option\option.h"
#include "..\..\..\..\calc\calc.h"
#include "..\..\..\..\input\input.h"
#include "..\..\..\..\file_reader\file_reader.h"
#include "..\..\..\..\file_data\file_data.h"
#include "..\..\..\..\sound\sound.h"
#include "..\..\..\..\shader_use\simple_shader.h"
#include "..\..\..\..\shader_use\post_wave.h"
#include "..\..\..\..\texture_manager\texture_manager.h"


//�r�[������
#define _beam_walk
//�r�[����1�t���[���ڂ̃X�N���[����ł̉��s��
#define _beam_start_z()				(0.9925f)
//�r�[���̍ŏI�t���[���̃X�N���[����ł̉��s��
#define _beam_end_z()				(0.9985f)
//�v���[���[�����S����y���W
#define _player_dead_line()			(-400.0f)
//�f�o�b�O�p�̏����o���I�v�V����
#define _output_debug_info			(0)

//�v���C���[�̊O�����
struct player_database_fbx_file_name_t
{
	char *_base;							//��{���f���̃p�X
	char *_stand;							//�������[�V�����̃p�X
	char *_front_walk;						//�O�i���[�V�����̃p�X
	char *_back_walk;						//���������[�V�����̃p�X
	char *_side_walk1;						//���������[�V�����̃p�X
	char *_side_walk2;						//���������[�V�����̃p�X
	char *_run;								//���胂�[�V�����̃p�X
	char *_jump;							//�W�����v���[�V�����̃p�X
	char *_damage;							//��_�����[�V�����̃p�X
	char *_fall;							//�������[�V�����̃p�X
};
//�v���C���[�̊O�����
struct player_database_status_t
{
	float _height;														//�g��
	float _scaling;														//�g��{��
	float _run_speed;													//���葬�x
	float _jump_pow;													//�W�����v��
	float _gravity;														//�d��
	time_t _beam_limit_frame;											//�r�[���̐�������
	float _sticking_forward_force;										//�r�[���̓��������ʒu�܂Ō�������
	float _magnetic_repulsion_force_x;									//���͔����̔�яオ���(������)
	float _magnetic_repulsion_force_y;									//���͔����̔�яオ���(�c����)
	time_t _magnetic_repulsion_duration_frame;							//���͔����̔�яオ��x�N�g����0�ɂȂ�܂ł̎���(�t���[��)
	float _qte_success_magnetic_repulsion_force_x;						//���͔����̔�яオ���(������)
	float _qte_success_magnetic_repulsion_force_y;						//���͔����̔�яオ���(�c����)
	time_t _qte_success_magnetic_repulsion_duration_frame;				//���͔����̔�яオ��x�N�g����0�ɂȂ�܂ł̎���(�t���[��)
	int _jump_count_max;												//�W�����v�\�ȉ�(1�Œ�A2�ȏゾ�Ƌ󒆃W�����v���ł���A1�������ƃW�����v�ł��Ȃ�)
	float _floating_amount_up;											//���V�A�N�V�����ŏ�ɍs����
	time_t _floating_time;												//���V�A�N�V�����ŏ�ɍs������܂ł̎���(�t���[��)
	time_t _floating_time_limit;										//���V�A�N�V�����̐�������(�t���[��)
	float _beam_shift_x;												//���_����r�[�����ˈʒu�܂ł̃x�N�g��(x)
	float _beam_shift_y;												//���_����r�[�����ˈʒu�܂ł̃x�N�g��(y)
	float _beam_shift_z;												//���_����r�[�����ˈʒu�܂ł̃x�N�g��(z)
	float _beam_thickness;												//�r�[���̑���
	bool _beam_thickness_display;										//�r�[���̑������Ə��J�[�\���ɂ����f����
	float _beam_extensive_thickness;									//�G�C���A�V�X�g�͈̔�
};
//�v���C���[�̊O�����
struct player_database_t
{
	player_database_fbx_file_name_t _fbx_file_name;
	player_database_status_t _status;
};
//�t�@�C������ǂݍ��񂾏�񂪊i�[����Ă�f�[�^�x�[�X�\���̂̃|�C���^��Ԃ�
static const player_database_t *get_database()
{
	static player_database_t database;
	static bool should_read = true;
	if (should_read)
	{
		should_read = false;
		player_database_status_t *status = &database._status;
		const datum_t *data = file_data_get(fdi_player_database, nullptr);
		status->_height = (data++)->_Float;
		status->_scaling = database._status._height / 10.0f;
		status->_run_speed = (data++)->_Float;
		status->_jump_pow = (data++)->_Float;
		status->_gravity = (data++)->_Float * -1.0f;
		status->_beam_limit_frame = (data++)->_Int;
		status->_sticking_forward_force = (data++)->_Float;
		status->_magnetic_repulsion_force_x = (data++)->_Float;
		status->_magnetic_repulsion_force_y = (data++)->_Float;
		status->_magnetic_repulsion_duration_frame = (time_t)((data++)->_Int);
		status->_qte_success_magnetic_repulsion_force_x = (data++)->_Float;
		status->_qte_success_magnetic_repulsion_force_y = (data++)->_Float;
		status->_qte_success_magnetic_repulsion_duration_frame = (time_t)((data++)->_Int);
		status->_jump_count_max = (data++)->_Int;
		status->_floating_amount_up = (data++)->_Float;
		status->_floating_time = (time_t)((data++)->_Int);
		status->_floating_time_limit = (time_t)((data++)->_Int);
		status->_beam_shift_x = (data++)->_Float;
		status->_beam_shift_y = (data++)->_Float;
		status->_beam_shift_z = (data++)->_Float;
		status->_beam_thickness = (data++)->_Float;
		status->_beam_thickness_display = (data++)->_Bool;
		status->_beam_extensive_thickness = 500.0f;
	}
	return &database;
}
//�X�e�[�gID
enum player_state_id_t
{
	psi_none,						//�X�e�[�g�Ȃ�
	psi_normal,						//�ʏ���(�ړ���W�����v���ł���)
	psi_sticking_migration,			//�������ڍs���(�r~~��������������)
	psi_sticking_stop,				//��������~���(�������ڍs��Ԃ��I�������)
	psi_max,						//�ő�
};

#if _output_debug_info
//�X�e�[�g���Ƃ̖��O
static const char *state_name(player_state_id_t state)
{
	static const char *name_table[(size_t)(psi_max)] =
	{
		"�Ȃ�",
		"�ʏ�",
		"������(�ڍs)",
		"������",
	};
	return name_table[(size_t)(state)];
}
#endif

//�A�N�V����ID
enum player_action_id_t
{
	pai_none,								//�A�N�V�����Ȃ�
	pai_run,								//����
	pai_jump,								//�W�����v
	pai_beam,								//�r~~���������Ă���
	pai_horizontal_blow,					//���������
	pai_floating,							//���V
	pai_max,								//�ő�
};

//�A�N�V�������Ƃ̖��O
#if _output_debug_info
static const char *action_name(player_action_id_t id)
{
	static const char *name_table[(size_t)(pai_max)] =
	{
		"none",
		"run",
		"jump",
		"beam",
		"horizontal blow",
		"floating",
	};
	return name_table[(size_t)(id)];
}
#endif

//���͏�񂩂�s���𔻒f����v���C���[�̔]
struct player_brain_t
{
	bool _should_run;							//����ׂ�
#ifdef _beam_walk
	bool _should_beam_walk;						//�����ׂ�
#endif
	float _input_force;							//���̗͂�(0~1)
	float _input_angle;							//���͂���Ă�p�x
	bool _should_jump;							//�W�����v����ׂ�
	bool _should_beam_fire;						//�r�[���𔭎˂���ׂ�
	bool _should_beam;							//�r�[��������������ׂ�
	bool _should_magnetic_jump;					//���͔����W�����v������ׂ�
	bool _should_floating;						//���͕��V�A�N�V����������ׂ�
};

//�v���C���[��p�̃p�����[�^�[
struct player_parameter_t
{
	player_brain_t _brain;																	//�]
	player_database_status_t _status;														//�f�[�^�x�[�X��̃X�e�[�^�X
	player_state_id_t _now_state;															//���݂̃X�e�[�g
	player_state_id_t _next_state;															//����X�V���Ƀ`�F���W����X�e�[�g
	bool _doing[(size_t)(pai_max)];															//����̃A�N�V���������s���Ă��邩
	time_t _doing_frame[(size_t)(pai_max)];													//����̃A�N�V���������s���Ă��鎞��(�t���[��)
	int _jump_count;																		//�W�����v������
	float _jump_move_y;																		//�W�����v�̈ړ���
	D3DXVECTOR3 _beam_start_position;														//�r~~���̔��ˈʒu
	D3DXVECTOR3 _beam_end_position;															//�r~~���̖ڕW�ʒu
	float _beam_t;																			//�r~~���̎���(0~1)
	D3DXVECTOR3 _sticking_position_before;													//�������ڍs�O�̈ʒu
	D3DXVECTOR3 _sticking_position_after;													//�������ڍs��̈ʒu
	float _horizontal_blow_angle;															//��������ъp�x
	D3DXVECTOR2 _horizontal_blow_move_vec;													//��������шړ��x�N�g��
	time_t _horizontal_blow_duration_frame;													//��������шړ��x�N�g����0�ɂȂ�܂ł̎���(�t���[��)
	time_t _horizontal_blow_fire_moment;													//��������т������u��
	D3DXVECTOR3 _floating_start_position;													//���V�J�n�ʒu
	bool _in_air;																			//�󒆂ɂ���
	int _remaining_num;																		//�c�@
	game_main_fbx_object_handle_t *_beam_hitting_object_handle;								//�r~~���Ɠ��������}�b�v�̃n���h��
	bool _beam_hitting;																		//�r~~���ƃ}�b�v���Փ˂��Ă��邩
	int _beam_hitting_polygon_id;															//�r~~���Ɠ��������}�b�v�̃|���S��ID
	int _beam_hitting_mesh_id;																//�r~~���Ɠ��������}�b�v�̃��b�V��ID
	game_main_fbx_object_handle_t *_beam_hitting_prediction_object_handle;					//�r~~���Ɠ��������}�b�v�̃n���h��
	bool _beam_hitting_prediction;															//�r~~���ƃ}�b�v�̏Փ˗\�����������Ă��邩
	int _beam_hitting_prediction_polygon_id;												//�r~~���ƃ}�b�v�̏Փ˗\���ɓ��������}�b�v�̃|���S��ID
	int _beam_hitting_prediction_mesh_id;													//�r~~���ƃ}�b�v�̏Փ˗\���ɓ��������}�b�v�̃��b�V��ID
	game_main_fbx_object_handle_t *_beam_hitting_extensive_prediction_object_handle;		//�r~~���Ɠ��������}�b�v�̃n���h��
	bool _beam_hitting_extensive_prediction;												//�r~~���ƃ}�b�v�̍L�͈͏Փ˗\�����������Ă��邩
	int _beam_hitting_extensive_prediction_polygon_id;										//�r~~���ƃ}�b�v�̍L�͈͏Փ˗\���ɓ��������}�b�v�̃|���S��ID
	int _beam_hitting_extensive_prediction_mesh_id;											//�r~~���ƃ}�b�v�̍L�͈͏Փ˗\���ɓ��������}�b�v�̃��b�V��ID
	D3DXVECTOR3 _shadow_position;															//�ۉe�̈ʒu
	D3DXVECTOR3 _shadow_normal;																//�ۉe�̖@��
	D3DXVECTOR3 _wrench_position;															//�����`�̈ʒu
#ifdef _beam_walk
	bool _beam_walk_mode_on;																//�r~~������
#endif
#if _output_debug_info
	bool _fordbg_draw_beam_ray;																//�f�o�b�O�p�̃r~~�����C��`�悷�邩�ǂ����̃t���O
	bool _fordbg_draw_ground_ray;															//�f�o�b�O�p�̒n�ʏՓ˃��C��`�悷�邩�ǂ����̃t���O
	D3DXVECTOR3 _fordbg_ray_position;														//�n�ʂƂ̂����蔻��Ɏg���Ă��郌�C�̎n�_(�f�o�b�O�p)
	D3DXVECTOR3 _fordbg_ray_vector;															//�n�ʂƂ̂����蔻��Ɏg���Ă��郌�C�̃x�N�g��(�f�o�b�O�p)
	D3DXVECTOR3 _fordbg_beam_ray_position;													//�r~~���̂����蔻��Ɏg���Ă��郌�C�̎n�_(�f�o�b�O�p)
	D3DXVECTOR3 _fordbg_beam_ray_vector;													//�r~~���̂����蔻��Ɏg���Ă��郌�C�̃x�N�g��(�f�o�b�O�p)
	D3DXVECTOR3 _fordbg_beam_intersect;														//�r~~���̂����蔻��Ɏg���Ă��郌�C�Ɠ��������z�Ƃ̌�_(�f�o�b�O�p)
	D3DXVECTOR3 _fordbg_beam_target_position;												//�r~~���̖ڕW�ʒu(�f�o�b�O�p)
	D3DXVECTOR3 _fordbg_ray2_position;														//2�{�ڂ̃��C�̈ʒu
	D3DXVECTOR3 _fordbg_ray2_vector;														//2�{�ڂ̃��C�̃x�N�g��
	D3DXVECTOR3 _fordbg_ray2_intersect;														//2�{�ڂ̃��C�̌�_
	float _fordbg_ray2_sub_length;															//2�{�ڂ̃��C�̈����Z��������
#endif
};


//�R���g���[���[����̓��͂��󂯎���ď���
static void brain_process(player_brain_t *brain)
{
	//���͂̎擾
	const virtual_input_t *repeat = mylib_get_virtual_input_repeat();
	const virtual_input_t *trigger = mylib_get_virtual_input_trigger();
	//�㉺���E�̓��̓x�N�g�������(2����)
	D3DXVECTOR2 input_vector = D3DXVECTOR2(0.0f, 0.0f);
	//x����
	input_vector.x += repeat->_lstick_x;
	if (fabsf(input_vector.x) <= 0.0f)
	{
		input_vector.x += (float)((int)(repeat->_lkey_right));
		input_vector.x -= (float)((int)(repeat->_lkey_left));
	}
	//y����
	input_vector.y -= repeat->_lstick_y;
	if (fabsf(input_vector.y) <= 0.0f)
	{
		input_vector.y += (float)((int)(repeat->_lkey_up));
		input_vector.y -= (float)((int)(repeat->_lkey_down));
	}
	//�W�����v�̓��̓`�F�b�N
	brain->_should_jump = trigger->_b;
	//�}�O�l�b�g�r~~���̔��˂����邩���̓`�F�b�N
	brain->_should_beam_fire = trigger->_a;
	//�}�O�l�b�g�r~~�������������邩���̓`�F�b�N(�P�������ōŌ�܂ł����悤�Ɏd�l�ύX����܂���)
	brain->_should_beam = true;
	//���͔����W�����v�A�N�V�����̓��̓`�F�b�N
#ifndef qte_lost
	brain->_should_magnetic_jump = true;
#else
	brain->_should_magnetic_jump = trigger->_a;
#endif
	//���V�A�N�V�����̓��̓`�F�b�N
	brain->_should_floating = false;
	//���͂̊p�x
	if (input_vector.x || input_vector.y)
	{
		brain->_input_angle = atan2f(input_vector.y, -input_vector.x);
	}
	//���̓x�N�g���̑傫��
	float input_vector_length = sqrtf(input_vector.x * input_vector.x + input_vector.y * input_vector.y);
	//���͏�񂩂�s���𔻒f
	brain->_should_run = input_vector_length >= 0.1;
	brain->_input_force = input_vector_length;
#ifdef _beam_walk
	//�����ׂ�
	brain->_should_beam_walk = brain->_should_beam;
#endif
}

#if 1
//���͐�����т����Ă��炠�܂莞�Ԃ������Ă��Ȃ����Ƃ𔻒肷��֐�
static bool not_much_time_has_come(const game_main_fbx_object_t *object, const player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	float Threshold = mylib_frame_to_second(param->_horizontal_blow_duration_frame, 60.0f) / 5.0f;
	return (timeGetTime() - param->_horizontal_blow_fire_moment) / 1000.0f < Threshold;
}

//���菈��
static void run_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	bool should_run = brain->_should_run;
#ifdef _beam_walk
	//�����ׂ����r~~���������Ă���Ԃ͑���ׂ��ł͂Ȃ�
	if (param->_beam_walk_mode_on && param->_doing[pai_beam])
	{
		should_run = false;
	}
#endif
	//���͐�����т̏����̊Ԃ͑���ׂ��ł͂Ȃ�
	if (not_much_time_has_come(object, param))
	{
		should_run = false;
	}
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_run);
	//�J�����̉������̊p�x
	float camera_phi = camera_manager_get_phi();
	//���t���[���̑������
	float run_angle = camera_phi - brain->_input_angle - (float)(M_PI_2);
	//�t���O���Z�b�g
	param->_doing[action_index] = false;
	//����ׂ���
	if (should_run)
	{
		//�ʒu�X�V
		object->_position.x += brain->_input_force * param->_status._run_speed * cosf(run_angle);
		object->_position.z += brain->_input_force * param->_status._run_speed * sinf(run_angle);
		//�p�x���X�V
		object->_rotation.y = -camera_phi + brain->_input_angle - (float)(M_PI);
		//�t���O�Z�b�g
		param->_doing[action_index] = true;
	}
	//�J�E���g
	if (param->_doing[action_index])
	{
		param->_doing_frame[action_index]++;
	}
	else
	{
		param->_doing_frame[action_index] = 0;
	}
}

//�W�����v�ł��邩�`�F�b�N
static bool can_jump(const player_parameter_t *param)
{
	//�W�����v�񐔂͓K����
	if (param->_jump_count >= param->_status._jump_count_max)
	{
		//�W�����v�ł��Ȃ�
		return false;
	}
	//�W�����v�ł���
	return true;
}

//�W�����v����
static void do_jump(player_parameter_t *param, float jump_pow)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_jump);
	//�t���O���Z�b�g
	param->_doing[action_index] = true;
	//���Ԃ��Z�b�g
	param->_doing_frame[action_index] = 0;
	//�ړ��ʂ��Z�b�g
	param->_jump_move_y = jump_pow;
	//�J�E���g�A�b�v
	param->_jump_count++;
	//�󒆂ɂ���
	param->_in_air = true;
	//���Ԃ�n��
	player_do_jump_moment_set(mylib_now_sec());
}

//���n
static void landing(player_parameter_t *param)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_jump);
	//�W�����v�̈ړ��ʂ̓[��
	param->_jump_move_y = 0.0f;
	//�t���O���Z�b�g
	param->_doing[action_index] = false;
	//���Ԃ��Z�b�g
	param->_doing_frame[action_index] = 0;
	//�W�����v�񐔂����Z�b�g
	param->_jump_count = 0;
	//�󒆂ɂ��Ȃ�
	param->_in_air = false;
}

//�W�����v����
static void jump_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(object);
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_jump);
	//�W�����v����ׂ���
	if (brain->_should_jump)
	{
		//�W�����v�ł��邩
		if (can_jump(param))
		{
			//�W�����v����
			do_jump(param, param->_status._jump_pow);
			//����炷
			sound_play(si_se_jump);
		}
	}
	//�J�E���g�A�b�v
	if (param->_doing[action_index])
	{
		param->_doing_frame[action_index]++;
	}
}

//��������т�����
static void do_horizontal_blow(player_parameter_t *param, float move_pow, time_t move_duration, float move_angle)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_horizontal_blow);
	//��������ї͂����߂�
	D3DXVECTOR2 move_vec = D3DXVECTOR2(
		move_pow * sinf(move_angle),
		move_pow * cosf(move_angle)
	);
	//���߂���������ї͂��g��
	param->_horizontal_blow_move_vec = move_vec;
	//�t���O���Z�b�g
	param->_doing[action_index] = true;
	//���Ԃ��Z�b�g
	param->_doing_frame[action_index] = 0;
	param->_horizontal_blow_duration_frame = move_duration;
	param->_horizontal_blow_fire_moment = timeGetTime();
}

//��������ђ��Ɏ��s���鏈��
static void horizontal_blow_process(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_horizontal_blow);
	//��������ђ���
	if (param->_doing[action_index])
	{
		//��������т̏����𑱍s���ׂ���
		if (param->_doing_frame[action_index] < param->_horizontal_blow_duration_frame)
		{
			//���s���ׂ��ł���
			float t =
				(float)(param->_doing_frame[action_index]) /
				(float)(param->_horizontal_blow_duration_frame);
			D3DXVECTOR2 move_vec_2d = param->_horizontal_blow_move_vec * (1.0f - t);
			D3DXVECTOR3 move_vec_3d = D3DXVECTOR3(move_vec_2d.x, 0.0f, move_vec_2d.y);
			object->_position += move_vec_3d;
			//���Ԃ��X�V
			param->_doing_frame[action_index]++;
		}
		else
		{
			//������߂�ׂ��ł���
			param->_doing[action_index] = false;
			//���Ԃ��Z�b�g
			param->_doing_frame[action_index] = 0;
		}
	}
}

//���V���J�n���Ă���n�t���[���ڂ̕��V�J�n�ʒu���猩���ړ��ʂ����߂�
static float floating_position(player_parameter_t *param, time_t n)
{
	//�v�Z���ĕԂ�
	//���Ԃ�0~1�ɕϊ�
	float t = (float)(n) / (float)(param->_status._floating_time);
	t = min(t, 1.0f);
	return t * param->_status._floating_amount_up;
}

//���V���J�n���Ă���n�t���[���ڂ�y���W�̈ړ��ʂ����߂�
static float floating_move_amount(player_parameter_t *param, time_t n)
{
	//�ړ��� = ���̃t���[���̈ʒu - ���̃t���[���̈ʒu
	return floating_position(param, n + 1) - floating_position(param, n);
}

//���V�ł��邩�`�F�b�N
static bool can_floating(const player_parameter_t *param)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_floating);
	//���ԃ`�F�b�N
	if (param->_doing_frame[action_index] >= param->_status._floating_time_limit)
	{
		//���V�ł��Ȃ�
		return false;
	}
	//���V�ł���
	return true;
}

//���V����
static void do_floating(player_parameter_t *param)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_floating);
	//���V�t���O���I��
	param->_doing[action_index] = true;
	//���V�J�E���^���X�V
	param->_doing_frame[action_index]++;
	//�W�����v�̈ړ��ʂ�ł�����
	param->_jump_move_y = 0.0f;
	//�󒆂ɂ���
	param->_in_air = true;
}

//���V����
static void floating_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//���͐�����т̏����̊Ԃ͕��V����ׂ��ł͂Ȃ�
	bool should_floating = brain->_should_floating;
	if (not_much_time_has_come(object, param))
	{
		should_floating = false;
	}
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_floating);
	//���V�t���O���Z�b�g
	param->_doing[action_index] = false;
	//���V����ׂ���
	if (should_floating)
	{
		//���V�ł��邩
		if (can_floating(param))
		{
			//���V����
			do_floating(param);
			//���V�J�n�̏u�Ԃ��������o��
			if (param->_doing_frame[action_index] == 1)
			{
				sound_play(si_se_maghover);
			}
		}
	}
	else
	{
		//�󒆂ɂ��邩�`�F�b�N
		if (param->_in_air == false)
		{
			//���Ԃ����Z�b�g
			param->_doing_frame[action_index] = 0;
		}
	}
	//���V�t���O�`�F�b�N
	if (param->_doing[action_index])
	{
		//���V���ԃ`�F�b�N
		if (param->_doing_frame[action_index] < param->_status._floating_time_limit)
		{
			//�ړ��ʂ����߂�
			float move_amount = floating_move_amount(param, param->_doing_frame[pai_floating]);
			//�ړ��ʂ̕������ړ�
			object->_position.y += move_amount;
		}
	}
}

//�d�͂𖳎�����ׂ������肷��
static bool ignore_gravity(player_parameter_t *param)
{
	//���V��
	if (param->_doing[(size_t)(pai_floating)])
	{
		//��������ׂ�
		return true;
	}
	//��������ׂ��ł͂Ȃ�
	return false;
}

//�d�͏���
static void gravity_process(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//�d�͂𖳎�����^�C�~���O�Ȃ牽�����Ȃ�
	if (ignore_gravity(param))
	{
		return;
	}
	//y���W�X�V
	object->_position.y += param->_jump_move_y;
	//�d�͕��������Z
	param->_jump_move_y += param->_status._gravity;
}

//���͔���������
static void do_magnetic_jump(game_main_fbx_object_t *object, player_parameter_t *param, time_t move_duration, float force_x, float force_y)
{
	//�W�����v
	do_jump(param, force_y);
	//���ړ�
	do_horizontal_blow(param, force_x, move_duration, param->_horizontal_blow_angle);
	//�p�x��ς���
	object->_rotation.y = param->_horizontal_blow_angle + (float)(M_PI);
	//�W�����v�񐔂�1�ɂ���
	param->_jump_count = 1;
	//���V�J�E���^���[����
	param->_doing_frame[(size_t)(pai_floating)] = 0;
}

//�n�ʂƂ̂����蔻��&�ʒu�C��&���n����
//�����ɉe�̈ʒu���v�Z���܂�
static void ground_hit_process(game_main_fbx_object_t *object, const player_brain_t *brain, player_parameter_t *param)
{
	float y_shift_base = 6.0f;
	//�n�ʂ̃I�u�W�F�N�g��������
	game_main_fbx_object_handle_t *map;
	//���Ƃ̔���
	{
		gmfoa_t_search_kind(object->_parrent_array, gmfok_stage_ground, nullptr, &map);
		//���C���ˈʒu���v�Z
		D3DXVECTOR3 ray_position = object->_position;
		float y_shift = y_shift_base + max(-param->_jump_move_y, 0.0f);
		ray_position.y += y_shift;
		D3DXVECTOR3 intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		D3DXVECTOR3 normal = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		D3DXVECTOR3 ray_vector = D3DXVECTOR3(0.0f, -500.0f, 0.0f);
		{
			//���C���΂�
			bool hitting = false;
			gmfoh_t_raycast(map, &ray_position, &ray_vector, &intersect, &normal, nullptr, &hitting, nullptr);
			//��΂������C���}�b�v�Ɠ�����Έʒu�X�V
			if (hitting)
			{
				//�v���[���[�����������v���[���[�̑����n�ʂ�����
				if ((param->_jump_move_y <= 0.0f) && (object->_position.y <= intersect.y))
				{
					//�ʒu�X�V
					object->_position.y = intersect.y;
					//���n����
					landing(param);
				}
			}
			//�e�̈ʒu�A�@������
			param->_shadow_position = intersect;
			param->_shadow_normal = normal;
#if _output_debug_info
			//�f�o�b�O�p�ɏ����Z�b�g
			param->_fordbg_ray_position = ray_position;
			param->_fordbg_ray_vector = ray_vector;
#endif
		}
	}
	//�ǂƂ̔���
	{
		//���C���ˈʒu���v�Z
		D3DXVECTOR3 ray_position = object->_position;
		ray_position.y += y_shift_base;
		//�����̈ʒu���玩���̐i�s�����Ɍ������ă��C���΂�
		float pl_angle = camera_manager_get_phi() - brain->_input_angle - (float)(M_PI_2);
		D3DXVECTOR3 ray_vector = D3DXVECTOR3(cosf(pl_angle), 0.0f, sinf(pl_angle)) * 100.0f;
		D3DXVECTOR3 intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		D3DXVECTOR3 normal = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		bool hitting = false;
		gmfoh_t_raycast(map, &ray_position, &ray_vector, &intersect, &normal, nullptr, &hitting, nullptr);
#if _output_debug_info
		//�f�o�b�O�p���ۑ�
		param->_fordbg_ray2_position = ray_position;
		param->_fordbg_ray2_vector = ray_vector;
#endif
		//��΂������C�������Ɠ��������炻��͕ǂł���
		if (hitting)
		{
			//���ˈʒu�����_�܂ł̃x�N�g��
			D3DXVECTOR3 sub = intersect - ray_position;
#if _output_debug_info
			param->_fordbg_ray2_intersect = intersect;
			param->_fordbg_ray2_sub_length = D3DXVec3Length(&sub);
#endif
			//���̒��������ȉ��ł��邩
			float threshold = 4.0f;
			if (D3DXVec3LengthSq(&sub) <= threshold * threshold)
			{
				//�P�ʉ�
				D3DXVECTOR3 norm;
				D3DXVec3Normalize(&norm, &sub);
				//�����̈ʒu���C��
				intersect.y -= y_shift_base;
				object->_position = intersect + norm * D3DXVec3Dot(&norm, &normal) * threshold;
			}
		}
	}
}

//�n�ʂƂ̂����蔻�����炸�e�̈ʒu�����v�Z����
static void calc_shadow_position(const game_main_fbx_object_t *object, player_parameter_t *param)
{
	//�n�ʂ̃I�u�W�F�N�g��������
	game_main_fbx_object_handle_t *map;
	gmfoa_t_search_kind(object->_parrent_array, gmfok_stage_ground, nullptr, &map);
	//���C���ˈʒu���v�Z
	D3DXVECTOR3 ray_position = object->_position;
	float y_shift = 8.0f;
	ray_position.y += y_shift;
	D3DXVECTOR3 intersect = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 normal = {};
	D3DXVECTOR3 ray_vector = D3DXVECTOR3(0.0f, -500.0f, 0.0f);
	//���C���΂�
	bool hitting = false;
	gmfoh_t_raycast(map, &ray_position, &ray_vector, &intersect, &normal, nullptr, &hitting, nullptr);
	//�e�̈ʒu�A�@������
	param->_shadow_position = intersect;
	param->_shadow_normal = normal;
#if _output_debug_info
	//�f�o�b�O�p�ɏ����Z�b�g
	param->_fordbg_ray_position = ray_position;
	param->_fordbg_ray_vector = ray_vector;
#endif
}

//�r~~����������
//���̊֐����ŁA���t���[���̃r�[���̔��ˈʒu�ƏI���ʒu���v�Z���܂�
//�`��p�̃����`�̈ʒu���v�Z���܂�
static void beam_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//�C���f�b�N�X���擾
	size_t action_index = (size_t)(pai_beam);
	//�r~~�������ׂ���
	if (brain->_should_beam_fire)
	{
		//����A�r~~���������Ă��Ȃ���
		if (param->_doing[action_index] == false)
		{
			//�t���O��؂�ւ�
			param->_doing[action_index] = true;
			//�����o��
			sound_play(si_se_beam);
			//�X�v���C�g�łт�т���o��
			sprite_integration_bibibi_fire(10, 10);
			//�V�F�[�_�[�Ɏ��Ԃ�n��
			post_wave_set_origin(mylib_frame_to_second(mylib_get_frame_count(), 60.0f));
		}
	}
	//�r~~��������������t���O�������Ă��邩�ǂ����擾
	bool should_beam = brain->_should_beam;
	//�r~~���̐������Ԃ𒴂��Ă��邩
	if (param->_doing_frame[action_index] >= param->_status._beam_limit_frame)
	{
		//�r~~��������������ׂ��ł͂Ȃ�
		should_beam = false;
	}
	//�r~~���𔭎˂�����t���O�������Ă���
	if (param->_doing[action_index])
	{
		//�r~~��������������ׂ�
		if (should_beam)
		{
			//���t���[���̃r~~���̔��ˈʒu���v�Z
			float cursor_x = 0.0f;
			float cursor_y = 0.0f;
			float cursor_z = _beam_start_z();
			D3DXVECTOR3 beam_start_screen_pos = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&param->_beam_start_position, &beam_start_screen_pos);
			//���t���[���̃r~~���̏I���ʒu���v�Z
			aiming_cursor_get_pos(&cursor_x, &cursor_y, nullptr);
			param->_beam_t = (float)(param->_doing_frame[action_index] + 1) / (float)(param->_status._beam_limit_frame);
			cursor_z = lerpf(_beam_start_z(), _beam_end_z(), param->_beam_t);
			D3DXVECTOR3 beam_end_screen_pos = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&param->_beam_end_position, &beam_end_screen_pos);
			{
				//�J�����̊p�x���擾
				float camera_phi = camera_manager_get_phi();
				//float camera_theta = camera_manager_get_theta();
				//�������甭�ˈʒu�܂ł̃��[�J���x�N�g�����擾
				D3DXVECTOR3 vec3 = D3DXVECTOR3(
					param->_status._beam_shift_x * param->_status._scaling,
					param->_status._beam_shift_y * param->_status._scaling,
					param->_status._beam_shift_z * param->_status._scaling
				);
				//�J�����̊p�x�ɂ���ĉ�]������
				D3DXVECTOR4 vec4;
				D3DXMATRIX camera_mat;
				D3DXMatrixRotationY(&camera_mat, -camera_phi);
				D3DXVec3Transform(&vec4, &vec3, &camera_mat);
				vec3 = D3DXVECTOR3(vec4.x, vec4.y, vec4.z);
				//�����`�̊J�n�ʒu���v�Z
				D3DXVECTOR3 start_pos = object->_position + vec3;
				//�����`�̏I���ʒu���v�Z
				D3DXVECTOR3 end_pos = param->_beam_end_position;
				//�����`�̈ʒu���v�Z
				D3DXVec3Lerp(&param->_wrench_position, &start_pos, &end_pos, param->_beam_t);
			}
#if _output_debug_info
			//�f�o�b�O�p�ɏ��ۑ�
			param->_fordbg_beam_target_position = param->_beam_end_position;
#endif
		}
		else
		{
			//�r�[������������ׂ��ł͂Ȃ��̂ŁA�t���O�𗧂ĂȂ�
			param->_doing[action_index] = false;
		}
	}
	//�r~~���J�E���^���X�V
	if (param->_doing[action_index])
	{
		param->_doing_frame[action_index]++;
	}
	else
	{
		param->_doing_frame[action_index] = 0;
	}
}

#ifdef _beam_walk
static void change_beam_walk_mode(player_parameter_t *param, const player_brain_t *brain)
{
	param->_beam_walk_mode_on = brain->_should_beam_walk;
}

static void beam_walk_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//�r~~���������[�h���I�t�Ȃ牽�����Ȃ�
	if (param->_beam_walk_mode_on == false)
	{
		return;
	}
	//�r�[���������Ă��Ȃ���Ή������Ȃ�
	if (param->_doing[(size_t)(pai_beam)] == false)
	{
		return;
	}
	//�J�����̉������̊p�x
	float camera_phi = camera_manager_get_phi();
	//�v���C���[�������ׂ����������߂�
	float walk_angle = camera_phi - brain->_input_angle - (float)(M_PI_2);
	//�����ׂ������ɕ���
	object->_position.x += brain->_input_force * cos(walk_angle) * param->_status._run_speed / 2.5f;
	object->_position.z += brain->_input_force * sin(walk_angle) * param->_status._run_speed / 2.5f;
	//�p�x��ς���
	object->_rotation.y = -camera_phi - (float)(M_PI_2);
}
#endif

//�r~~���ƃ}�b�v�̂����蔻��
static void check_beam_hit_map(
	game_main_fbx_object_t *object,
	D3DXVECTOR3 *start_pos,
	D3DXVECTOR3 *end_pos,
	float thickness,
	game_main_fbx_object_handle_t **map,
	bool *hitting,
	int *polygon_id,
	int *mesh_id,
	D3DXVECTOR3 *intersect,
#if _output_debug_info
	D3DXVECTOR3 *normal,
	D3DXVECTOR3 *dbg_ray_position, D3DXVECTOR3 *dbg_ray_vector, D3DXVECTOR3 *dbg_intersect
#else
	D3DXVECTOR3 *normal
#endif
)
{
	//���C�̊J�n�ʒu����I���ʒu�܂ł̃x�N�g��
	D3DXVECTOR3 ray_vector = *end_pos - *start_pos;
	//�����T��
	if (gmfoa_t_search_kind(object->_parrent_array, gmfok_stage_magnet, nullptr, map))
	{
		if (normal)
		{
			gmfoh_t_sphere_cast(*map, start_pos, &ray_vector, thickness, intersect, normal, polygon_id, hitting, mesh_id);
		}
		else
		{
			D3DXVECTOR3 _normal = {};
			gmfoh_t_sphere_cast(*map, start_pos, &ray_vector, thickness, intersect, &_normal, polygon_id, hitting, mesh_id);
		}
	}
#if _output_debug_info
	//�f�o�b�O�p�ɏ����Z�b�g
	if (dbg_ray_position)
	{
		*dbg_ray_position = *start_pos;
	}
	if (dbg_ray_vector)
	{
		*dbg_ray_vector = ray_vector;
	}
	if (dbg_intersect)
	{
		*dbg_intersect = *intersect;
	}
#endif
}

//�r~~���ƃ}�b�v�̂����蔻������ׂ����Ԃ�
static bool should_check_beam_hit_map(const player_parameter_t *param)
{
	return param->_doing[(size_t)(pai_beam)];
}

//�������ڍs����
static void sticking_migration_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(brain);
	//�ړ��ʂ����߂�
	D3DXVECTOR3 normalized;
	D3DXVECTOR3 not_normalized = D3DXVECTOR3(param->_sticking_position_after - object->_position);
	D3DXVec3Normalize(&normalized, &not_normalized);
	D3DXVECTOR3 movevec = normalized * param->_status._sticking_forward_force;
	//�ʒu�X�V
	object->_position += movevec;
}

//�������ڍs�������I��������Ƃ𔻒�
static bool sticking_migration_ended(game_main_fbx_object_t *object, player_parameter_t *param)
{
	D3DXVECTOR3 vec1 = D3DXVECTOR3(object->_position - param->_sticking_position_before);
	D3DXVECTOR3 vec2 = D3DXVECTOR3(param->_sticking_position_after - param->_sticking_position_before);
	float t = D3DXVec3LengthSq(&vec1);
	float threshold = D3DXVec3LengthSq(&vec2);
	return t > threshold;
}

//��������~����
static void sticking_stop_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(param);
	//��]
	if (brain->_should_run)
	{
		object->_rotation.y = -camera_manager_get_phi() + brain->_input_angle - (float)(M_PI);
	}
}

//�v���C���[���������������Ƃ𔻒肷��֐�
static bool is_fell(const game_main_fbx_object_t *object)
{
	return object->_position.y < -1000.0f;
}

//�����������畜�����鏈��
static void respawn_if(game_main_fbx_object_t *object, player_parameter_t *param, bool fire)
{
	if (fire)
	{
		//�����ʒu���v�Z
		D3DXVECTOR3 respawn_position;
		stage_data_get_start_point(stage_get_now(), &respawn_position);
		//�����ʒu�ɓ�����
		object->_position = respawn_position;
		//�W�����v�̉񐔂����Z�b�g
		param->_jump_count = min(param->_jump_count, 1);
		//�W�����v�̈ړ��ʂ����Z�b�g
		param->_jump_move_y = 0.0f;
		//��Ԃ����Z�b�g
		param->_next_state = psi_normal;
		//�c�@�����炷
		param->_remaining_num--;
		//�����������Ƃ�`����
		holder_falled_player_record();
	}
}

//���炫��G�t�F�N�g��n�o��
static void generate_glitter(const game_main_fbx_object_t *object, const player_parameter_t *param, int n)
{
	for (int i = 0; i < n; i++)
	{
		//�ړ���
		D3DXVECTOR3 v(
			uniform_randf(-1.0f, 1.0f),
			uniform_randf(-1.0f, 1.0f),
			uniform_randf(-1.0f, 1.0f)
		);
		v *= 50.0f;
		//�J�n�ʒu
		D3DXVECTOR3 pos = object->_position;
		pos.y += param->_jump_move_y + 8.0f;
		//�I���ʒu
		D3DXVECTOR3 end_pos = pos + v;
		//���炫��G�t�F�N�g���o�����߂̏����܂Ƃ߂�
		const float scale_max = 25.0f;
		const float scale_min = 1.0f;
		const float life_sec_max = 3.5f;
		const float life_sec_min = 0.5f;
		const float scale_base = uniform_randf(0.0f, 1.0f);
		const float scale = scale_base * (scale_max - scale_min) + scale_min;
		const float life_sec = (1.0f - scale_base) * (life_sec_max - life_sec_min) + life_sec_min;
		float point = 2.0f;
		const float red = uniform_randf(0.0f, 1.0f);
		point -= red;
		const float green = uniform_randf(0.0f, 1.0f);
		point -= green;
		point = min(point, 1.0f);
		const float blue = uniform_randf(0.0f, point);
		const glitter_add_t info = {
			gk_random(),
			life_sec,
			scale,
			et_EaseOutRandom(ioel_include_linear),
			pos,
			end_pos,
			red,
			green,
			blue
		};
		//���炫��G�t�F�N�g���o��
		glitter_manager_add(&info);
	}
}

//���͖������̉����𗬂�
static void sound_play_hit_beam()
{
	sound_index_t index = si_se_voice_magjump;
	int index_add = rand() % 2;
	index = (sound_index_t)((int)(index)+index_add);
	sound_play(index);
}
#endif

//�S��Ԃɂ����čs���鎖�O����(�X�e�[�g���Ƃ̍X�V�O�ɌĂ΂��)
static void state_common_pre_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(brain);
	//'S'�A'K'�A'I'�𓯎������ŃS�[���Ƀ��[�v(�f�o�b�O�p�̋@�\)
	if (
		input_keyboard_repeat(DIK_S) &&
		input_keyboard_repeat(DIK_K) &&
		input_keyboard_repeat(DIK_I)
		)
	{
		stage_data_get_goal_point(stage_get_now(), &object->_position);
		object->_position.y += 0.5f;
	}
	//�r�[���̏Փ˗\��
	{
		bool beam_hitting_prediction;
		float cursor_x = 0.0f;
		float cursor_y = 0.0f;
		float cursor_z = _beam_start_z();
		D3DXVECTOR3 beam_intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		game_main_fbx_object_handle_t *map = nullptr;
		int polygon_id = -1;
		int mesh_id = -1;
		D3DXVECTOR3 start_position;
		D3DXVECTOR3 beam_screen_pos_start = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
		mylib_to_world_pos(&start_position, &beam_screen_pos_start);
		D3DXVECTOR3 end_position;
		aiming_cursor_get_pos(&cursor_x, &cursor_y, nullptr);
		cursor_z = _beam_end_z();
		D3DXVECTOR3 beam_screen_pos_end = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
		mylib_to_world_pos(&end_position, &beam_screen_pos_end);
		float thickness = 0.0;
		if (param->_status._beam_thickness_display)
		{
			thickness = param->_status._beam_thickness;
		}
		check_beam_hit_map(
			object,
			&start_position,
			&end_position,
			thickness,
			&map,
			&beam_hitting_prediction,
			&polygon_id,
			&mesh_id,
			&beam_intersect,
#if _output_debug_info
			nullptr,
			nullptr,
			nullptr,
			nullptr
#else
			nullptr
#endif
		);
		if (beam_hitting_prediction)
		{
			param->_beam_hitting_prediction_object_handle = map;
			param->_beam_hitting_prediction = true;
			param->_beam_hitting_prediction_polygon_id = polygon_id;
			param->_beam_hitting_prediction_mesh_id = mesh_id;
		}
		else
		{
			param->_beam_hitting_prediction_object_handle = nullptr;
			param->_beam_hitting_prediction = false;
			param->_beam_hitting_prediction_polygon_id = -1;
			param->_beam_hitting_prediction_mesh_id = -1;
		}
		//�Ə��J�[�\�����I�����W�F�ɂ���
		player_beam_hit_object_set(beam_hitting_prediction);
	}
	//�r�[���̍L�͈͏Փ˗\��
	{
		bool beam_hitting_extensive_prediction = false;
		float cursor_x = 0.0f;
		float cursor_y = 0.0f;
		float cursor_z = _beam_start_z();
		D3DXVECTOR3 beam_intersect = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		game_main_fbx_object_handle_t *map = nullptr;
		int polygon_id = -1;
		int mesh_id = -1;
		//�r�[���̍L�͈͏Փ˗\�������邩
		if (option_get_do_aim_assist())
		{
			D3DXVECTOR3 start_position;
			D3DXVECTOR3 beam_screen_pos_start = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&start_position, &beam_screen_pos_start);
			D3DXVECTOR3 end_position;
			cursor_x = 0.0f;
			cursor_y = 0.0f;
			cursor_z = _beam_end_z();
			D3DXVECTOR3 beam_screen_pos_end = D3DXVECTOR3(cursor_x, cursor_y, cursor_z);
			mylib_to_world_pos(&end_position, &beam_screen_pos_end);
			float thickness = 0.0;
			thickness = param->_status._beam_extensive_thickness;
			check_beam_hit_map(
				object,
				&start_position,
				&end_position,
				thickness,
				&map,
				&beam_hitting_extensive_prediction,
				&polygon_id,
				&mesh_id,
				&beam_intersect,
#if _output_debug_info
				nullptr,
				nullptr,
				nullptr,
				nullptr
#else
				nullptr
#endif
			);
		}
		if (beam_hitting_extensive_prediction)
		{
			param->_beam_hitting_extensive_prediction_object_handle = map;
			param->_beam_hitting_extensive_prediction = true;
			param->_beam_hitting_extensive_prediction_polygon_id = polygon_id;
			param->_beam_hitting_extensive_prediction_mesh_id = mesh_id;
		}
		else
		{
			param->_beam_hitting_extensive_prediction_object_handle = nullptr;
			param->_beam_hitting_extensive_prediction = false;
			param->_beam_hitting_extensive_prediction_polygon_id = -1;
			param->_beam_hitting_extensive_prediction_mesh_id = -1;
		}
	}
	//���݁A���͈ڍs��Ԃ�������
	ask_player_state_is_sticking_migration(param->_now_state == psi_sticking_migration);
}


//�S��Ԃɂ����čs���鎖�㏈��(�X�e�[�g���Ƃ̍X�V��ɌĂ΂��)
static void state_common_post_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(brain);
	//�W�����v�����A����n��
	player_is_jumping_set(param->_jump_count > 0);
	//���̎��_�ł�Y�ړ��ʂ�n��
	player_jump_move_y_set(param->_jump_move_y);
	//�Ə��J�[�\���X�V
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(NAN, NAN, NAN);
		game_main_fbx_object_handle_t *map = nullptr;
		int polygon_id = -1;
		if (param->_beam_hitting_extensive_prediction)
		{
			//�L�͈͗\���ɐ������Ă���
			if (param->_beam_hitting_prediction)
			{
				//���ʂ̗\���ł��������Ă���
				map = param->_beam_hitting_prediction_object_handle;
				polygon_id = param->_beam_hitting_prediction_polygon_id;
			}
			else
			{
				//���ʂ̗\���ł͐������Ă��Ȃ�
				map = param->_beam_hitting_extensive_prediction_object_handle;
				polygon_id = param->_beam_hitting_extensive_prediction_polygon_id;
			}
		}
		int min_polygon_id = ((int)(polygon_id) / 12) * 12;
		int max_polygon_id = min_polygon_id + 12;
		D3DXVECTOR3 screen_pos = D3DXVECTOR3(invalid_aiming_cursor_pos(), invalid_aiming_cursor_pos(), invalid_aiming_cursor_pos());
		if (map)
		{
			game_main_fbx_object_t *map_object = to_object(map);
			D3DXMATRIX world;
			gmfoh_t_get_world(map, &world, nullptr);
			fbx_table_get_polygon_position_range(
				map_object->_looks,
				map_object->_now_motion,
				&world,
				map_object->_motion_frame,
				min_polygon_id,
				max_polygon_id,
				&pos
			);
			mylib_to_screen_pos(&screen_pos, &pos);
		}
		//�Ə��J�[�\���ʒu�X�V
		aiming_cursor_pos_update(screen_pos.x, screen_pos.y, screen_pos.z);
#if _output_debug_info
		mylib_register_debug_string("hit polygon ID : %d", polygon_id);
		mylib_register_debug_string("hit min / max polygon ID : (%d, %d)", min_polygon_id, max_polygon_id);
		mylib_register_debug_string("hit mesh ID : %d", param->_beam_hitting_mesh_id);
		mylib_register_debug_string("hit pos (x : %f, y : %f, z : %f)", pos.x, pos.y, pos.z);
		mylib_register_debug_string("hit screen pos (x : %f, y : %f, z : %f)", screen_pos.x, screen_pos.y, screen_pos.z);
#endif
	}
}


//�ʏ��Ԃ̃��[�V�����؂�ւ�����
static void change_motion_normal(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//���s���Ԃ��ł��Z���A�N�V������T��
	time_t shortest_action_frame = 0xffffffff;
	player_action_id_t shortest_action = pai_none;
	for (size_t i = 0; i < pai_max; i++)
	{
		if (param->_doing_frame[i])
		{
			if (param->_doing_frame[i] < shortest_action_frame)
			{
				shortest_action_frame = param->_doing_frame[i];
				shortest_action = (player_action_id_t)(i);
			}
		}
	}
	//���͂�ⓚ���p�̏���
	if (param->_doing[(size_t)(pai_horizontal_blow)])
	{
		shortest_action = pai_horizontal_blow;
	}
	switch (shortest_action)
	{
	case pai_none:
		object->_now_motion = ftmi_player_stand;
		break;
	case pai_run:
		object->_now_motion = ftmi_player_run;
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_jump:
		object->_now_motion = ftmi_player_fall;
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_beam:
		object->_now_motion = ftmi_player_front_walk;
#ifdef _beam_walk
		if (param->_beam_walk_mode_on)
		{
			//�v���[���[�̊p�x�𒲂ׂ�
			float player_angle = camera_manager_get_phi() - brain->_input_angle - (float)(M_PI_2);
			//�r�[���̊p�x�𒲂ׂ�
			D3DXVECTOR3 beam_vector = param->_beam_end_position - param->_beam_start_position;
			float beam_angle = atan2f(beam_vector.z, beam_vector.x);
			//�����Z����
			float sub = player_angle - beam_angle;
			//�f�O���[
			int deg = (int)(D3DXToDegree(sub));
			//45��������
			int deg45 = deg + 45;
			//�����360*3����
			int deg45360 = deg45 + 360 * 3;
			//90�Ŋ���Z���ď�]�����
			int _0123 = (deg45360 / 90) % 4;
			//�A�j���[�V�����`�F���W
			switch (_0123)
			{
			case 0:
				object->_now_motion = ftmi_player_front_walk;
				break;
			case 1:
				object->_now_motion = ftmi_player_side_walk1;
				break;
			case 2:
				object->_now_motion = ftmi_player_back_walk;
				break;
			case 3:
				object->_now_motion = ftmi_player_side_walk2;
				break;
			default:
				break;
			}
		}
#endif
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_horizontal_blow:
		object->_now_motion = ftmi_player_round;
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_floating:
		if (shortest_action_frame == 1)
		{
			object->_motion_frame = 0.0f;
		}
		break;
	case pai_max:
		break;
	default:
		break;
	}
}
//�ʏ��Ԃ̊J�n����
static void state_normal_start(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
}
//�ʏ��Ԃ̍X�V����
static void state_normal_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
#ifdef _beam_walk
	//�r~~���������[�h���肩��
	change_beam_walk_mode(param, brain);
#endif
	//����
	run_process(object, param, brain);
	//�W�����v
	jump_process(object, param, brain);
	//��������т𔽉f������
	horizontal_blow_process(object, param);
	//���V����
	floating_process(object, param, brain);
	//�d�͏���
	gravity_process(object, param);
	//�n�ʂƂ̂����蔻��
	ground_hit_process(object, brain, param);
	//�r~~��������
	beam_process(object, param, brain);
#ifdef _beam_walk
	//�r~~���������Ă���Ԃ̕�������
	beam_walk_process(object, param, brain);
#endif
	//�r~~���ƃ}�b�v�̂����蔻����Ƃ鏀��
	game_main_fbx_object_handle_t *map = nullptr;
	bool beam_hitting = false;
	int polygon_id = -1;
	int mesh_id = -1;
	D3DXVECTOR3 beam_intersect = D3DXVECTOR3(NAN, NAN, NAN);
	//�r~~���ƃ}�b�v�̂����蔻��(�r�[���������Ă���Ԃ̂�)
	if (should_check_beam_hit_map(param))
	{
		//�����蔻������
		check_beam_hit_map(
			object,
			&param->_beam_start_position,
			&param->_beam_end_position,
			param->_status._beam_thickness,
			&map,
			&beam_hitting,
			&polygon_id,
			&mesh_id,
			&beam_intersect,
#if _output_debug_info
			nullptr,
			&param->_fordbg_beam_ray_position,
			&param->_fordbg_beam_ray_vector,
			&param->_fordbg_beam_intersect
#else
			nullptr
#endif
		);
	}
	if (beam_hitting)
	{
		param->_beam_hitting_object_handle = map;
		param->_beam_hitting = true;
		param->_beam_hitting_polygon_id = polygon_id;
		param->_beam_hitting_mesh_id = mesh_id;
	}
	else
	{
		param->_beam_hitting_object_handle = nullptr;
		param->_beam_hitting = false;
		param->_beam_hitting_polygon_id = -1;
		param->_beam_hitting_mesh_id = -1;
	}
	//�r~~���ƃ}�b�v������������
	if (beam_hitting)
	{
		//���̎��_�ł������ڍs��ԂɂȂ�ׂ��ł���
		//�������ڍs�O�̈ʒu���o����
		param->_sticking_position_before = object->_position;
		//�������ڍs��̈ʒu���o����
		param->_sticking_position_after = beam_intersect;
		//�������ڍs��ԂɂȂ�
		param->_next_state = psi_sticking_migration;
		//���Ԃ��L�^
		player_beam_hit_record();
	}
	//�S�[���n�_�ɓ��B�������`�F�b�N
	if (stage_event_get_player_near_goal(stage_data_get_goal_goal_threshold(stage_get_now())))
	{
		//�S�[���������Ƃɂ���
		stage_event_stage_goal();
	}
	//�������畜������
	respawn_if(object, param, is_fell(object));
	//���[�V�����`�F���W
	change_motion_normal(object, param, brain);
	//���݂��邮�钆��
	if (param->_doing[(size_t)(pai_horizontal_blow)])
	{
		//���炫��G�t�F�N�g���o��
		generate_glitter(object, param, 2);
	}
}
//�ʏ��Ԃ̏I������
static void state_normal_end(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
	//�����o��
	sound_play(si_se_maghit);
}



//�������ڍs��Ԃ̊J�n����
static void state_sticking_migration_start(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//�����o��
	sound_play(si_se_magfly);
	sound_play_hit_beam();
	//��������т̊p�x������
	param->_horizontal_blow_angle = object->_rotation.y + (float)(M_PI);
	//�W�����v�񐔂�0�ɂ���
	param->_jump_count = 0;
}

//�������ڍs��Ԃ̍X�V����
static void state_sticking_migration_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//�Ə��J�[�\�����I�����W�ɂ���
	player_beam_hit_object_set(true);
	//�������ڍs���̈ړ�����
	sticking_migration_process(object, param, brain);
#if 0
	//���V�A�N�V���������悤�Ƃ��Ă��邩
	if (brain->_should_floating)
	{
		//�t���O��؂�ւ�
		param->_doing[(size_t)(pai_beam)] = false;
		//�ʏ��Ԃɖ߂�
		param->_next_state = psi_normal;
		return;
	}
#endif //2019/01/30 ���̋@�\�͕s�v�ɂȂ���
	//�e�̈ʒu���v�Z
	calc_shadow_position(object, param);
	//�ڍs��Ԃ��I�������
	if (sticking_migration_ended(object, param))
	{
		//�t���O��؂�ւ�
		param->_doing[(size_t)(pai_beam)] = false;
		//��������~��ԂɂȂ�
		param->_next_state = psi_sticking_stop;
		//�}�b�v������(2019/02/05���̏����͕s�v�ɂȂ���)
		//gmfoh_t_set_ignore_flag(param->_beam_hit_map_object, param->_beam_hitting_mesh_id, true);
		return;
	}
	param->_next_state = psi_sticking_migration;
}

//�������ڍs��Ԃ̏I������
static void state_sticking_migration_end(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
}


//��������~��Ԃ̊J�n����
static void state_sticking_stop_start(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
	//QTE�J�n
	sprite_integration_qte_start();
}

//��������~��Ԃ̍X�V����
static void state_sticking_stop_process(game_main_fbx_object_t *object, player_parameter_t *param, const player_brain_t *brain)
{
	//�Ə��J�[�\�����I�����W�ɂ���
	player_beam_hit_object_set(true);
	//��������~���̏���
	sticking_stop_process(object, param, brain);
	//�e�̈ʒu���v�Z
	calc_shadow_position(object, param);
	//���͔����W�����v������ׂ���
	bool should_magnetic_jump = false;
	should_magnetic_jump |= brain->_should_magnetic_jump;
	should_magnetic_jump |= sprite_integration_qte_timeout();
	//���͔����W�����v������
	if (should_magnetic_jump)
	{
		//QTE������
		if (sprite_integration_qte_judge() == true)
		{
			///QTE�����Ȃ̂ŋ����W�����v������
			//���͔����W�����v������
			do_magnetic_jump(
				object,
				param,
				param->_status._qte_success_magnetic_repulsion_duration_frame,
				param->_status._qte_success_magnetic_repulsion_force_x,
				param->_status._qte_success_magnetic_repulsion_force_y
			);
		}
		else
		{
			///QTE���s�Ȃ̂ŕ��ʂ̃W�����v������
			//���͔����W�����v������
			do_magnetic_jump(
				object,
				param,
				param->_status._magnetic_repulsion_duration_frame,
				param->_status._magnetic_repulsion_force_x,
				param->_status._magnetic_repulsion_force_y
			);
		}
		//�����o��
		sound_play(si_se_magarrive);
		//���ʂ̏�Ԃɖ߂�
		param->_next_state = psi_normal;
		return;
	}
	param->_next_state = psi_sticking_stop;
}

//��������~��Ԃ̏I������
static void state_sticking_stop_end(game_main_fbx_object_t *object, player_parameter_t *param)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(param);
}


//�X�e�[�g���Ƃ̊J�n����������
static void state_start(game_main_fbx_object_t *object, player_parameter_t *param, player_state_id_t state)
{
	switch (state)
	{
	case psi_none:
		break;
	case psi_normal:
		state_normal_start(object, param);
		break;
	case psi_sticking_migration:
		state_sticking_migration_start(object, param);
		break;
	case psi_sticking_stop:
		state_sticking_stop_start(object, param);
		break;
	case psi_max:
		break;
	default:
		break;
	}
}

//�X�e�[�g���Ƃ̍X�V����������
static void state_process(game_main_fbx_object_t *object, player_parameter_t *param)
{
	//���͂��s���w�߂ւƕϊ�
	brain_process(&param->_brain);
	//���ʎ��O����
	state_common_pre_process(object, param, &param->_brain);
	//�X�e�[�g���Ƃ̏���
	switch (param->_now_state)
	{
	case psi_none:
		param->_next_state = psi_normal;
		break;
	case psi_normal:
		state_normal_process(object, param, &param->_brain);
		break;
	case psi_sticking_migration:
		state_sticking_migration_process(object, param, &param->_brain);
		break;
	case psi_sticking_stop:
		state_sticking_stop_process(object, param, &param->_brain);
		break;
	case psi_max:
		param->_next_state = psi_normal;
		break;
	default:
		param->_next_state = psi_normal;
		break;
	}
	//���ʎ��㏈��
	state_common_post_process(object, param, &param->_brain);
	//���[�V�����i�s
	object->_motion_frame += 1.0f;
#if _output_debug_info
	//�f�o�b�O�p���C�\���t���O���Z�b�g
	param->_fordbg_draw_ground_ray = true;
	param->_fordbg_draw_beam_ray = param->_doing[(size_t)(pai_beam)];
#endif
}

//�X�e�[�g���Ƃ̏I������������
static void state_end(game_main_fbx_object_t *object, player_parameter_t *param, player_state_id_t state)
{
	switch (state)
	{
	case psi_none:
		break;
	case psi_normal:
		state_normal_end(object, param);
		break;
	case psi_sticking_migration:
		state_sticking_migration_end(object, param);
		break;
	case psi_sticking_stop:
		state_sticking_stop_end(object, param);
		break;
	case psi_max:
		break;
	default:
		break;
	}
}

//�X�e�[�g��؂�ւ��鏈��
static void state_change(game_main_fbx_object_t *object, player_parameter_t *param)
{
	if (param->_now_state != param->_next_state)
	{
		state_end(object, param, param->_now_state);
		param->_now_state = param->_next_state;
		state_start(object, param, param->_now_state);
	}
}

//�I�u�W�F�N�g���v���C���[�Ƃ��ď�����
extern void gmfo_t_player_init(game_main_fbx_object_t *object)
{
	//�O������ǂݍ���
	const player_database_t *database = get_database();
	//�������m��
	object->_kind_parameter = malloc(sizeof(player_parameter_t));
	if (object->_kind_parameter == nullptr)
	{
		MessageBox(nullptr, "�������m�ۂɎ��s���܂���", "�G���[", MB_OK);
		exit(EXIT_FAILURE);
	}
	ZeroMemory(object->_kind_parameter, sizeof(player_parameter_t));
	//�v���C���[��p�̏����Z�b�g
	player_parameter_t *param = (player_parameter_t*)(object->_kind_parameter);
	param->_status = database->_status;									//�f�[�^�x�[�X����R�s�[
	param->_now_state = psi_none;										//���݂̃X�e�[�g
	param->_next_state = param->_now_state;								//����X�V���Ƀ`�F���W����X�e�[�g
	//�X�e�[�W�̊J�n�ʒu�ɓ���
	stage_data_get_start_point(stage_get_now(), &object->_position);
	//�g��{����ύX
	object->_scale = D3DXVECTOR3(database->_status._scaling, database->_status._scaling, database->_status._scaling);
	//�Ə��J�[�\��������
	aiming_cursor_pos_init();
}

//�I�u�W�F�N�g���v���C���[�Ƃ��ďI������
extern void gmfo_t_player_end(game_main_fbx_object_t *object)
{
	//���������
	_mylib_safe_free(object->_kind_parameter);
}

//�I�u�W�F�N�g���v���C���[�Ƃ��čX�V
extern void gmfo_t_player_update(game_main_fbx_object_t *object)
{
	player_parameter_t *param = (player_parameter_t*)(object->_kind_parameter);
	//�X�e�[�g��؂�ւ���
	state_change(object, param);
	//�X�V�O�̈ʒu���擾
	float object_prev_y = object->_position.y;
	//�X�V
	state_process(object, param);
	//�X�V��̈ʒu���擾
	float object_updated_y = object->_position.y;
	//�v���[���[�͎��񂾂�
	if (object_updated_y < _player_dead_line())
	{
		//���񂾏u�Ԃ�(�X�V�O�Ɏ���ł��Ȃ�������)
		if (object_prev_y > _player_dead_line())
		{
			//�����T�E���h��炷
			sound_play(si_se_voice_miss);
		}
	}
#if _output_debug_info
	mylib_register_debug_string("pl state : %s", state_name(param->_now_state));
	mylib_register_debug_string("pl pos (x : %f, y : %f, z : %f)", object->_position.x, object->_position.y, object->_position.z);
	mylib_register_debug_string("pl jump move y : %f", param->_jump_move_y);
	//
	for (int i = 0; i < pai_max;i++)
	{
		mylib_register_debug_string("%s, %d", action_name(player_action_id_t(i)), param->_doing_frame[player_action_id_t(i)]);
	}
#endif
}

//�I�u�W�F�N�g���v���C���[�Ƃ��ĕ`��
extern void gmfo_t_player_draw(const game_main_fbx_object_t *object)
{
	player_parameter_t *param = (player_parameter_t*)(object->_kind_parameter);
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
	//�v���[���[�̈ʒu
	holder_player_position_set(&object->_position);
	//�A�[���̈ʒu���o����
	holder_player_magnet_hand_position_set(param->_doing[(size_t)(pai_beam)], param->_doing_frame[(size_t)(pai_beam)], &param->_wrench_position);
	//�v���[���̉e�̈ʒu���o����
	holder_player_shadow_position_set(&param->_shadow_position);
	//�v���[���̉e�̖@�����o����
	holder_player_shadow_normal_set(&param->_shadow_normal);
#if _output_debug_info
	//�f�o�b�O�p�`��
	if (param->_fordbg_draw_ground_ray)
	{
		mylib_draw_ray(&param->_fordbg_ray_position, &param->_fordbg_ray_vector, D3DCOLOR_XRGB(0, 255, 0));
		mylib_draw_sphere(&param->_shadow_position, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
	if (param->_fordbg_draw_beam_ray)
	{
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.0f, 0.0f, 0.0f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.1f, 0.0f, 0.0f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.0f, 0.1f, 0.0f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_ray(&D3DXVECTOR3(param->_fordbg_beam_ray_position + D3DXVECTOR3(0.0f, 0.0f, 0.1f)), &param->_fordbg_beam_ray_vector, D3DCOLOR_XRGB(255, 0, 0));
		mylib_draw_sphere(&param->_fordbg_beam_intersect, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
	}
	mylib_draw_ray(&param->_fordbg_ray2_position, &param->_fordbg_ray2_vector, D3DCOLOR_XRGB(0, 255, 0));
	mylib_draw_sphere(&param->_fordbg_ray2_intersect, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
#endif
}
