#pragma once

#include "test_enemy_object.h"


//�e�X�g�p�G�L�����̍s�������肷��\����
struct test_enemy_brain_t
{
	bool _should_move;			//�����ׂ���
	double _move_force;			//�ǂꂭ�炢�����ׂ���
	angle_t _move_direction;	//�����ׂ�����
	bool _should_jump;			//�W�����v����ׂ���
};


//�����̒u����Ă���󋵂𒲂ׂāA�ǂ��s�����邩���f����
extern void test_enemy_brain_t_judge_action(test_enemy_brain_t *brain, const test_enemy_object_t *object);


