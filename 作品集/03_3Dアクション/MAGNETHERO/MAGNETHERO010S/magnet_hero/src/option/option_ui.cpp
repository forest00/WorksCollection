//親ヘッダー
#include "option_ui.h"
//その他のヘッダー
#include <assert.h>
#include "..\mylib\mylib.h"
#include "..\texture_manager\texture_manager.h"
#include "..\easing\easing.h"
#include "..\easing\easing_type.h"

#define _aim_duration_sec		(1.2f)
#define _aim_pos_x1				(0.5f)
#define _aim_pos_y1				(0.3f)
#define _aim_pos_x2				(0.5f)
#define _aim_pos_y2				(-0.3f)
#define _aim_scale_min			(0.15f)
#define _aim_scale_max			(0.35f)
#define _mouse_duration_sec		(1.2f)
#define _mouse_frame_left		(-0.75f)
#define _mouse_frame_top		(0.75f)
#define _mouse_frame_right		(-0.25f)
#define _mouse_frame_bottom		(-0.75f)
#define _mouse_frame_middle_x	((_mouse_frame_left + _mouse_frame_right) / 2.0f)
#define _mouse_char_left		(_mouse_frame_left + 0.05f)
#define _mouse_char_top			(_mouse_frame_top - 0.15f)
#define _mouse_char_right		(_mouse_frame_right - 0.05f)
#define _mouse_char_bottom		(_mouse_frame_top - 0.25f)
#define _mouse_gauge_left		(_mouse_frame_middle_x - 0.1f)
#define _mouse_gauge_top		(_mouse_char_bottom - 0.1f)
#define _mouse_gauge_right		(_mouse_frame_middle_x + 0.1f)
#define _mouse_gauge_bottom		(_mouse_frame_bottom + 0.1f)
#define _mouse_arrow_width		(0.2f)
#define _mouse_arrow_height		(0.2f)
#define _type_func(funcname,type,...) type##_##funcname(type *obj, __VA_ARGS__)
#define _type_func_const(funcname,type,...) type##_##funcname(const type *obj, __VA_ARGS__)


//現在秒の取得
static float(*get_current_sec)() = mylib_now_sec;


//イージング↓--------------------------------------------------------
#if 1
struct easing_param_t
{
	easing_type_t _type;
	float _start;
	float _end;
	float _current;
	float _prev;
};

static float _type_func_const(start_to_end, easing_param_t)
{
	float start_to_end = obj->_end - obj->_start;
	return start_to_end;
}

static float _type_func_const(prev_to_current, easing_param_t)
{
	float prev_to_current = obj->_current - obj->_prev;
	return prev_to_current;
}

struct easing_progress_t
{
	float _start_sec;
	float _duration;
};

static float _type_func_const(elapsed, easing_progress_t, float current_sec)
{
	float elapsed = current_sec - obj->_start_sec;
	return elapsed;
}

static float _type_func_const(normalize, easing_progress_t, float current_sec)
{
	float elapsed = easing_progress_t_elapsed(obj, current_sec);
	float normalized = elapsed / obj->_duration;
	return normalized;
}

static void easing(easing_progress_t *progress, easing_param_t *param, float current_sec)
{
	float t = easing_progress_t_normalize(progress, current_sec);
	param->_prev = param->_current;
	if (t <= 0.0f)
	{
		param->_current = param->_start;
	}
	else if (t >= 1.0f)
	{
		param->_current = param->_end;
	}
	else
	{
		param->_current = call_easing(param->_type, t, param->_start, easing_param_t_start_to_end(param), 1.0f);
	}
}
#endif
//イージング↑--------------------------------------------------------


//画像の描画↓--------------------------------------------------------
#if 1
enum index_t
{
	i_none,
	i_aim_off,
	i_aim_on,
	i_mouse_arrow,
	i_mouse_char,
	i_mouse_frame,
	i_mouse_gauge,
	i_max,
};

