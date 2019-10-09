//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	

//�e�w�b�_�[
#include "input.h"
//���̑��̃w�b�_�[
#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_controller.h"








/*-------------------------------------------------------------------------*/
extern int input_init(HWND window_handle)
{
	int result = 0;
	//�L�[�{�[�h�̏�����
	if (FAILED(keyboard_manager_init(window_handle)))
	{
		//MessageBox(nullptr, "�L�[�{�[�h�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);
		result = -1;
	}

	//�}�E�X�̏�����
	if (FAILED(mouse_manager_init(window_handle)))
	{
		//MessageBox(nullptr, "�}�E�X�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);
		result = -2;
	}

	//�R���g���[���[�̏�����
	if (FAILED(controller_manager_init(window_handle)))
	{
		//MessageBox(nullptr, "�R���g���[���[�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);
		result = -3;
	}

	return result;
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
extern time_t input_keyboard_count(int code)
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
extern time_t input_mouse_count(input_mouse_code_t code)
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
static int correct(LONG axis)
{
	int c = axis / 5;
	return c * 5;
}
extern double input_controller_left_x()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)correct(controller_state()->lX) / controller_stick_inclination();
}
extern double input_controller_left_y()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)correct(controller_state()->lY) / controller_stick_inclination();
}
extern double input_controller_right_x()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)correct(controller_state()->lRx) / controller_stick_inclination();
}
extern double input_controller_right_y()
{
	if (controller_device() == nullptr)
	{
		return 0.0;
	}
	return (double)correct(controller_state()->lRy) / controller_stick_inclination();
}
//�L�[��������Ă��鎞�Ԃ�Ԃ�(������Ă��Ȃ����0)
extern time_t input_controller_count(input_controller_code_t code)
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
extern time_t input_controller_cross_count(input_controller_cross_code_t code)
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
//(ZL�������Ɛ��̕����AZR�ŕ��̕����A�ǂ����������Ă����0�A�͈͂�0.99~-1.00)
extern float input_controller_ZL_ZR()
{
	return (float)(controller_ZL_ZR());
}
/*-------------------------------------------------------------------------*/





