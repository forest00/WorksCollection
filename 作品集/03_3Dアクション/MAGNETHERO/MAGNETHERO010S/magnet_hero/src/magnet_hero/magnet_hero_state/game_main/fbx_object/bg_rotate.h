#pragma once
#include "object_manager.h"


//オブジェクトをあたり判定なしの回転し続けるマップとして初期化
extern void gmfo_t_stage_bg_rotate_init(game_main_fbx_object_t *object);
//オブジェクトをあたり判定なしの回転し続けるマップとして終了処理
extern void gmfo_t_stage_bg_rotate_end(game_main_fbx_object_t *object);
//オブジェクトをあたり判定なしの回転し続けるマップとして更新
extern void gmfo_t_stage_bg_rotate_update(game_main_fbx_object_t *object);
//オブジェクトをあたり判定なしの回転し続けるマップとして描画
extern void gmfo_t_stage_bg_rotate_draw(const game_main_fbx_object_t *object);
