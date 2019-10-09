#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	

#include <Windows.h>
#include <dinput.h>
#include "input_button.h"
#include "input_controller_code.h"

/*------------------------------------------------------------------------------*/
//コールバック関数
extern BOOL CALLBACK controller_callback_enum_objects(const DIDEVICEOBJECTINSTANCE* didoi, VOID *context);
//コールバック関数
extern BOOL CALLBACK controller_callback_enum_joysticks(const DIDEVICEINSTANCE* did_instance, VOID *context);
//初期化
extern HRESULT controller_manager_init(HWND window_handle);
//更新
extern void controller_manager_update();
//終了
extern void controller_manager_end();
//デバイス
extern const LPDIRECTINPUTDEVICE8 controller_device();
//スティック
extern const DIJOYSTATE2 *controller_state();
//ボタン
extern const input_button_t *controller_button(input_controller_code_t code);
//十字ボタン
extern const input_button_t *controller_cross(input_controller_cross_code_t code);
//ZLとZRの入力値を足した値を返す
//(ZLを押すと正の方向、ZRで負の方向、どっちも押していると0、範囲は99~-100)
extern double controller_ZL_ZR();
//スティック入力の最大値
extern double controller_stick_inclination();
/*------------------------------------------------------------------------------*/

