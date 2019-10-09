//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	

//親ヘッダー
#include "input_mouse.h"
//その他のヘッダー
#include "input_define.h"


/*------------------------------------------------------------------------------*/
//マウス入力
//direct inputの変数
static LPDIRECTINPUT8 g_mouse = nullptr;
static LPDIRECTINPUTDEVICE8 g_mouse_device = nullptr;
static DIMOUSESTATE2 g_mouse_state = {};
//ボタンの配列
static input_button_t g_mouse_buttons[(size_t)(imc_max)];
/*------------------------------------------------------------------------------*/











/*-------------------------------------------------------------------------*/
//マウス入力の初期化関数
extern HRESULT mouse_manager_init(HWND window_handle)
{
	HRESULT ret = S_FALSE;
	// 「DirectInput」オブジェクトの作成

	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_mouse, nullptr)))
	{
		return E_FAIL;
	}

	if (g_mouse == NULL)
	{
		return E_FAIL;
	}

	// マウス用にデバイスオブジェクトを作成
	ret = g_mouse->CreateDevice(GUID_SysMouse, &g_mouse_device, NULL);
	if (FAILED(ret))
	{
		// デバイスの作成に失敗
		return E_FAIL;
	}

	// データフォーマットを設定
	ret = g_mouse_device->SetDataFormat(&c_dfDIMouse2);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret))
	{
		// データフォーマットに失敗
		return E_FAIL;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	ret = g_mouse_device->SetCooperativeLevel(window_handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret))
	{
		// モードの設定に失敗
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = g_mouse_device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret))
	{
		// デバイスの設定に失敗
		return E_FAIL;
	}

	// 入力制御開始
	ret = g_mouse_device->Acquire();
	if (FAILED(ret))
	{
		// 入力制御開始に失敗
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
		for (int i = 0; i < (int)(imc_max); i++)
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
	input_define_safe_release(g_mouse_device);
	input_define_safe_release(g_mouse);
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







