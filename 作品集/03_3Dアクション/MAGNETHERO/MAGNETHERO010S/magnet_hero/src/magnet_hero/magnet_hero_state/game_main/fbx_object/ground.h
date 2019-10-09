#pragma once
#include "object_manager.h"


//オブジェクトをあたり判定付きマップとして初期化
extern void gmfo_t_ground_init(game_main_fbx_object_t *object);
//オブジェクトをあたり判定付きマップとして終了処理
extern void gmfo_t_ground_end(game_main_fbx_object_t *object);
//オブジェクトをあたり判定付きマップとして更新
extern void gmfo_t_ground_update(game_main_fbx_object_t *object);
//オブジェクトをあたり判定付きマップとして描画
extern void gmfo_t_ground_draw(const game_main_fbx_object_t *object);
