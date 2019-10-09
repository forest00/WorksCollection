#pragma once

#include "stage_index.h"

//初期化
extern void stage_clear_data_init();

//現在のステージを得る
extern stage_index_t stage_get_now();

//現在のステージをクリアしたときに呼び出す関数
//この関数を呼ぶと現在ステージに対してクリアフラグが立つ
extern void stage_clear_now_stage();

//ステージを進める
//この関数を呼ぶと現在ステージがインクリメントされる
//最終ステージをクリアし、次のステージがない場合、trueを返す(全クリ)
extern bool stage_advance();

//そのステージをクリアしているか
extern bool stage_cleared_stage(stage_index_t index);
