#pragma once

#include "..\..\..\common_def\public\common_def.h"
#include "..\..\..\angle\public\angle.h"
#include "..\..\..\x_file\public\x_file_type.h"

//�e�X�g�G�L������V�K�ɏo���Ƃ��ɊO������w��ł������
struct test_enemy_initializer_t
{
	vector3_t _position;
	angle_t _direction;
	x_file_type_t _type;
};
