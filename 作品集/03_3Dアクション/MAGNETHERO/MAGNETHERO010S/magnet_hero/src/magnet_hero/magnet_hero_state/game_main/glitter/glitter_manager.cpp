//親ヘッダー
#include "glitter_manager.h"
//その他のヘッダー
#include "glitter.h"

//キャパ
#define _manage_capacity		(0x200)

//管理用構造体
struct manage_t
{
	bool _is_active;
	glitter_t *_object;
};

//グローバル変数
struct global_t
{
	manage_t *_manage;
};

static global_t g;

//種別->テクスチャID変換
static texture_id_t to_texture_id(glitter_kind_t kind)
{
	switch (kind)
	{
	case gk_none:
		return ti_none;
		break;
	case gk_orange:
		return ti_flare01;
		break;
	case gk_purple:
		return ti_flare07;
		break;
	case gk_ring:
		return ti_flare08;
		break;
	case gk_green:
		return ti_flare09;
		break;
	case gk_blue:
		return ti_flare10;
		break;
	case gk_white:
		return ti_flare12;
		break;
	case gk_red:
		return ti_flare13;
		break;
	case gk_max:
		return ti_none;
		break;
	default:
		return ti_none;
		break;
	}
}

//確保
static void manage_alloc(manage_t **p, size_t n)
{
	*p = (manage_t*)malloc(sizeof(manage_t) * n);
}

//ゼロ埋め
static void fill_inactive(manage_t *p, size_t n)
{
#if 0
	for (size_t i = 0; i < n; i++)
	{
		manage_t *manage = &p[i];
		manage->_is_active = false;
		manage->_object = nullptr;
	}
#else
	ZeroMemory(p, sizeof(*p) * n);
#endif
}

//ゼロ探し
static void find_inactive(manage_t **found, global_t *global)
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &global->_manage[i];
		if (manage->_is_active == false)
		{
			*found = manage;
			return;
		}
	}
	*found = nullptr;
}

extern void glitter_manager_init()
{
	//メモリ確保
	manage_alloc(&g._manage, _manage_capacity);
	//ゼロ埋め
	fill_inactive(g._manage, _manage_capacity);
}

extern void glitter_manager_end()
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &g._manage[i];
		//生存チェック
		if (manage->_is_active)
		{
			//終了処理
			glitter_t_end_with_free(&manage->_object);
		}
	}
	//メモリ解放
	free(g._manage);
	g._manage = nullptr;
}

extern void glitter_manager_update()
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &g._manage[i];
		//生存チェック
		if (manage->_is_active)
		{
			bool should_end = false;
			//更新
			glitter_t_update(manage->_object, &should_end);
			if (should_end)
			{
				//フラグ切り替え
				manage->_is_active = false;
				//終了してメモリ解放
				glitter_t_end_with_free(&manage->_object);
			}
		}
	}
}

extern void glitter_manager_draw()
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &g._manage[i];
		//生存チェック
		if (manage->_is_active)
		{
			//描画
			glitter_t_draw(manage->_object);
		}
	}
}

extern void glitter_manager_add(const glitter_add_t *info)
{
	manage_t *manage;
	//空きさがし
	find_inactive(&manage, &g);
	//見つからない
	if (manage == nullptr)
	{
		return;
	}
	//フラグ切り替え
	manage->_is_active = true;
#if 0
	glitter_initializer_t initialzer;
	initialzer._life_sec = info->_life_sec;						//寿命(秒)
	initialzer._scale_base = info->_scale_base;					//拡大率
	initialzer._easing_type = info->_easing_type;				//イージングタイプ
	initialzer._texture_id = to_texture_id(info->_kind);		//テクスチャID
	initialzer._start_position = info->_start_position;			//開始位置
	initialzer._end_position = info->_end_position;				//終了位置
	initialzer._red = info->_red;								//赤成分
	initialzer._green = info->_green;							//緑成分
	initialzer._blue = info->_blue;								//青成分
#else
	const glitter_initializer_t initialzer = {
		info->_life_sec,				//寿命(秒)
		info->_scale_base,				//拡大率
		info->_easing_type,				//イージングタイプ
		to_texture_id(info->_kind),		//テクスチャID
		info->_start_position,			//開始位置
		info->_end_position,			//終了位置
		info->_red,						//赤成分
		info->_green,					//緑成分
		info->_blue						//青成分
	};
#endif
	//メモリ確保&初期化
	glitter_t_init_with_alloc(&manage->_object, &initialzer);
}

extern glitter_kind_t gk_random()
{
	return (glitter_kind_t)(rand() % (int)(gk_max - 1) + 1);
}
