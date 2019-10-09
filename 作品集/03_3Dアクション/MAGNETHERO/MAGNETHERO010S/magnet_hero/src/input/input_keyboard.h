#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

#include <Windows.h>
#include "input_button.h"


/*------------------------------------------------------------------------------*/
//初期化
extern HRESULT keyboard_manager_init(HWND window_handle);
//更新
extern void keyboard_manager_update();
//終了
extern void keyboard_manager_end();
//キー
extern const input_button_t *keyboard_button(int code);
/*------------------------------------------------------------------------------*/




