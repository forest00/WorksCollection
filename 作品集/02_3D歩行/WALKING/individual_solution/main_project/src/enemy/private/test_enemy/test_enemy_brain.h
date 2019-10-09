#pragma once

#include "test_enemy_object.h"


//テスト用敵キャラの行動を決定する構造体
struct test_enemy_brain_t
{
	bool _should_move;			//動くべきか
	double _move_force;			//どれくらい動くべきか
	angle_t _move_direction;	//動くべき方向
	bool _should_jump;			//ジャンプするべきか
};


//自分の置かれている状況を調べて、どう行動するか判断する
extern void test_enemy_brain_t_judge_action(test_enemy_brain_t *brain, const test_enemy_object_t *object);


