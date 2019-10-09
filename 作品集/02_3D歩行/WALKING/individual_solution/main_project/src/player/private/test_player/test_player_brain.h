#pragma once

#include "test_player_object.h"
#include "..\..\..\angle\public\angle.h"

//�v���C���[�̍s�������肷��\����
struct test_player_brain_t
{
	bool _should_move;			//�����ׂ���
	double _move_force;			//�ǂꂭ�炢�����ׂ���
	angle_t _move_direction;		//�����ׂ�����
	bool _should_jump;			//�W�����v����ׂ���
};

//���͂𒲂ׂāA�ǂ��s�����邩���f����
extern void test_player_brain_t_judge_action(test_player_brain_t *brain, const test_player_object_t *object);

