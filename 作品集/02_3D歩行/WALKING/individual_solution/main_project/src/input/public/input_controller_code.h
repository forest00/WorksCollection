#pragma once

//コントローラーのボタン表
enum input_controller_code_t
{
	input_controller_code_a,
	input_controller_code_b,
	input_controller_code_x,
	input_controller_code_y,
	input_controller_code_l,
	input_controller_code_r,
	input_controller_code_back,
	input_controller_code_start,
	input_controller_code_left_stick_push,
	input_controller_code_right_stick_push,
	input_controller_code__max,
};

//コントローラーの十字ボタン
enum input_controller_cross_code_t
{
	input_controller_cross_code_up,
	input_controller_cross_code_right,
	input_controller_cross_code_down,
	input_controller_cross_code_left,
	input_controller_cross_code__max,
};
