#pragma once

#include "..\..\..\common_def\public\common_def.h"
#include "..\..\..\angle\public\angle.h"
#include "..\..\..\x_file\public\x_file_type.h"

//テスト敵キャラを新規に出すときに外部から指定できるもの
struct test_enemy_initializer_t
{
	vector3_t _position;
	angle_t _direction;
	x_file_type_t _type;
};
