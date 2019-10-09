#pragma once


#include "..\..\..\common_def\public\common_def.h"
#include "..\..\..\x_file\public\x_file_type.h"


//外部から操作可能なプレイヤー初期情報
struct test_player_initializer_t
{
	vector3_t _position;			//位置
	x_file_type_t _type;		//見た目
};
