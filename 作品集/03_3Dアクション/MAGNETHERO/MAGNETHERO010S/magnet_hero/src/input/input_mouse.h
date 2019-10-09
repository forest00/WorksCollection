#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	


#include <Windows.h>
#include <dinput.h>
#include "input_button.h"
#include "input_mouse_code.h"

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

