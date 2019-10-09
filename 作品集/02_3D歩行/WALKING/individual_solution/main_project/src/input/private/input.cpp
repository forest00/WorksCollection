

//親ヘッダー
#include "..\public\input.h"
//その他のヘッダー
#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_controller.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"








/*-------------------------------------------------------------------------*/
extern int input_init(HWND window_handle)
{
	//キーボードの初期化
	if (keyboard_manager_init(window_handle) != 0)
	{
		return -1;
	}

	//マウスの初期化
	mouse_manager_init(window_handle);

	//コントローラーの初期化
	controller_manager_init(window_handle);

	return 0;
}

extern void input_update()
{
	//キーボードの更新
	keyboard_manager_update();

	//マウスの更新
	mouse_manager_update();

	//コントローラーの更新
	controller_manager_update();
}

extern void input_end()
{
	//キーボードの終了
	keyboard_manager_end();

	//マウスの終了
	mouse_manager_end();

	//コントローラーの終了
	controller_manager_end();
}
/*-------------------------------------------------------------------------*/














/*-------------------------------------------------------------------------*/
//キーが押されている時間を返す(押されていなければ0)
extern int input_keyboard_count(int code)
{
	return
		input_button_t_count(
			keyboard_button(code)
		);
}
//キーが押された瞬間か判断する
extern bool input_keyboard_trigger(int code)
{
	return
		input_button_t_trigger(
			keyboard_button(code)
		);
}
//キーが押されているか判断する
extern bool input_keyboard_repeat(int code)
{
	return
		input_button_t_repeat(
			keyboard_button(code)
		);
}
//キーが離された瞬間か判断する
extern bool input_keyboard_release(int code)
{
	return
		input_button_t_release(
			keyboard_button(code)
		);
}
/*-------------------------------------------------------------------------*/












/*-------------------------------------------------------------------------*/
extern int input_mouse_move_x()
{
	return mouse_state()->lX;
}
extern int input_mouse_move_y()
{
	return mouse_state()->lY;
}
extern int input_mouse_wheel()
{
	return mouse_state()->lZ;
}
//キーが押されている時間を返す(押されていなければ0)
extern int input_mouse_count(input_mouse_code_t code)
{
	return
		input_button_t_count(
			mouse_button(code)
		);
}
//キーが押された瞬間か判断する
extern bool input_mouse_trigger(input_mouse_code_t code)
{
	return
		input_button_t_trigger(
			mouse_button(code)
		);
}
//キーが押されているか判断する
extern bool input_mouse_repeat(input_mouse_code_t code)
{
	return
		input_button_t_repeat(
			mouse_button(code)
		);
}
//キーが離された瞬間か判断する
extern bool input_mouse_release(input_mouse_code_t code)
{
	return
		input_button_t_release(
			mouse_button(code)
		);
}
/*-------------------------------------------------------------------------*/















/*-------------------------------------------------------------------------*/
extern double input_controller_left_x()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)(controller_state()->lX) / controller_stick_inclination();
}
extern double input_controller_left_y()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)(controller_state()->lY + 1) / controller_stick_inclination();
}
extern double input_controller_right_x()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)(controller_state()->lRx) / controller_stick_inclination();
}
extern double input_controller_right_y()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)(controller_state()->lRy + 1) / controller_stick_inclination();
}
//キーが押されている時間を返す(押されていなければ0)
extern int input_controller_count(input_controller_code_t code)
{
	return
		input_button_t_count(
			controller_button(code)
		);
}
//キーが押された瞬間か判断する
extern bool input_controller_trigger(input_controller_code_t code)
{
	return
		input_button_t_trigger(
			controller_button(code)
		);
}
//キーが押されているか判断する
extern bool input_controller_repeat(input_controller_code_t code)
{
	return
		input_button_t_repeat(
			controller_button(code)
		);
}
//キーが離された瞬間か判断する
extern bool input_controller_release(input_controller_code_t code)
{
	return
		input_button_t_release(
			controller_button(code)
		);
}
//十字ボタンが押されている時間を返す(押されていなければ0)
extern int input_controller_cross_count(input_controller_cross_code_t code)
{
	return
		input_button_t_count(
			controller_cross(code)
		);
}
//十字ボタンが押された瞬間か判断する
extern bool input_controller_cross_trigger(input_controller_cross_code_t code)
{
	return
		input_button_t_trigger(
			controller_cross(code)
		);
}
//十字ボタンが押されているか判断する
extern bool input_controller_cross_repeat(input_controller_cross_code_t code)
{
	return
		input_button_t_repeat(
			controller_cross(code)
		);
}
//十字ボタンが離された瞬間か判断する
extern bool input_controller_cross_release(input_controller_cross_code_t code)
{
	return
		input_button_t_release(
			controller_cross(code)
		);
}
//ZLとZRの入力値を足した値を返す
//(ZLを押すと正の方向、ZRで負の方向、どっちも押していると0、範囲は99~-100)
extern int input_controller_ZL_ZR()
{
	return controller_ZL_ZR();
}
/*-------------------------------------------------------------------------*/





