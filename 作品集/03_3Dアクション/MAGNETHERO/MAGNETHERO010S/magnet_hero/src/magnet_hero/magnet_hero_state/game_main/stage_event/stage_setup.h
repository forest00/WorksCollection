#pragma once
//ステージのセットアップをする
//プレーヤーとかの初期化もここで行います
//作成者　森仁志

//ステージのインデックス
#include "stage_index.h"
//fbxオブジェクト管理用ヘッダー
#include "..\fbx_object\object_manager.h"

//N番目のステージのセットアップを行う関数です
//初期化済みのカメラと、初期化済みのFBXオブジェクト配列管理用構造体が必要です
extern void stage_setup(stage_index_t n, game_main_fbx_object_array_t *obj_array);

//N番目のステージをリセットする関数です
extern void stage_reset(stage_index_t n);

//N番目のステージを破棄する関数です
extern void stage_destroy(stage_index_t n, game_main_fbx_object_array_t *obj_array);
