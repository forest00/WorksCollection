#pragma once

//�e�X�g�p�G�L�����̊�{���
struct test_enemy_database_t
{
	double _move_speed;
	double _jump_pow;
	double _turning_force;
	double _gravity;
};

//�f�t�H���g�̃f�[�^�𓾂�
//todo:�e�L�X�g�t�@�C������ǂݍ���
extern void test_enemy_database_default(test_enemy_database_t *data);
