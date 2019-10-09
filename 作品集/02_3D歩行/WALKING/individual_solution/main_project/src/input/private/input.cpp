

//�e�w�b�_�[
#include "..\public\input.h"
//���̑��̃w�b�_�[
#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_controller.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"








/*-------------------------------------------------------------------------*/
extern int input_init(HWND window_handle)
{
	//�L�[�{�[�h�̏�����
	if (keyboard_manager_init(window_handle) != 0)
	{
		return -1;
	}

	//�}�E�X�̏�����
	mouse_manager_init(window_handle);

	//�R���g���[���[�̏�����
	controller_manager_init(window_handle);

	return 0;
}

extern void input_update()
{
	//�L�[�{�[�h�̍X�V
	keyboard_manager_update();

	//�}�E�X�̍X�V
	mouse_manager_update();

	//�R���g���[���[�̍X�V
	controller_manager_update();
}

extern void input_end()
{
	//�L�[�{�[�h�̏I��
	keyboard_manager_end();

	//�}�E�X�̏I��
	mouse_manager_end();

	//�R���g���[���[�̏I��
	controller_manager_end();
}
/*-------------------------------------------------------------------------*/














/*-------------------------------------------------------------------------*/
//�L�[��������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_keyboard_count(int code)
{
	return
		input_button_t_count(
			keyboard_button(code)
		);
}
//�L�[�������ꂽ�u�Ԃ����f����
extern bool input_keyboard_trigger(int code)
{
	return
		input_button_t_trigger(
			keyboard_button(code)
		);
}
//�L�[��������Ă��邩���f����
extern bool input_keyboard_repeat(int code)
{
	return
		input_button_t_repeat(
			keyboard_button(code)
		);
}
//�L�[�������ꂽ�u�Ԃ����f����
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
//�L�[��������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_mouse_count(input_mouse_code_t code)
{
	return
		input_button_t_count(
			mouse_button(code)
		);
}
//�L�[�������ꂽ�u�Ԃ����f����
extern bool input_mouse_trigger(input_mouse_code_t code)
{
	return
		input_button_t_trigger(
			mouse_button(code)
		);
}
//�L�[��������Ă��邩���f����
extern bool input_mouse_repeat(input_mouse_code_t code)
{
	return
		input_button_t_repeat(
			mouse_button(code)
		);
}
//�L�[�������ꂽ�u�Ԃ����f����
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
//�L�[��������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_controller_count(input_controller_code_t code)
{
	return
		input_button_t_count(
			controller_button(code)
		);
}
//�L�[�������ꂽ�u�Ԃ����f����
extern bool input_controller_trigger(input_controller_code_t code)
{
	return
		input_button_t_trigger(
			controller_button(code)
		);
}
//�L�[��������Ă��邩���f����
extern bool input_controller_repeat(input_controller_code_t code)
{
	return
		input_button_t_repeat(
			controller_button(code)
		);
}
//�L�[�������ꂽ�u�Ԃ����f����
extern bool input_controller_release(input_controller_code_t code)
{
	return
		input_button_t_release(
			controller_button(code)
		);
}
//�\���{�^����������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern int input_controller_cross_count(input_controller_cross_code_t code)
{
	return
		input_button_t_count(
			controller_cross(code)
		);
}
//�\���{�^���������ꂽ�u�Ԃ����f����
extern bool input_controller_cross_trigger(input_controller_cross_code_t code)
{
	return
		input_button_t_trigger(
			controller_cross(code)
		);
}
//�\���{�^����������Ă��邩���f����
extern bool input_controller_cross_repeat(input_controller_cross_code_t code)
{
	return
		input_button_t_repeat(
			controller_cross(code)
		);
}
//�\���{�^���������ꂽ�u�Ԃ����f����
extern bool input_controller_cross_release(input_controller_cross_code_t code)
{
	return
		input_button_t_release(
			controller_cross(code)
		);
}
//ZL��ZR�̓��͒l�𑫂����l��Ԃ�
//(ZL�������Ɛ��̕����AZR�ŕ��̕����A�ǂ����������Ă����0�A�͈͂�99~-100)
extern int input_controller_ZL_ZR()
{
	return controller_ZL_ZR();
}
/*-------------------------------------------------------------------------*/





