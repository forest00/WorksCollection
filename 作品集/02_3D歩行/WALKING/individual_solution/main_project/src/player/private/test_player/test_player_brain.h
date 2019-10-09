#pragma once

#include "test_player_object.h"
#include "..\..\..\angle\public\angle.h"

//プレイヤーの行動を決定する構造体
struct test_player_brain_t
{
	bool _should_move;			//動くべきか
	double _move_force;			//どれくらい動くべきか
	angle_t _move_direction;		//動くべき方向
	bool _should_jump;			//ジャンプするべきか
};

//入力を調べて、どう行動するか判断する
extern void test_player_brain_t_judge_action(test_player_brain_t *brain, const test_player_object_t *object);

