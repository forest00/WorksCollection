#pragma once

#include <Windows.h>
#include "input_button.h"


/*------------------------------------------------------------------------------*/
//������
extern HRESULT keyboard_manager_init(HWND window_handle);
//�X�V
extern void keyboard_manager_update();
//�I��
extern void keyboard_manager_end();
//�L�[
extern const input_button_t *keyboard_button(int code);
/*------------------------------------------------------------------------------*/




