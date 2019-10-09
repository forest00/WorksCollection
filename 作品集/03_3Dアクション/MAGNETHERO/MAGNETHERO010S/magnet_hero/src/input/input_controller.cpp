//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	

//�e�w�b�_�[
#include "input_controller.h"
//���̑��̃w�b�_�[
#include "input_controller_code.h"
#include "input_define.h"


/*------------------------------------------------------------------------------*/
//�R���g���[���[����
//direct input�̕ϐ�
static LPDIRECTINPUT8 g_controller = nullptr;
static LPDIRECTINPUTDEVICE8 g_controller_device = nullptr;
static DIJOYSTATE2 g_controller_state = {};
//�{�^���̔z��
static input_button_t g_controller_buttons[(size_t)(icc_max)];
static input_button_t g_controller_cross[(size_t)(iccc_max)];
/*------------------------------------------------------------------------------*/





/*------------------------------------------------------------------------------*/
extern const double c_stick_inclination = 100.0;
/*------------------------------------------------------------------------------*/






/*-------------------------------------------------------------------------*/
//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��R�[���o�b�N�֐�
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

//���p�\�ȃW���C�X�e�B�b�N��񋓂���R�[���o�b�N�֐�
extern BOOL CALLBACK controller_callback_enum_joysticks(const DIDEVICEINSTANCE* did_instance, VOID *context)
{
	UNREFERENCED_PARAMETER(context);
#if 0
	//�����񋓂����ꍇ�A���[�U�[�ɑI���E�m�F������
	TCHAR szConfirm[MAX_PATH + 1];
	sprintf(szConfirm, "���̕����f�o�C�X�Ńf�o�C�X�I�u�W�F�N�g���쐬���܂����H\n%s\n%s",
		did_instance->tszProductName, did_instance->tszInstanceName);
	if (MessageBox(0, szConfirm, "�m�F", MB_YESNO) == IDNO)
	{
		return DIENUM_CONTINUE;
	}
#endif
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(g_controller->CreateDevice(did_instance->guidInstance,
		&g_controller_device, nullptr)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}




//�R���g���[���[���͂̏������֐�
extern HRESULT controller_manager_init(HWND window_handle)
{
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_controller, nullptr)))
	{
		return E_FAIL;
	}
	//���p�\�ȃR���g���[���[�̗񋓊֐������s
	if (FAILED(g_controller->EnumDevices(DI8DEVCLASS_GAMECTRL, controller_callback_enum_joysticks,
		nullptr, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	if (g_controller_device == nullptr)
	{
		return E_FAIL;
	}
	// �f�o�C�X���W���C�X�e�B�b�N�ɐݒ�
	if (FAILED(g_controller_device->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if (FAILED(g_controller_device->SetCooperativeLevel(
		window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��
	if (FAILED(g_controller_device->EnumObjects(controller_callback_enum_objects,
		nullptr, DIDFT_ALL)))
	{
		return E_FAIL;
	}
	// �f�o�C�X���u�擾�v����
	g_controller_device->Acquire();
	return S_OK;
}
//�R���g���[���[����̓��͂𒲂ׂ�
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
		///g_controller_state.rgdwPOV[0]�ɂ́A�\���{�^���̓���"����"���i�[����Ă���
		///���Ƃ����Ă�����{�^��4�ɐ؂蕪����
		//�܂��͓��̓`�F�b�N
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
//�R���g���[���[�̎g�p�I��
extern void controller_manager_end()
{
	if (g_controller_device)
	{
		g_controller_device->Unacquire();
	}
	input_define_safe_release(g_controller_device);
	input_define_safe_release(g_controller);
}
//�f�o�C�X
extern const LPDIRECTINPUTDEVICE8 controller_device()
{
	return g_controller_device;
}
//�X�e�B�b�N
extern const DIJOYSTATE2 *controller_state()
{
	return &g_controller_state;
}
//�{�^��
extern const input_button_t *controller_button(input_controller_code_t code)
{
	return &g_controller_buttons[(int)(code)];
}
//�\���{�^��
extern const input_button_t *controller_cross(input_controller_cross_code_t code)
{
	return &g_controller_cross[(size_t)(code)];
}
//ZL��ZR�̓��͒l�𑫂����l��Ԃ�
//(ZL�������Ɛ��̕����AZR�ŕ��̕����A�ǂ����������Ă����0�A�͈͂�0.99~-1.00)
extern double controller_ZL_ZR()
{
	return (double)(g_controller_state.lZ) / 100.0;
}
//�X�e�B�b�N���͂̍ő�l
extern double controller_stick_inclination()
{
	return (double)(c_stick_inclination);
}
/*-------------------------------------------------------------------------*/






