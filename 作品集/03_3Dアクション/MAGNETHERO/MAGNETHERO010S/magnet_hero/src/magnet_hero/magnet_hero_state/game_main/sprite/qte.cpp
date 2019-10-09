//親ヘッダー
#include "qte.h"
#ifndef qte_lost
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\file_data\file_data.h"
#include "..\..\..\..\texture_manager\texture_manager.h"

//グローバル変数
struct global_t
{
	DWORD _judge_all_time;
	DWORD _judge_success_start;
	DWORD _judge_success_end;
	DWORD _start_moment;
	DWORD _count_time;
	DWORD _do_draw;
	float _out_scale;
};

static global_t g;

bool between(DWORD x, DWORD min, DWORD max)
{
	return min <= x && x < max;
}

extern void qte_init()
{
	const datum_t *data = file_data_get(fdi_qte_config, nullptr);
	g._judge_all_time = (data++)->_Int;
	g._judge_success_start = (data++)->_Int;
	g._judge_success_end = (data++)->_Int;
	g._start_moment = 0;
	g._count_time = 0;
}

extern void qte_end()
{

}

extern void qte_update()
{
	DWORD now = g._count_time - g._start_moment;
	if (now < g._judge_success_start)
	{
		float t = (float)(now) / (float)(g._judge_success_start);
		float s = 1.0f - t;
		g._out_scale = 1.0f + s * 3.0f;
	}
	else if (now < g._judge_success_end)
	{
		float t = (float)(now - g._judge_success_start) / (float)(g._judge_success_end - g._judge_success_start);
		float s = 1.0f - t;
		g._out_scale = 1.0f;
	}
	else if (now < g._judge_all_time)
	{
		float t = (float)(now - g._judge_success_end) / (float)(g._judge_all_time - g._judge_success_end);
		float s = 1.0f - t;
		g._out_scale = s;
	}
	g._do_draw = between(now, 0, g._judge_all_time);
	g._count_time++;
}

extern void qte_draw()
{
	if (g._do_draw)
	{
		mylib_draw_point_sprite(
			&D3DXVECTOR2(0.0f, 0.0f),
			0.0f,
			&D3DXVECTOR2(1.0f, 1.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR2(0.0f, 0.0f),
			&D3DXVECTOR2(1.0f, 0.0f),
			&D3DXVECTOR2(0.0f, 1.0f),
			&D3DXVECTOR2(1.0f, 1.0f),
			texture_manager_get(ti_png_qte_in),
			mbm_alpha
		);
		mylib_draw_point_sprite(
			&D3DXVECTOR2(0.0f, 0.0f),
			0.0f,
			&D3DXVECTOR2(g._out_scale, g._out_scale),
			D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR2(0.0f, 0.0f),
			&D3DXVECTOR2(1.0f, 0.0f),
			&D3DXVECTOR2(0.0f, 1.0f),
			&D3DXVECTOR2(1.0f, 1.0f),
			texture_manager_get(ti_png_qte_out),
			mbm_alpha
		);
	}
}

extern void qte_start()
{

	g._start_moment = g._count_time;
}

extern bool qte_judge()
{
	DWORD now = g._count_time - g._start_moment;
	if (between(now, g._judge_success_start, g._judge_success_end))
	{
		return true;
	}
	return false;
}

extern bool qte_timeout()
{
	DWORD now = g._count_time - g._start_moment;
	return now > g._judge_all_time;
}
#endif//qte_lost