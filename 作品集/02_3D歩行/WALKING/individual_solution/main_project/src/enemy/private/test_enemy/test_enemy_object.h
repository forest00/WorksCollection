#pragma once

#include "..\..\..\angle\public\angle.h"
#include "..\..\public\test_enemy\test_enemy_initializer.h"
#include "..\..\public\test_enemy\test_enemy_object_declaration.h"
#include "test_enemy_database.h"

//テスト用敵キャラ一体分のメモリ確保と初期化をしてポインタを返す
//失敗したらnullを返す
extern test_enemy_object_t *test_enemy_object_t_init(const test_enemy_database_t *database, const test_enemy_initializer_t *initializer);

//テスト用敵キャラ一体分の更新
extern void test_enemy_object_t_update(test_enemy_object_t *object);

//テスト用敵キャラ一体分の描画
extern void test_enemy_object_t_draw(test_enemy_object_t *object, IDirect3DDevice9 *device);

//テスト用敵キャラ一体分の終了とメモリ解放
//引数として渡したポインタにもnullが代入されます
extern void test_enemy_object_t_end(test_enemy_object_t **object);

//テスト用敵キャラの位置を得る
extern const vector3_t *test_enemy_object_t_get_position(const test_enemy_object_t *object);

//テスト用敵キャラの角度を得る
extern const angle_t *test_enemy_object_t_get_direction(const test_enemy_object_t *object);
