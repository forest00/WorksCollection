//親ヘッダー
#include "option.h"
//その他のヘッダー
#include "option_ui.h"
#include "..\input\input.h"

//グローバル変数
struct global_t
{
	bool _do_aim_assist;
	float _mouse_sensitivity;
};

static global_t g;

//ゲーム開始時に呼ぶ(※mylibの初期化の後)
extern void option_init()
{
	g._do_aim_assist = true;
	g._mouse_sensitivity = 0.5f;
	option_ui_init();
}

//ゲーム終了時に呼ぶ
extern void option_end()
{
	option_ui_end();
}

//更新：毎フレーム呼ぶ
extern void option_update()
{
	bool change_aim_assist = input_mouse_trigger(imc_right) || input_keyboard_trigger(DIK_M);
	float stages = (float)(15);
	float add_mouse_sensitivity = (float)(input_mouse_wheel()) / 120.0f / stages;
	add_mouse_sensitivity += (float)(input_keyboard_repeat(DIK_N)) / stages;
	add_mouse_sensitivity -= (float)(input_keyboard_repeat(DIK_B)) / stages;
	if (change_aim_assist)
	{
		g._do_aim_assist = !g._do_aim_assist;
		option_ui_aim_assist(g._do_aim_assist);
	}
	if (add_mouse_sensitivity)
	{
		g._mouse_sensitivity += add_mouse_sensitivity;
		g._mouse_sensitivity = min(max(g._mouse_sensitivity, 0.0f), 1.0f);
		option_ui_mouse_sensitivity(g._mouse_sensitivity);
	}
	option_ui_update();
}

//描画：毎フレーム呼ぶ
extern void option_draw()
{
	option_ui_draw();
}

//エイムアシストをする
extern bool option_get_do_aim_assist()
{
	return g._do_aim_assist;
}

//マウス感度(0~1)
extern float option_get_mouse_sensitivity()
{
	return g._mouse_sensitivity;
}