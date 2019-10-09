#pragma once


#include <Windows.h>
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#include "input_button.h"
#include "..\public\input_mouse_code.h"

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

