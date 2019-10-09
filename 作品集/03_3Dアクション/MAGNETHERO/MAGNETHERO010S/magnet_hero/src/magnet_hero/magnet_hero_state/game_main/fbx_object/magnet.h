#pragma once
#include "object_manager.h"


//オブジェクトを磁石あたり判定マップとして初期化
extern void gmfo_t_magnet_init(game_main_fbx_object_t *object);
//オブジェクトを磁石あたり判定マップとして終了処理
extern void gmfo_t_magnet_end(game_main_fbx_object_t *object);
//オブジェクトを磁石あたり判定マップとして更新
extern void gmfo_t_magnet_update(game_main_fbx_object_t *object);
//オブジェクトを磁石あたり判定マップとして描画
extern void gmfo_t_magnet_draw(const game_main_fbx_object_t *object);
