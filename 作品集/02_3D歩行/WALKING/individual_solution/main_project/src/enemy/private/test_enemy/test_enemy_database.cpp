

//親ヘッダー
#include "test_enemy_database.h"
//その他のヘッダー
#include "..\..\..\math_with_defines\math_with_defines.h"

//デフォルトのデータを得る
//todo:テキストファイルから読み込む
extern void test_enemy_database_default(test_enemy_database_t *data)
{
	data->_move_speed = 1.0;
	data->_jump_pow = 18.0;
	data->_turning_force = M_PI * 1.5;
	data->_gravity = -30.0;
}

