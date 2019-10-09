//親ヘッダー
#include "stage_event.h"
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "stage_data.h"
#include "stage_index_saver.h"
#include "..\connecter\info_holder.h"

//グローバル変数
struct global_t
{
	bool _already_goal;
	time_t _stage_start_moment;
	time_t _stage_goal_moment;
	float _distance_from_player_to_goal;
};

static global_t g;


//初期化
extern void stage_event_init()
{

}

//終了
extern void stage_event_end()
{

}

//更新
extern void stage_event_update()
{
	//プレーヤーの位置
	D3DXVECTOR3 player_position;
	holder_player_position_get(&player_position);
	//ゴール地点
	D3DXVECTOR3 goal_pos;
	stage_data_get_goal_point(stage_get_now(), &goal_pos);
	//現在位置からゴール地点までの距離
	D3DXVECTOR3 sub = player_position - goal_pos;
	g._distance_from_player_to_goal = D3DXVec3Length(&sub);
}

//プレーヤーからゴールまでの距離
extern float stage_event_get_distance_from_player_to_goal()
{
	return g._distance_from_player_to_goal;
}

//プレーヤーがゴール地点の近くにいるときにtrueを返す
extern bool stage_event_get_player_near_goal(float threshold)
{
	return g._distance_from_player_to_goal < threshold;
}

//ステージを開始する瞬間に呼ばれる関数
extern void stage_event_stage_start()
{
	g._already_goal = false;
	g._stage_start_moment = mylib_get_frame_count();
}

//プレーヤーがゴール地点に達したときに呼ばれる関数
extern void stage_event_stage_goal()
{
	if (g._already_goal == false)
	{
		g._already_goal = true;
		g._stage_goal_moment = mylib_get_frame_count();
	}
}

//すでにゴールしている
extern bool stage_event_get_already_goal()
{
	return g._already_goal;
}

//開始した瞬間
extern time_t stage_event_get_start_moment()
{
	return g._stage_start_moment;
}

//ゴールした瞬間
extern time_t stage_event_get_goal_moment()
{
	return g._stage_goal_moment;
}

//ゴールしてから経過した時間を取得
extern time_t stage_event_get_goal_elapsed()
{
	return mylib_get_frame_count() - g._stage_goal_moment;
}
