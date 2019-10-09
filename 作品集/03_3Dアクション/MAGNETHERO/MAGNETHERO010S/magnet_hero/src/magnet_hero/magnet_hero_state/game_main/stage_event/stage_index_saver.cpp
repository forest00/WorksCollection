//親ヘッダー
#include "stage_index_saver.h"

#include <d3dx9.h>

//グローバル変数
struct global_t
{
	stage_index_t _now_stage;
	bool _cleared[stage_max];
};

static global_t g;

extern void stage_clear_data_init()
{
	for (int i = stage_1;i < stage_max;i++)
	{
		g._cleared[i] = false;
	}
	g._now_stage = stage_1;
}

extern stage_index_t stage_get_now()
{
	return g._now_stage;
}

extern void stage_clear_now_stage()
{
	g._cleared[g._now_stage] = true;
}

extern bool stage_advance()
{
	g._now_stage = (stage_index_t)((int)(g._now_stage) + 1);
	return g._now_stage == stage_max;
}

extern bool stage_cleared_stage(stage_index_t index)
{
	return g._cleared[index];
}
