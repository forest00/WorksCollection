#pragma once

//テスト用敵キャラの基本情報
struct test_enemy_database_t
{
	double _move_speed;
	double _jump_pow;
	double _turning_force;
	double _gravity;
};

//デフォルトのデータを得る
//todo:テキストファイルから読み込む
extern void test_enemy_database_default(test_enemy_database_t *data);
