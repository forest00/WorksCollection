#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	

#include <Windows.h>
#include <dinput.h>
#include "input_button.h"
#include "input_controller_code.h"

/*------------------------------------------------------------------------------*/
//�R�[���o�b�N�֐�
extern BOOL CALLBACK controller_callback_enum_objects(const DIDEVICEOBJECTINSTANCE* didoi, VOID *context);
//�R�[���o�b�N�֐�
extern BOOL CALLBACK controller_callback_enum_joysticks(const DIDEVICEINSTANCE* did_instance, VOID *context);
//������
extern HRESULT controller_manager_init(HWND window_handle);
//�X�V
extern void controller_manager_update();
//�I��
extern void controller_manager_end();
//�f�o�C�X
extern const LPDIRECTINPUTDEVICE8 controller_device();
//�X�e�B�b�N
extern const DIJOYSTATE2 *controller_state();
//�{�^��
extern const input_button_t *controller_button(input_controller_code_t code);
//�\���{�^��
extern const input_button_t *controller_cross(input_controller_cross_code_t code);
//ZL��ZR�̓��͒l�𑫂����l��Ԃ�
//(ZL�������Ɛ��̕����AZR�ŕ��̕����A�ǂ����������Ă����0�A�͈͂�99~-100)
extern double controller_ZL_ZR();
//�X�e�B�b�N���͂̍ő�l
extern double controller_stick_inclination();
/*------------------------------------------------------------------------------*/

