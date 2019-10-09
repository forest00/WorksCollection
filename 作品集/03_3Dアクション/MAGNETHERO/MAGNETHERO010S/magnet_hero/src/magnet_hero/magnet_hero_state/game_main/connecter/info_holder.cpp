//�e�w�b�_�[
#include "info_holder.h"
//���̑��̃w�b�_�[
//#include "info_holder.h"

struct global_t
{
	D3DXVECTOR3 _player_position;
	bool _player_magnet_hand_do_draw;
	time_t _player_magnet_hand_frame;
	D3DXVECTOR3 _player_magnet_hand_position;
	D3DXVECTOR3 _player_shadow_position;
	D3DXVECTOR3 _player_shadow_normal;
	DWORD _player_falled_time;
	DWORD _player_goal_time;
	float _distance_from_player_to_goal;
	DWORD _stage_start_moment;
	DWORD _player_beam_hit_moment;
	bool _player_beam_hit_object;
	bool _player_state_is_sticking_migration;
	bool _player_is_jumping;
	float _player_jump_move_y;
	float _player_do_jump_moment;
};

static global_t g = {};

//�v���[���[�̈ʒu��ۑ�
extern void holder_player_position_set(const D3DXVECTOR3 *position)
{
	g._player_position = *position;
}
//�v���[���[�̈ʒu���擾
extern void holder_player_position_get(D3DXVECTOR3 *position)
{
	*position = g._player_position;
}

//�v���[���[�̔�΂���̈ʒu��ۑ�
extern void holder_player_magnet_hand_position_set(bool do_draw, time_t frame, const D3DXVECTOR3 *position)
{
	g._player_magnet_hand_do_draw = do_draw;
	g._player_magnet_hand_frame = frame;
	g._player_magnet_hand_position = *position;
}
//�v���[���[�̔�΂���̈ʒu���擾
extern void holder_player_magnet_hand_position_get(bool *do_draw, time_t *frame, D3DXVECTOR3 *position)
{
	*do_draw = g._player_magnet_hand_do_draw;
	*frame = g._player_magnet_hand_frame;
	*position = g._player_magnet_hand_position;
}

//�v���[���[�̉e�̈ʒu��ۑ�
extern void holder_player_shadow_position_set(const D3DXVECTOR3 *position)
{
	g._player_shadow_position = *position;
}
//�v���[���[�̉e�̈ʒu���擾
extern void holder_player_shadow_position_get(D3DXVECTOR3 *position)
{
	*position = g._player_shadow_position;
}

//�v���[���[�̉e�̖@����ۑ�
extern void holder_player_shadow_normal_set(const D3DXVECTOR3 *normal)
{
	g._player_shadow_normal = *normal;
}
//�v���[���[�̉e�̖@�����擾
extern void holder_player_shadow_normal_get(D3DXVECTOR3 *normal)
{
	*normal = g._player_shadow_normal;
}

//�v���[���[�̐L�΂��r�[���ƃI�u�W�F�N�g�������������Ԃ�ۑ�
extern void player_beam_hit_record()
{
	g._player_beam_hit_moment = timeGetTime();
}
//�v���[���[�̐L�΂��r�[���ƃI�u�W�F�N�g�������������Ԃ��擾
extern DWORD player_beam_hit_get()
{
	return g._player_beam_hit_moment;
}

//�v���[���[�������������Ԃ�ۑ�
extern void holder_falled_player_record()
{
	g._player_falled_time = timeGetTime();
}
//�v���[���[�������������Ԃ��擾
extern DWORD holder_falled_player_get()
{
	return g._player_falled_time;
}


//�v���[���[���r�[����ł����瓖���邩�ǂ�������ς݂̒l��n��
extern void player_beam_hit_object_set(bool flag)
{
	g._player_beam_hit_object = flag;
}
//�v���[���[���r�[����ł����瓖���邩�ǂ�������ς݂̒l��Ԃ�
extern bool player_beam_hit_object_get()
{
	return g._player_beam_hit_object;
}

//���̊֐��͖��t���[���Ăяo���A�v���[���[�����͈ڍs��ԂȂ�true��n���A�����łȂ����false��n���Ȃ���΂����Ȃ�
extern void ask_player_state_is_sticking_migration(bool is)
{
	g._player_state_is_sticking_migration = is;
}
//�v���[���[�����͈ڍs��ԂȂ�true��Ԃ�
extern bool player_state_is_sticking_migration()
{
	return g._player_state_is_sticking_migration;
}


//�v���[���[���W�����v���Ȃ�true�ŌĂԁA�����łȂ����false�ŌĂ�
extern void player_is_jumping_set(bool is)
{
	g._player_is_jumping = is;
}
//�v���[���[���W�����v���Ȃ�true���Ԃ�
extern bool player_is_jumping_get()
{
	return g._player_is_jumping;
}


//�v���[���[Y���W�ړ��ʂ𖈃t���[���n��
extern void player_jump_move_y_set(float y)
{
	g._player_jump_move_y = y;
}
//�v���[���[Y���W�ړ���
extern float player_jump_move_y_get()
{
	return g._player_jump_move_y;
}


//�v���[���[���W�����v�����u�ԂɌĂ�(���Ԃ�b�P�ʂœn��)
extern void player_do_jump_moment_set(float moment)
{
	g._player_do_jump_moment = moment;
}
//�v���[���[���W�����v�����u�Ԃ̎��Ԃ�b�P�ʂŕԂ�
extern float player_do_jump_moment_get()
{
	return g._player_do_jump_moment;
}
