//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

//親ヘッダー
#include "input_keyboard.h"
//その他のヘッダー
#include <dinput.h>
#include "input_button.h"
#include "input_define.h"


/*------------------------------------------------------------------------------*/
//キーボード入力
//direct inputの変数
static LPDIRECTINPUT8 g_keyboard = nullptr;
static LPDIRECTINPUTDEVICE8 g_keyboard_device = nullptr;
//ボタンの配列
static unsigned char g_keyboard_keys[256];
static input_button_t g_keyboard_buttons[256];
/*------------------------------------------------------------------------------*/











/*-------------------------------------------------------------------------*/


//キーボードの使用開始
extern HRESULT keyboard_manager_init(HWND window_handle)
{
	//キーボード入力の初期化関数
	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_keyboard, nullptr)))
	{
		return E_FAIL;
	}
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(g_keyboard->CreateDevice(GUID_SysKeyboard,
		&g_keyboard_device, nullptr)))
	{
		return E_FAIL;
	}
	// デバイスをキーボードに設定
	if (FAILED(g_keyboard_device->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(g_keyboard_device->SetCooperativeLevel(
		window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	// デバイスを「取得」する
	g_keyboard_device->Acquire();
	return S_OK;
}
//キーボードの全チェック
extern void keyboard_manager_update()
{
	if (g_keyboard_device)
	{
		HRESULT h_result = g_keyboard_device->Acquire();
		if (h_result == DI_OK || h_result == S_FALSE)
		{
			g_keyboard_device->GetDeviceState(sizeof(g_keyboard_keys), &g_keyboard_keys);
			for (int i = 0; i < 256; i++)
			{
				input_button_t_count_up_if(
					&g_keyboard_buttons[i], g_keyboard_keys[i] & 0x80
				);
			}
		}
	}
}
//キーボード使用終了
extern void keyboard_manager_end()
{
	if (g_keyboard_device != nullptr)
	{
		g_keyboard_device->Unacquire();
	}
	input_define_safe_release(g_keyboard_device);
	input_define_safe_release(g_keyboard);
}

//キー
extern const input_button_t *keyboard_button(int code)
{
	return &g_keyboard_buttons[code];
}
/*-------------------------------------------------------------------------*/










