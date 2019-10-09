
//親ヘッダー
#include "input_mouse.h"
//その他のヘッダー
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\common_def\public\common_def.h"


/*------------------------------------------------------------------------------*/
//マウス入力
//direct inputの変数
static LPDIRECTINPUT8 g_mouse = nullptr;
static LPDIRECTINPUTDEVICE8 g_mouse_device = nullptr;
static DIMOUSESTATE2 g_mouse_state = {};
//ボタンの配列
static input_button_t g_mouse_buttons[input_mouse_code__max];
/*------------------------------------------------------------------------------*/











/*-------------------------------------------------------------------------*/
//マウス入力の初期化関数
extern HRESULT mouse_manager_init(HWND window_handle)
{
	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_mouse, NULL)))
	{
		return E_FAIL;
	}
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(g_mouse->CreateDevice(GUID_SysMouse, &g_mouse_device, NULL)))
	{
		return E_FAIL;
	}
	// デバイスをマウスに設定
	if (FAILED(g_mouse_device->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(g_mouse_device->SetCooperativeLevel(
		window_handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}
	// デバイスを「取得」する
	if (FAILED(g_mouse_device->Acquire()))
	{
		return E_FAIL;
	}
	return S_OK;
}
//マウスからの入力を調べる
extern void mouse_manager_update()
{
	if (g_mouse_device == nullptr)
	{
		return;
	}
	if (SUCCEEDED(g_mouse_device->GetDeviceState(sizeof(g_mouse_state), &g_mouse_state)))
	{
		g_mouse_device->Acquire();
		for (int i = 0; i < (int)(input_mouse_code__max); i++)
		{
			input_button_t_count_up_if(
				&g_mouse_buttons[i], g_mouse_state.rgbButtons[i] & 0x80
			);
		}
	}
}
//マウスの使用終了
extern void mouse_manager_end()
{
	if (g_mouse_device)
	{
		g_mouse_device->Unacquire();
	}
	safe_release(g_mouse_device);
	safe_release(g_mouse);
}
//移動とか
extern const DIMOUSESTATE2 *mouse_state()
{
	return &g_mouse_state;
}
//クリック
extern const input_button_t *mouse_button(input_mouse_code_t code)
{
	return &g_mouse_buttons[(int)(code)];
}
/*-------------------------------------------------------------------------*/







