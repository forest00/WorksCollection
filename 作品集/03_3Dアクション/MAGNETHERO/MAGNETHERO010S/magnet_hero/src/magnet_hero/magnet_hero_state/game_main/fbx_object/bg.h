#pragma once
#include "object_manager.h"


//オブジェクトをあたり判定なしの動かないマップとして初期化
extern void gmfo_t_stage_bg_init(game_main_fbx_object_t *object);
//オブジェクトをあたり判定なしの動かないマップとして終了処理
extern void gmfo_t_stage_bg_end(game_main_fbx_object_t *object);
//オブジェクトをあたり判定なしの動かないマップとして更新
extern void gmfo_t_stage_bg_update(game_main_fbx_object_t *object);
//オブジェクトをあたり判定なしの動かないマップとして描画
extern void gmfo_t_stage_bg_draw(const game_main_fbx_object_t *object);
