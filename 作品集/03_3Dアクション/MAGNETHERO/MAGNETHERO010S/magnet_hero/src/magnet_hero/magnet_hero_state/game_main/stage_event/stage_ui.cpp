//親ヘッダー
#include "stage_ui.h"
//その他のヘッダー
#include "stage_event.h"
#include "stage_index_saver.h"
#include "stage_data.h"
#include "..\connecter\info_holder.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"

//グローバル変数
struct global_t
{
	char *_fordbg_test;
	float _now_sec;
	float _now_sec8;
	float _triangular_wave8;
};

static global_t g;

//プレーヤーからゴールまでの距離を表示
static void stage_ui_display_distance_from_player_to_goal(float distance)
{
	//実質デバッグ用の表示だったのでもういらない
	UNREFERENCED_PARAMETER(distance);
}


//ゴールしてから次のステージに移るまでの間呼ばれる
static void stage_ui_display_goal_production(time_t goal_frame, time_t goal_limit_frame)
{
	UNREFERENCED_PARAMETER(goal_frame);
	UNREFERENCED_PARAMETER(goal_limit_frame);
}


//初期化
extern void stage_ui_init()
{

}


//終了
extern void stage_ui_end()
{

}


//更新
extern void stage_ui_update()
{
	g._now_sec = (float)(timeGetTime()) / 1000.0f;
	g._now_sec8 = g._now_sec / 8.0f;
	g._triangular_wave8 = fabsf(fmodf(g._now_sec8, 2.0f) - 1.0f);
}


//描画
extern void stage_ui_draw(time_t goal_frame, time_t goal_limit_frame)
{
	//頂点を用意
	static D3DXVECTOR3 position[4];
	//UV座標を用意
	static D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//ゴールまでの距離を描画
	stage_ui_display_distance_from_player_to_goal(stage_event_get_distance_from_player_to_goal());
	//ゴールしていれば演出を描画
	if (stage_event_get_already_goal())
	{
		//ゴールした瞬間を判定
		if (goal_frame == 1)
		{
			//ここでゴールした瞬間だけ何かできる
			g._fordbg_test = "goal!";
		}
		//ゴール時のみの演出を描画
		stage_ui_display_goal_production(goal_frame, goal_limit_frame);
	}
	//ゴール地点になんか描画
	{
		D3DXVECTOR3 goal_pos;
		stage_data_get_goal_point(stage_get_now(), &goal_pos);
		int i_max = 10;
		for (int i = 0; i < i_max; i++)
		{
			const float scale_base = 100.0f;
			const float scale_range = (float)((i_max - i) * 50);
			const float scale = scale_base + scale_range * g._triangular_wave8;
			const int alpha = i * 4 + 2;
			const D3DXVECTOR2 _scale = D3DXVECTOR2(scale, scale);
			mylib_draw_billboard(
				&goal_pos,
				g._now_sec8,
				&_scale,
				D3DCOLOR_ARGB(alpha, 255, 255, 255),
				&uv[0],
				&uv[1],
				&uv[2],
				&uv[3],
				texture_manager_get(ti_flare07),
				mbm_add
			);
		}
	}
	//集中線を描画する
	if (player_state_is_sticking_migration())
	{
		IDirect3DTexture9 *texture = texture_manager_get(ti_mag_move);
		int count = (int)(g._now_sec * 1000.0f);
		int num = count % 5;
		float u0 = (float)(num) * 0.2f;
		float u1 = (float)(num + 1) * 0.2f;
		//頂点を用意
		position[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		position[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		position[2] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		position[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		//UV座標を用意
		uv[0] = D3DXVECTOR2(u0, 0.0f);
		uv[1] = D3DXVECTOR2(u1, 0.0f);
		uv[2] = D3DXVECTOR2(u0, 1.0f);
		uv[3] = D3DXVECTOR2(u1, 1.0f);
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture,
			mbm_alpha
		);
	}
}
