#pragma once

//プレイヤーの基本情報
struct test_player_database_t
{
	double _move_speed;
	double _jump_pow;
	double _turning_force;
	double _gravity;
};

extern void test_player_database_t_default(test_player_database_t *data);
