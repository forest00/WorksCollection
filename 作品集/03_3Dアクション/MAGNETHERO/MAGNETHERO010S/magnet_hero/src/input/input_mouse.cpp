//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	

//�e�w�b�_�[
#include "input_mouse.h"
//���̑��̃w�b�_�[
#include "input_define.h"


/*------------------------------------------------------------------------------*/
//�}�E�X����
//direct input�̕ϐ�
static LPDIRECTINPUT8 g_mouse = nullptr;
static LPDIRECTINPUTDEVICE8 g_mouse_device = nullptr;
static DIMOUSESTATE2 g_mouse_state = {};
//�{�^���̔z��
static input_button_t g_mouse_buttons[(size_t)(imc_max)];
/*------------------------------------------------------------------------------*/











/*-------------------------------------------------------------------------*/
//�}�E�X���͂̏������֐�
extern HRESULT mouse_manager_init(HWND window_handle)
{
	HRESULT ret = S_FALSE;
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬

	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_mouse, nullptr)))
	{
		return E_FAIL;
	}

	if (g_mouse == NULL)
	{
		return E_FAIL;
	}

	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	ret = g_mouse->CreateDevice(GUID_SysMouse, &g_mouse_device, NULL);
	if (FAILED(ret))
	{
		// �f�o�C�X�̍쐬�Ɏ��s
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	ret = g_mouse_device->SetDataFormat(&c_dfDIMouse2);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(ret))
	{
		// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return E_FAIL;
	}

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	ret = g_mouse_device->SetCooperativeLevel(window_handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret))
	{
		// ���[�h�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = g_mouse_device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret))
	{
		// �f�o�C�X�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	// ���͐���J�n
	ret = g_mouse_device->Acquire();
	if (FAILED(ret))
	{
		// ���͐���J�n�Ɏ��s
		return E_FAIL;
	}
	return S_OK;
}
//�}�E�X����̓��͂𒲂ׂ�
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
//�}�E�X�̎g�p�I��
extern void mouse_manager_end()
{
	if (g_mouse_device)
	{
		g_mouse_device->Unacquire();
	}
	input_define_safe_release(g_mouse_device);
	input_define_safe_release(g_mouse);
}
//�ړ��Ƃ�
extern const DIMOUSESTATE2 *mouse_state()
{
	return &g_mouse_state;
}
//�N���b�N
extern const input_button_t *mouse_button(input_mouse_code_t code)
{
	return &g_mouse_buttons[(int)(code)];
}
/*-------------------------------------------------------------------------*/