struct texture_draw_func_t
{
	//使用するテクスチャID
	texture_id_t _tex_id[i_max];
};

static void _type_func(init, texture_draw_func_t)
{
	//使用するテクスチャID
	obj->_tex_id[i_none] = ti_none;
	obj->_tex_id[i_aim_off] = ti_opt_aim_opt_off;
	obj->_tex_id[i_aim_on] = ti_opt_aim_opt_on;
	obj->_tex_id[i_mouse_arrow] = ti_opt_mouse_opt_arrow;
	obj->_tex_id[i_mouse_char] = ti_opt_mouse_opt_char;
	obj->_tex_id[i_mouse_frame] = ti_opt_mouse_opt_frame;
	obj->_tex_id[i_mouse_gauge] = ti_opt_mouse_opt_gauge;
}

static void _type_func(end, texture_draw_func_t)
{

}

//4つの頂点を指定して描画...Z型
static void _type_func_const(draw, texture_draw_func_t, index_t index, const D3DXVECTOR3 *position, int alpha, mylib_blend_mode_t blend_mode)
{
	static D3DXVECTOR2 uv[4] = { { 0.0f, 0.0f },{ 1.0f, 0.0f } ,{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
	int alpha_correct = min(max(alpha, 0), 255);
	D3DCOLOR color = D3DCOLOR_ARGB(alpha_correct, 255, 255, 255);
	mylib_draw_board_2d(
		&position[0], &uv[0], color,
		&position[1], &uv[1], color,
		&position[2], &uv[2], color,
		&position[3], &uv[3], color,
		texture_manager_get(obj->_tex_id[index]),
		blend_mode
	);
}

//4つの頂点を指定して描画...四角
static void _type_func_const(draw_rect, texture_draw_func_t, index_t index, float left, float top, float right, float bottom, int alpha, mylib_blend_mode_t blend_mode)
{
	static D3DXVECTOR3 position[4];
	float aspect = mylib_get_aspect();
	if (aspect > 1.0f)
	{
		position[0] = { left / aspect,top,0.0f };
		position[1] = { right / aspect,top,0.0f };
		position[2] = { left / aspect,bottom,0.0f };
		position[3] = { right / aspect,bottom,0.0f };
	}
	else
	{
		position[0] = { left,top * aspect,0.0f };
		position[1] = { right,top * aspect,0.0f };
		position[2] = { left,bottom * aspect,0.0f };
		position[3] = { right,bottom * aspect,0.0f };
	}
	texture_draw_func_t_draw(obj, index, position, alpha, blend_mode);
}
#endif
//画像の描画↑--------------------------------------------------------


//エイムのOFFボタン↓--------------------------------------------------------
#if 1
struct aim_off_ui_t
{
	bool _is_active;
	easing_progress_t _easing_progress;
	float _x;
	float _y;
	easing_param_t _scale;
	easing_param_t _alpha;
	float _scale_max;
	float _scale_min;
	bool _should_draw;
	float _draw_x;
	float _draw_y;
	float _draw_scale;
	int _draw_alpha;
};

static void _type_func(init, aim_off_ui_t, float duration, float x, float y, float scale_max, float scale_min)
{
	obj->_is_active = false;
	obj->_easing_progress._duration = duration;
	obj->_x = x;
	obj->_y = y;
	obj->_scale_max = scale_max;
	obj->_scale_min = scale_min;
	obj->_scale._current = obj->_scale_min;
}

static void _type_func(end, aim_off_ui_t)
{

}

static void _type_func(update, aim_off_ui_t, float current_sec)
{
	if (obj->_is_active == false)
	{
		obj->_should_draw = false;
		return;
	}
	float easing_pos = easing_progress_t_normalize(&obj->_easing_progress, current_sec);
	if (easing_pos > 1.0f)
	{
		obj->_is_active = false;
		obj->_should_draw = false;
		return;
	}
	easing(&obj->_easing_progress, &obj->_scale, current_sec);
	easing(&obj->_easing_progress, &obj->_alpha, current_sec);
	obj->_should_draw = true;
	obj->_draw_x = obj->_x;
	obj->_draw_y = obj->_y;
	obj->_draw_scale = obj->_scale._current;
	obj->_draw_alpha = (int)(obj->_alpha._current);
}

static void _type_func(draw, aim_off_ui_t, const texture_draw_func_t *draw_func)
{
	if (obj->_should_draw == false)
	{
		return;
	}
	texture_draw_func_t_draw_rect(
		draw_func,
		i_aim_off,
		obj->_draw_x - obj->_draw_scale,
		obj->_draw_y + obj->_draw_scale,
		obj->_draw_x + obj->_draw_scale,
		obj->_draw_y - obj->_draw_scale,
		obj->_draw_alpha,
		mbm_alpha
	);
}

static void _type_func(switching, aim_off_ui_t, bool on_off, float current_sec)
{
	//to active
	obj->_is_active = true;
	//easing progress setting
	obj->_easing_progress._start_sec = current_sec;
	//easing parameter setting
	obj->_scale._type = et_EaseOutExpo;
	obj->_scale._start = obj->_scale._current;
	obj->_scale._end = on_off ? obj->_scale_max : obj->_scale_min;
	obj->_alpha._type = et_EaseInExpo;
	obj->_alpha._start = 255.0f;
	obj->_alpha._end = 0.0f;
}
#endif
//エイムのOFFボタン↑--------------------------------------------------------


//エイムのONボタン↓--------------------------------------------------------
#if 1
struct aim_on_ui_t
{
	bool _is_active;
	easing_progress_t _easing_progress;
	float _x;
	float _y;
	easing_param_t _scale;
	easing_param_t _alpha;
	float _scale_max;
	float _scale_min;
	bool _should_draw;
	float _draw_x;
	float _draw_y;
	float _draw_scale;
	int _draw_alpha;
};

static void _type_func(init, aim_on_ui_t, float duration, float x, float y, float scale_max, float scale_min)
{
	obj->_is_active = false;
	obj->_easing_progress._duration = duration;
	obj->_x = x;
	obj->_y = y;
	obj->_scale_max = scale_max;
	obj->_scale_min = scale_min;
	obj->_scale._current = obj->_scale_min;
}

static void _type_func(end, aim_on_ui_t)
{

}

static void _type_func(update, aim_on_ui_t, float current_sec)
{
	if (obj->_is_active == false)
	{
		obj->_should_draw = false;
		return;
	}
	float easing_pos = easing_progress_t_normalize(&obj->_easing_progress, current_sec);
	if (easing_pos > 1.0f)
	{
		obj->_is_active = false;
		obj->_should_draw = false;
		return;
	}
	easing(&obj->_easing_progress, &obj->_scale, current_sec);
	easing(&obj->_easing_progress, &obj->_alpha, current_sec);
	obj->_should_draw = true;
	obj->_draw_x = obj->_x;
	obj->_draw_y = obj->_y;
	obj->_draw_scale = obj->_scale._current;
	obj->_draw_alpha = (int)(obj->_alpha._current);
}

static void _type_func(draw, aim_on_ui_t, const texture_draw_func_t *draw_func)
{
	if (obj->_should_draw == false)
	{
		return;
	}
	texture_draw_func_t_draw_rect(
		draw_func,
		i_aim_on,
		obj->_draw_x - obj->_draw_scale,
		obj->_draw_y + obj->_draw_scale,
		obj->_draw_x + obj->_draw_scale,
		obj->_draw_y - obj->_draw_scale,
		obj->_draw_alpha,
		mbm_alpha
	);
}

static void _type_func(switching, aim_on_ui_t, bool on_off, float current_sec)
{
	//to active
	obj->_is_active = true;
	//easing progress setting
	obj->_easing_progress._start_sec = current_sec;
	//easing parameter setting
	obj->_scale._type = et_EaseOutExpo;
	obj->_scale._start = obj->_scale._current;
	obj->_scale._end = on_off ? obj->_scale_max : obj->_scale_min;
	obj->_alpha._type = et_EaseInExpo;
	obj->_alpha._start = 255.0f;
	obj->_alpha._end = 0.0f;
}
#endif
//エイムのONボタン↑--------------------------------------------------------


//マウス感度メーター↓--------------------------------------------------------
#if 1
struct float_rect_t
{
	D3DXVECTOR2 min;
	D3DXVECTOR2 max;
};

static void _type_func(set, float_rect_t, float left, float top, float right, float bottom)
{
	obj->min.x = left;
	obj->min.y = top;
	obj->max.x = right;
	obj->max.y = bottom;
}

static void _type_func_const(draw_image, float_rect_t, const texture_draw_func_t *func, index_t index, int alpha, mylib_blend_mode_t blend_mode)
{
	texture_draw_func_t_draw_rect(func, index, obj->min.x, obj->min.y, obj->max.x, obj->max.y, alpha, blend_mode);
}

struct mouse_ui_t
{
	bool _is_active;
	easing_progress_t _easing_progress;
	easing_param_t _alpha;
	float_rect_t _frame;
	float_rect_t _char;
	float_rect_t _gauge;
	float_rect_t _arrow;
	float _arraow_width;
	float _arrow_height;
	float _arrow_pos;
	float _arrow_pos_y;
	bool _should_draw;
	int _draw_alpha;
};

static void _type_func(init, mouse_ui_t, float duration,
	float frame_left, float frame_top, float frame_right, float frame_bottom,
	float char_left, float char_top, float char_right, float char_bottom,
	float gauge_left, float gauge_top, float gauge_right, float gauge_bottom,
	float arraow_width, float arrow_height
)
{
	obj->_is_active = false;
	obj->_easing_progress._duration = duration;
	float_rect_t_set(&obj->_frame, frame_left, frame_top, frame_right, frame_bottom);
	float_rect_t_set(&obj->_char, char_left, char_top, char_right, char_bottom);
	float_rect_t_set(&obj->_gauge, gauge_left, gauge_top, gauge_right, gauge_bottom);
	obj->_arraow_width = arraow_width;
	obj->_arrow_height = arrow_height;
}

static void _type_func(end, mouse_ui_t)
{

}

static void _type_func(update, mouse_ui_t, float current_sec)
{
	if (obj->_is_active == false)
	{
		obj->_should_draw = false;
		return;
	}
	float easing_pos = easing_progress_t_normalize(&obj->_easing_progress, current_sec);
	if (easing_pos > 1.0f)
	{
		obj->_is_active = false;
		obj->_should_draw = false;
		return;
	}
	easing(&obj->_easing_progress, &obj->_alpha, current_sec);
	obj->_should_draw = true;
	obj->_draw_alpha = (int)(obj->_alpha._current);
}

static void _type_func(draw, mouse_ui_t, const texture_draw_func_t *draw_func)
{
	if (obj->_should_draw == false)
	{
		return;
	}
	int alpha = obj->_draw_alpha;
	float_rect_t_draw_image(&obj->_frame, draw_func, i_mouse_frame, alpha, mbm_alpha);
	float_rect_t_draw_image(&obj->_char, draw_func, i_mouse_char, alpha, mbm_alpha);
	float_rect_t_draw_image(&obj->_gauge, draw_func, i_mouse_gauge, alpha, mbm_alpha);
	float_rect_t_draw_image(&obj->_arrow, draw_func, i_mouse_arrow, alpha, mbm_alpha);
}

static void _type_func(changed, mouse_ui_t, float t, float current_sec)
{
	//to active
	obj->_is_active = true;
	//easing progress setting
	obj->_easing_progress._start_sec = current_sec;
	//easing parameter setting
	obj->_alpha._type = et_EaseInExpo;
	obj->_alpha._start = 255.0f;
	obj->_alpha._end = 0.0f;
	//set position
	obj->_arrow_pos = 1.0f - t;
	obj->_arrow_pos_y = (obj->_gauge.max.y - obj->_gauge.min.y) * obj->_arrow_pos + obj->_gauge.min.y;
	obj->_arrow.min.x = obj->_gauge.min.x - _mouse_arrow_width;
	obj->_arrow.min.y = obj->_arrow_pos_y + _mouse_arrow_height / 2.0f;
	obj->_arrow.max.x = obj->_gauge.min.x;
	obj->_arrow.max.y = obj->_arrow_pos_y - _mouse_arrow_height / 2.0f;
}
#endif
//マウス感度メーター↑--------------------------------------------------------


//グローバル変数
struct global_t
{
	//エイム関係
	aim_on_ui_t _aim_on;
	aim_off_ui_t _aim_off;
	//マウス関係
	mouse_ui_t _mouse;
	//描画関係
	texture_draw_func_t _draw_func;
};

static global_t g;
//初期化
extern void option_ui_init()
{
	//エイム関係
	aim_on_ui_t_init(&g._aim_on, _aim_duration_sec, _aim_pos_x1, _aim_pos_y1, _aim_scale_max, _aim_scale_min);
	aim_off_ui_t_init(&g._aim_off, _aim_duration_sec, _aim_pos_x2, _aim_pos_y2, _aim_scale_max, _aim_scale_min);
	//マウス関係
	mouse_ui_t_init(&g._mouse, _mouse_duration_sec,
		_mouse_frame_left, _mouse_frame_top, _mouse_frame_right, _mouse_frame_bottom,
		_mouse_char_left, _mouse_char_top, _mouse_char_right, _mouse_char_bottom,
		_mouse_gauge_left, _mouse_gauge_top, _mouse_gauge_right, _mouse_gauge_bottom,
		_mouse_arrow_width, _mouse_arrow_height
	);
	//描画関係
	texture_draw_func_t_init(&g._draw_func);
}

//終了
extern void option_ui_end()
{
	//エイム関係
	aim_on_ui_t_end(&g._aim_on);
	aim_off_ui_t_end(&g._aim_off);
	//マウス関係
	mouse_ui_t_end(&g._mouse);
	//描画関係
	texture_draw_func_t_end(&g._draw_func);
}

//更新
extern void option_ui_update()
{
	//現在秒
	float current_sec = get_current_sec();
	//エイム関係
	aim_on_ui_t_update(&g._aim_on, current_sec);
	aim_off_ui_t_update(&g._aim_off, current_sec);
	//マウス関係
	mouse_ui_t_update(&g._mouse, current_sec);
}

//描画
extern void option_ui_draw()
{
	//エイム関係
	aim_on_ui_t_draw(&g._aim_on, &g._draw_func);
	aim_off_ui_t_draw(&g._aim_off, &g._draw_func);
	//マウス関係
	mouse_ui_t_draw(&g._mouse, &g._draw_func);
}


//エイムアシストのON/OFFが切り換えられたら呼ぶ
extern void option_ui_aim_assist(bool on_off)
{
	//現在秒
	float current_sec = get_current_sec();
	if (on_off == true)
	{
		//ONをでかく
		aim_on_ui_t_switching(&g._aim_on, true, current_sec);
		//OFFを小さく
		aim_off_ui_t_switching(&g._aim_off, false, current_sec);
	}
	else
	{
		//ONを小さく
		aim_on_ui_t_switching(&g._aim_on, false, current_sec);
		//OFFをでかく
		aim_off_ui_t_switching(&g._aim_off, true, current_sec);
	}
}


//マウス感度が変更されたら呼ぶ
extern void option_ui_mouse_sensitivity(float t)
{
	//現在秒
	float current_sec = get_current_sec();
	//メーターを出す
	mouse_ui_t_changed(&g._mouse, t, current_sec);
}
