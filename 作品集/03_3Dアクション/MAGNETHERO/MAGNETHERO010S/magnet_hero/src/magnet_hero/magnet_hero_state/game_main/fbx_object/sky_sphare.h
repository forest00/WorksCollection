#pragma once
#include "object_manager.h"

//オブジェクトをスカイスフィアとして初期化
extern void gmfo_t_sky_sphere_init(game_main_fbx_object_t *object);

//オブジェクトをスカイスフィアとして終了処理
extern void gmfo_t_sky_sphere_end(game_main_fbx_object_t *object);

//オブジェクトをスカイスフィアとして更新
extern void gmfo_t_sky_sphere_update(game_main_fbx_object_t *object);

//オブジェクトをスカイスフィアとして描画
extern void gmfo_t_sky_sphere_draw(const game_main_fbx_object_t *object);
