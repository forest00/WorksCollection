//�e�w�b�_�[
#include "stage_event.h"
//���̑��̃w�b�_�[
#include "..\..\..\..\mylib\mylib.h"
#include "stage_data.h"
#include "stage_index_saver.h"
#include "..\connecter\info_holder.h"

//�O���[�o���ϐ�
struct global_t
{
	bool _already_goal;
	time_t _stage_start_moment;
	time_t _stage_goal_moment;
	float _distance_from_player_to_goal;
};

static global_t g;


//������
extern void stage_event_init()
{

}

//�I��
extern void stage_event_end()
{

}

//�X�V
extern void stage_event_update()
{
	//�v���[���[�̈ʒu
	D3DXVECTOR3 player_position;
	holder_player_position_get(&player_position);
	//�S�[���n�_
	D3DXVECTOR3 goal_pos;
	stage_data_get_goal_point(stage_get_now(), &goal_pos);
	//���݈ʒu����S�[���n�_�܂ł̋���
	D3DXVECTOR3 sub = player_position - goal_pos;
	g._distance_from_player_to_goal = D3DXVec3Length(&sub);
}

//�v���[���[����S�[���܂ł̋���
extern float stage_event_get_distance_from_player_to_goal()
{
	return g._distance_from_player_to_goal;
}

//�v���[���[���S�[���n�_�̋߂��ɂ���Ƃ���true��Ԃ�
extern bool stage_event_get_player_near_goal(float threshold)
{
	return g._distance_from_player_to_goal < threshold;
}

//�X�e�[�W���J�n����u�ԂɌĂ΂��֐�
extern void stage_event_stage_start()
{
	g._already_goal = false;
	g._stage_start_moment = mylib_get_frame_count();
}

//�v���[���[���S�[���n�_�ɒB�����Ƃ��ɌĂ΂��֐�
extern void stage_event_stage_goal()
{
	if (g._already_goal == false)
	{
		g._already_goal = true;
		g._stage_goal_moment = mylib_get_frame_count();
	}
}

//���łɃS�[�����Ă���
extern bool stage_event_get_already_goal()
{
	return g._already_goal;
}

//�J�n�����u��
extern time_t stage_event_get_start_moment()
{
	return g._stage_start_moment;
}

//�S�[�������u��
extern time_t stage_event_get_goal_moment()
{
	return g._stage_goal_moment;
}

//�S�[�����Ă���o�߂������Ԃ��擾
extern time_t stage_event_get_goal_elapsed()
{
	return mylib_get_frame_count() - g._stage_goal_moment;
}
