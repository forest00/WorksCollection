#pragma once
//
//ステージプレイ中に起こる様々なイベントを管理する場所です
//内部で行う計算の結果はいつでも取得できます
//
//作成者　森仁志
//履歴
//2019/02/05...ファイルを作成
//2019/02/05...初期化、更新、終了関数を追加
//2019/02/05...プレーヤーからゴールまでの距離を取得する関数を追加
//2019/02/05...プレーヤーがゴールに到達したことを判定する関数を追加
//2019/02/05...ステージ開始時の時間を記録する関数を追加
//2019/02/05...ゴールの判定ができる関数を追加
//2019/02/05...更新した回数を取得する関数を追加
//2019/02/05...ゴールしてから経過した時間を取得できる関数を追加

//DirectX
#include <d3dx9.h>

//初期化
extern void stage_event_init();

//終了
extern void stage_event_end();

//更新
extern void stage_event_update();

//プレーヤーからゴールまでの距離
extern float stage_event_get_distance_from_player_to_goal();

//プレーヤーがゴール地点の近くにいるときにtrueを返す
extern bool stage_event_get_player_near_goal(float threshold);

//ステージを開始する瞬間に呼ばれる関数
extern void stage_event_stage_start();

//プレーヤーがゴール地点に達したときに呼ばれる関数
extern void stage_event_stage_goal();

//すでにゴールしている
extern bool stage_event_get_already_goal();

//開始した瞬間
extern time_t stage_event_get_start_moment();

//ゴールした瞬間
extern time_t stage_event_get_goal_moment();

//ゴールしてから経過した時間を取得
extern time_t stage_event_get_goal_elapsed();
