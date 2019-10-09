
//親ヘッダー
#include "test_player_brain.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "..\..\public\test_player\test_player_manager.h"
#include "..\..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\..\input\public\input.h"


//入力を調べて、どう行動するか判断する
extern void test_player_brain_t_judge_action(test_player_brain_t *brain, const test_player_object_t *)
{
	//入力チェック
	double input_x = 0.0;
	double input_y = 0.0;
	bool input_right = input_keyboard_repeat(DIK_RIGHT);
	bool input_left = input_keyboard_repeat(DIK_LEFT);
	bool input_up = input_keyboard_repeat(DIK_UP);
	bool input_down = input_keyboard_repeat(DIK_DOWN);
	input_x = input_controller_left_x();
	input_y = input_controller_left_y();

	if (input_left)
	{
		input_x -= 1.0f;
	}
	if (input_right)
	{
		input_x += 1.0f;
	}
	if (input_up)
	{
		input_y -= 1.0f;
	}
	if (input_down)
	{
		input_y += 1.0f;
	}

	brain->_should_move = false;
	brain->_move_force = hypot(input_x, input_y);
	brain->_move_direction = atan2(input_y, -input_x);

	brain->_should_jump =
		input_controller_trigger(input_controller_code_a) ||
		input_controller_trigger(input_controller_code_b) ||
		input_keyboard_trigger(DIK_Z);
}
