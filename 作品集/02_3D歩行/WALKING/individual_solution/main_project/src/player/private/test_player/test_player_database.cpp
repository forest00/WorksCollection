
//親ヘッダー
#include "test_player_database.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "..\..\..\my_3d_lib\public\my_3d_lib.h"

//todo:テキストファイルから読み込むようにする
extern void test_player_database_t_default(test_player_database_t *data)
{
	data->_move_speed = 0.1;
	data->_jump_pow = 0.5;
	data->_turning_force = (double)(0.1 * M_PI);
	data->_gravity = -0.02;
}
