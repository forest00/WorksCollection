#pragma once
#include "object_manager.h"

//オブジェクトをプレイヤーとして初期化
extern void gmfo_t_player_init(game_main_fbx_object_t *object);

//オブジェクトをプレイヤーとして終了処理
extern void gmfo_t_player_end(game_main_fbx_object_t *object);

//オブジェクトをプレイヤーとして更新
extern void gmfo_t_player_update(game_main_fbx_object_t *object);

//オブジェクトをプレイヤーとして描画
extern void gmfo_t_player_draw(const game_main_fbx_object_t *object);
