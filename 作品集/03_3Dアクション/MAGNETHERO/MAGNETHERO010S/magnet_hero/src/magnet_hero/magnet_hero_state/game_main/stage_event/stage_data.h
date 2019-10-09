#pragma once
//作成者　森仁志
//履歴
//2019/02/05...ファイルを作成
//2019/02/05...初期化関数を追加
//2019/02/05...さまざまな取得関数を追加


//DirectX
#include <d3dx9.h>
//インデックス
#include "stage_index.h"

//初期化
extern void stage_data_init();

//ステージNのスタート地点
extern void stage_data_get_start_point(stage_index_t n, D3DXVECTOR3 *position);

//ステージNのスタート時のプレーヤーの向き
extern void stage_data_get_start_player_direction(stage_index_t n, float *direction);

//ステージNスタート時のカメラの向き
extern void stage_data_get_start_camera_angle(stage_index_t n, float *phi, float *theta);

//ステージNでどれだけゴールに近づいたらゴールにするか
extern float stage_data_get_goal_goal_threshold(stage_index_t n);

//ステージNのゴール地点
extern void stage_data_get_goal_point(stage_index_t n, D3DXVECTOR3 *position);
