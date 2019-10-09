//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	

//親ヘッダー
#include "input_controller.h"
//その他のヘッダー
#include "input_controller_code.h"
#include "input_define.h"


/*------------------------------------------------------------------------------*/
//コントローラー入力
//direct inputの変数
static LPDIRECTINPUT8 g_controller = nullptr;
static LPDIRECTINPUTDEVICE8 g_controller_device = nullptr;
static DIJOYSTATE2 g_controller_state = {};
//ボタンの配列
static input_button_t g_controller_buttons[(size_t)(icc_max)];
static input_button_t g_controller_cross[(size_t)(iccc_max)];
/*------------------------------------------------------------------------------*/





/*------------------------------------------------------------------------------*/
extern const double c_stick_inclination = 100.0;
/*------------------------------------------------------------------------------*/






/*-------------------------------------------------------------------------*/
//アプリケーションで使用するコントローラーのプロパティを列挙して設定するコールバック関数
extern BOOL CALLBACK controller_callback_enum_objects(const DIDEVICEOBJECTINSTANCE* didoi, VOID *context)
{
	UNREFERENCED_PARAMETER(context);
	if (didoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = didoi->dwType;
		diprg.lMin = -(long)(c_stick_inclination);
		diprg.lMax = (long)(c_stick_inclination);
		if (FAILED(g_controller_device->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//利用可能なジョイスティックを列挙するコールバック関数
extern BOOL CALLBACK controller_callback_enum_joysticks(const DIDEVICEINSTANCE* did_instance, VOID *context)
{
	UNREFERENCED_PARAMETER(context);
#if 0
	//複数列挙される場合、ユーザーに選択・確認させる
	TCHAR szConfirm[MAX_PATH + 1];
	sprintf(szConfirm, "この物理デバイスでデバイスオブジェクトを作成しますか？\n%s\n%s",
		did_instance->tszProductName, did_instance->tszInstanceName);
	if (MessageBox(0, szConfirm, "確認", MB_YESNO) == IDNO)
	{
		return DIENUM_CONTINUE;
	}
#endif
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(g_controller->CreateDevice(did_instance->guidInstance,
		&g_controller_device, nullptr)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}




//コントローラー入力の初期化関数
extern HRESULT controller_manager_init(HWND window_handle)
{
	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_controller, nullptr)))
	{
		return E_FAIL;
	}
	//利用可能なコントローラーの列挙関数を実行
	if (FAILED(g_controller->EnumDevices(DI8DEVCLASS_GAMECTRL, controller_callback_enum_joysticks,
		nullptr, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	if (g_controller_device == nullptr)
	{
		return E_FAIL;
	}
	// デバイスをジョイスティックに設定
	if (FAILED(g_controller_device->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(g_controller_device->SetCooperativeLevel(
		window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
	if (FAILED(g_controller_device->EnumObjects(controller_callback_enum_objects,
		nullptr, DIDFT_ALL)))
	{
		return E_FAIL;
	}
	// デバイスを「取得」する
	g_controller_device->Acquire();
	return S_OK;
}
//コントローラーからの入力を調べる
extern void controller_manager_update()
{
	if (g_controller_device == nullptr)
	{
		return;
	}
	HRESULT hr = g_controller_device->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		g_controller_device->GetDeviceState(sizeof(g_controller_state), &g_controller_state);
		for (int i = 0; i < (int)(icc_max); i++)
		{
			input_button_t_count_up_if(
				&g_controller_buttons[i], g_controller_state.rgbButtons[i] & 0x80
			);
		}
		///g_controller_state.rgdwPOV[0]には、十字ボタンの入力"方向"が格納されている
		///何とかしてこれをボタン4つに切り分ける
		//まずは入力チェック
		if (g_controller_state.rgdwPOV[0] == -1)
		{
			input_button_t_count_up_if(&g_controller_cross[0], false);
			input_button_t_count_up_if(&g_controller_cross[1], false);
			input_button_t_count_up_if(&g_controller_cross[2], false);
			input_button_t_count_up_if(&g_controller_cross[3], false);
		}
		else
		{
			int dir = g_controller_state.rgdwPOV[0];
			input_button_t_count_up_if(&g_controller_cross[0], (0 <= dir && dir <= 4500) || (31500 <= dir && dir <= 36000));
			input_button_t_count_up_if(&g_controller_cross[1], (4500 <= dir && dir <= 13500));
			input_button_t_count_up_if(&g_controller_cross[2], (13500 <= dir && dir <= 22500));
			input_button_t_count_up_if(&g_controller_cross[3], (22500 <= dir && dir <= 31500));
		}
	}
}
//コントローラーの使用終了
extern void controller_manager_end()
{
	if (g_controller_device)
	{
		g_controller_device->Unacquire();
	}
	input_define_safe_release(g_controller_device);
	input_define_safe_release(g_controller);
}
//デバイス
extern const LPDIRECTINPUTDEVICE8 controller_device()
{
	return g_controller_device;
}
//スティック
extern const DIJOYSTATE2 *controller_state()
{
	return &g_controller_state;
}
//ボタン
extern const input_button_t *controller_button(input_controller_code_t code)
{
	return &g_controller_buttons[(int)(code)];
}
//十字ボタン
extern const input_button_t *controller_cross(input_controller_cross_code_t code)
{
	return &g_controller_cross[(size_t)(code)];
}
//ZLとZRの入力値を足した値を返す
//(ZLを押すと正の方向、ZRで負の方向、どっちも押していると0、範囲は0.99~-1.00)
extern double controller_ZL_ZR()
{
	return (double)(g_controller_state.lZ) / 100.0;
}
//スティック入力の最大値
extern double controller_stick_inclination()
{
	return (double)(c_stick_inclination);
}
/*-------------------------------------------------------------------------*/






