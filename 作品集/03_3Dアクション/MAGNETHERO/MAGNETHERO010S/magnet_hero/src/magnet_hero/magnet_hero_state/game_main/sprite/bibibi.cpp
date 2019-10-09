//親ヘッダー
#include "bibibi.h"
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\..\calc\calc.h"

//びりびりの最大数
#define _bibibi_max_num		(0x1000)

//びりびり構造体
struct bibibi_t
{
	bool _active;
	time_t _life_time;
	time_t _update_count;
	D3DXVECTOR2 _start_position;
	D3DXVECTOR2 _position;
	float _angle;
	float _scale;
	int _alpha;
	float _distance_max;
};




//ファイル内グローバル変数
struct global_t
{
	bibibi_t _bibibi[_bibibi_max_num];
	IDirect3DTexture9 *_bibibi_tex;
};

static global_t g;


//びりびり追加
static void _bibibi_add(float x, float y, float angle)
{
	for (int i = 0; i < _bibibi_max_num;i++)
	{
		bibibi_t *bibibi = &g._bibibi[i];
		if (bibibi->_active == false)
		{
			bibibi->_active = true;
			bibibi->_life_time = (time_t)(uniform_randf(32, 96));
			bibibi->_update_count = 0;
			bibibi->_start_position = D3DXVECTOR2(x, y);
			bibibi->_position = bibibi->_start_position;
			bibibi->_angle = angle;
			bibibi->_scale = uniform_randf(0.05f, 0.2f);
			bibibi->_distance_max = uniform_randf(2.5f, 7.0f) * bibibi->_scale;
			return;
		}
	}
}

//びりびり更新
static void _bibibi_update(bibibi_t *bibibi)
{
	const float t = (float)(bibibi->_update_count) / (float)(bibibi->_life_time);
	const float s = 1.0f - t;
	const D3DXVECTOR2 angle_vec = D3DXVECTOR2(cosf(bibibi->_angle), sinf(bibibi->_angle));
	if (bibibi->_update_count >= bibibi->_life_time)
	{
		bibibi->_active = false;
		return;
	}
	bibibi->_position = bibibi->_start_position + bibibi->_distance_max * angle_vec * (1.0f - (s - 0.1f) * (s - 0.1f));
	bibibi->_alpha = (int)(s * 255.0f);
	bibibi->_update_count++;
}


//初期化
extern void bibibi_init()
{
	for (int i = 0; i < _bibibi_max_num; i++)
	{
		bibibi_t *bibibi = &g._bibibi[i];
		bibibi->_active = false;
	}
	g._bibibi_tex = texture_manager_get(ti_red_thunder);
}

//終了
extern void bibibi_end()
{
	for (int i = 0; i < _bibibi_max_num; i++)
	{
		g._bibibi[i]._active = false;
	}
}

//更新
extern void bibibi_update()
{
	for (int i = 0; i < _bibibi_max_num; i++)
	{
		bibibi_t *bibibi = &g._bibibi[i];
		if (bibibi->_active)
		{
			_bibibi_update(bibibi);
		}
	}
}

//描画
extern void bibibi_draw()
{
	for (int i = 0; i < _bibibi_max_num; i++)
	{
		bibibi_t *bibibi = &g._bibibi[i];
		if (bibibi->_active)
		{
			if (bibibi->_update_count)
			{
				mylib_draw_point_sprite(
					&bibibi->_position,
					bibibi->_angle + (float)(M_PI_4),
					&D3DXVECTOR2(bibibi->_scale, bibibi->_scale),
					D3DCOLOR_ARGB(bibibi->_alpha, 255, 255, 255),
					&D3DXVECTOR2(0.0f, 0.0f),
					&D3DXVECTOR2(1.0f, 0.0f),
					&D3DXVECTOR2(0.0f, 1.0f),
					&D3DXVECTOR2(1.0f, 1.0f),
					g._bibibi_tex,
					mbm_add
					);
			}
		}
	}
}

//メッセージ発行
extern void bibibi_fire(int base, int random_range)
{
	int n = base + rand() % random_range;
	for (int i = 0; i < n; i++)
	{
		_bibibi_add(0.0f, 0.0f, uniform_randf(-(float)(M_PI), (float)(M_PI)));
	}
}

