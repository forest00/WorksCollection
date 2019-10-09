#pragma once


#include <Windows.h>
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#include "input_button.h"
#include "..\public\input_mouse_code.h"

/*------------------------------------------------------------------------------*/
//初期化
extern HRESULT mouse_manager_init(HWND window_handle);
//更新
extern void mouse_manager_update();
//終了
extern void mouse_manager_end();
//移動とか
extern const DIMOUSESTATE2 *mouse_state();
//クリック
extern const input_button_t *mouse_button(input_mouse_code_t code);
/*------------------------------------------------------------------------------*/

