#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	


#include <Windows.h>
#include <dinput.h>
#include "input_button.h"
#include "input_mouse_code.h"

/*------------------------------------------------------------------------------*/
//������
extern HRESULT mouse_manager_init(HWND window_handle);
//�X�V
extern void mouse_manager_update();
//�I��
extern void mouse_manager_end();
//�ړ��Ƃ�
extern const DIMOUSESTATE2 *mouse_state();
//�N���b�N
extern const input_button_t *mouse_button(input_mouse_code_t code);
/*------------------------------------------------------------------------------*/

