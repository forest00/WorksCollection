

//�e�w�b�_�[
#include "test_enemy_database.h"
//���̑��̃w�b�_�[
#include "..\..\..\math_with_defines\math_with_defines.h"

//�f�t�H���g�̃f�[�^�𓾂�
//todo:�e�L�X�g�t�@�C������ǂݍ���
extern void test_enemy_database_default(test_enemy_database_t *data)
{
	data->_move_speed = 1.0;
	data->_jump_pow = 18.0;
	data->_turning_force = M_PI * 1.5;
	data->_gravity = -30.0;
}

