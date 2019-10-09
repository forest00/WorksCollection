#pragma once

#include "..\..\public\test_enemy\test_enemy_initializer.h"
#include "..\..\public\test_enemy\test_enemy_object_declaration.h"
#include "..\..\..\collision\public\collision.h"


struct test_enemy_manager_t;

/*---------------------------------------*/
//テスト用敵キャラマネージャーを確保して初期化して返す
extern test_enemy_manager_t *test_enemy_manager_t_init();

//テスト用敵キャラマネージャーの更新
//管理下にある全敵キャラに対して、更新処理をする
extern void test_enemy_manager_t_update(test_enemy_manager_t *manager);

//テスト用敵キャラマネージャーが管理しているテスト用敵キャラをすべて描画
extern void test_enemy_manager_t_draw(const test_enemy_manager_t *manager, IDirect3DDevice9 *device);

//テスト用敵キャラマネージャーの終了処理 & メモリ解放処理
//引数として渡したポインタにもnullが代入されます
extern void test_enemy_manager_t_end(test_enemy_manager_t **manager);

//テスト用敵キャラマネージャーが管理しているテスト用敵キャラの数を返す
//敵キャラを一体追加するたびに、増えていく
extern size_t test_enemy_manager_t_get_num_of_active(const test_enemy_manager_t *manager);

//テスト用敵キャラマネージャーが管理出来るテスト用敵キャラの最大数を返す
extern size_t test_enemy_manager_t_get_capacity(const test_enemy_manager_t *manager);

//テスト用敵キャラを追加する
//追加したテスト用敵キャラへのポインタを返す
//何らかの原因により失敗したら、nullを返す
extern test_enemy_object_t **test_enemy_manager_t_add(test_enemy_manager_t *manager, const test_enemy_initializer_t *initializer);

//テスト用敵キャラを削除する
//これを呼び出した後、ポインタにnullが代入されます
extern void test_enemy_manager_t_delete(test_enemy_manager_t *manager, test_enemy_object_t **object);

//テスト用敵キャラマネージャーが管理しているオブジェクトの中で、最も取得しやすいオブジェクトへのポインタを返す
extern test_enemy_object_t **test_enemy_manager_t_search_active(test_enemy_manager_t *manager);

//テスト用敵キャラマネージャーが管理しているオブジェクトの中で、AABBと衝突しているオブジェクトへのポインタを返す
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_aabb(test_enemy_manager_t *manager, const aabb_t *aabb);

//テスト用敵キャラマネージャーが管理しているオブジェクトの中で、球体と衝突しているオブジェクトへのポインタを返す
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_sphere(test_enemy_manager_t *manager, const sphere_t *sphere);

//テスト用敵キャラの位置を得る
extern const vector3_t *test_enemy_manager_t_get_position(const test_enemy_manager_t *manager, const test_enemy_object_t *object);

//テスト用敵キャラの角度を得る
extern const angle_t *test_enemy_manager_t_get_direction(const test_enemy_manager_t *manager, const test_enemy_object_t *object);
/*---------------------------------------*/




