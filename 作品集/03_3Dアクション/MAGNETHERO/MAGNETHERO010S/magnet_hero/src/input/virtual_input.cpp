//親ヘッダー
#include "virtual_input.h"
//その他のヘッダー
#include "input.h"


static void virtual_input_repeat_update(virtual_input_t *repeat)
{
	repeat->_lstick_x = (float)input_controller_left_x();
	repeat->_lstick_y = (float)input_controller_left_y();
	repeat->_rstick_x = (float)input_controller_right_x();
	repeat->_rstick_y = (float)input_controller_right_y();
	repeat->_lkey_left =
		input_keyboard_repeat(DIK_A) |
		input_controller_cross_repeat(iccc_left) |
		(repeat->_lstick_x < -0.5f) |
		false;
	repeat->_lkey_up =
		input_keyboard_repeat(DIK_W) |
		input_controller_cross_repeat(iccc_up) |
		(repeat->_lstick_y < -0.5f) |
		false;
	repeat->_lkey_right =
		input_keyboard_repeat(DIK_D) |
		input_controller_cross_repeat(iccc_right) |
		(repeat->_lstick_x > 0.5f) |
		false;
	repeat->_lkey_down =
		input_keyboard_repeat(DIK_S) |
		input_controller_cross_repeat(iccc_down) |
		(repeat->_lstick_y > 0.5f) |
		false;
	repeat->_rkey_left =
		input_keyboard_repeat(DIK_LEFTARROW) |
		(repeat->_rstick_x < -0.5f) |
		false;
	repeat->_rkey_up =
		input_keyboard_repeat(DIK_UPARROW) |
		(repeat->_rstick_y < -0.5f) |
		false;
	repeat->_rkey_right =
		input_keyboard_repeat(DIK_RIGHTARROW) |
		(repeat->_rstick_x > 0.5f) |
		false;
	repeat->_rkey_down =
		input_keyboard_repeat(DIK_DOWNARROW) |
		(repeat->_rstick_y > 0.5f) |
		false;
	repeat->_a =
		input_keyboard_repeat(DIK_Z) |
		input_keyboard_trigger(DIK_SPACE) |
		input_keyboard_trigger(DIK_RETURN) |
		input_keyboard_trigger(DIK_NUMPADENTER) |
		input_keyboard_repeat(DIK_LSHIFT) |
		input_keyboard_repeat(DIK_RSHIFT) |
		input_mouse_repeat(imc_left) |
		input_controller_repeat(icc_a) |
		input_controller_repeat(icc_b) |
		input_controller_repeat(icc_r) |
		input_controller_repeat(icc_l) |
		false;
	repeat->_b =
		input_keyboard_repeat(DIK_X) |
		input_mouse_repeat(imc_right) |
		input_controller_repeat(icc_x) |
		input_controller_repeat(icc_y) |
		false;
	repeat->_pause =
		input_keyboard_repeat(DIK_C) |
		input_keyboard_repeat(DIK_Q) |
		input_keyboard_repeat(DIK_E) |
		input_keyboard_repeat(DIK_RCONTROL) |
		input_keyboard_repeat(DIK_LCONTROL) |
		input_keyboard_repeat(DIK_BACKSPACE) |
		input_keyboard_repeat(DIK_INSERT) |
		input_keyboard_repeat(DIK_DELETE) |
		input_mouse_repeat(imc_middle) |
		input_controller_trigger(icc_start) |
		input_controller_trigger(icc_right_stick_push) |
		input_controller_trigger(icc_left_stick_push) |
		false;
}

static void virtual_input_trigger_update(const virtual_input_t *prev_repeat, const virtual_input_t *repeat, virtual_input_t *trigger)
{
	trigger->_lstick_x = 0.0f;
	trigger->_lstick_y = 0.0f;
	trigger->_rstick_x = 0.0f;
	trigger->_rstick_y = 0.0f;
	trigger->_lkey_up = repeat->_lkey_up && !prev_repeat->_lkey_up;
	trigger->_lkey_down = repeat->_lkey_down && !prev_repeat->_lkey_down;
	trigger->_lkey_right = repeat->_lkey_right && !prev_repeat->_lkey_right;
	trigger->_lkey_left = repeat->_lkey_left && !prev_repeat->_lkey_left;
	trigger->_rkey_up = repeat->_rkey_up && !prev_repeat->_rkey_up;
	trigger->_rkey_down = repeat->_rkey_down && !prev_repeat->_rkey_down;
	trigger->_rkey_right = repeat->_rkey_right && !prev_repeat->_rkey_right;
	trigger->_rkey_left = repeat->_rkey_left && !prev_repeat->_rkey_left;
	trigger->_a = repeat->_a && !prev_repeat->_a;
	trigger->_b = repeat->_b && !prev_repeat->_b;
	trigger->_pause = repeat->_pause && !prev_repeat->_pause;
}

static void virtual_input_release_update(const virtual_input_t *prev_repeat, const virtual_input_t *repeat, virtual_input_t *release)
{
	release->_lstick_x = 0.0f;
	release->_lstick_y = 0.0f;
	release->_rstick_x = 0.0f;
	release->_rstick_y = 0.0f;
	release->_lkey_up = !repeat->_lkey_up && prev_repeat->_lkey_up;
	release->_lkey_down = !repeat->_lkey_down && prev_repeat->_lkey_down;
	release->_lkey_right = !repeat->_lkey_right && prev_repeat->_lkey_right;
	release->_lkey_left = !repeat->_lkey_left && prev_repeat->_lkey_left;
	release->_rkey_up = !repeat->_rkey_up && prev_repeat->_rkey_up;
	release->_rkey_down = !repeat->_rkey_down && prev_repeat->_rkey_down;
	release->_rkey_right = !repeat->_rkey_right && prev_repeat->_rkey_right;
	release->_rkey_left = !repeat->_rkey_left && prev_repeat->_rkey_left;
	release->_a = !repeat->_a && prev_repeat->_a;
	release->_b = !repeat->_b && prev_repeat->_b;
	release->_pause = !repeat->_pause && prev_repeat->_pause;
}

extern void virtual_input_update(virtual_input_t *repeat, virtual_input_t *trigger, virtual_input_t *release)
{
	virtual_input_t prev_repeat = *repeat;
	virtual_input_repeat_update(repeat);
	virtual_input_trigger_update(&prev_repeat, repeat, trigger);
	virtual_input_release_update(&prev_repeat, repeat, release);
}