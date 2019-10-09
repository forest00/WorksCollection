//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	

//�e�w�b�_�[
#include "input_keyboard.h"
//���̑��̃w�b�_�[
#include <dinput.h>
#include "input_button.h"
#include "input_define.h"


/*------------------------------------------------------------------------------*/
//�L�[�{�[�h����
//direct input�̕ϐ�
static LPDIRECTINPUT8 g_keyboard = nullptr;
static LPDIRECTINPUTDEVICE8 g_keyboard_device = nullptr;
//�{�^���̔z��
static unsigned char g_keyboard_keys[256];
static input_button_t g_keyboard_buttons[256];
/*------------------------------------------------------------------------------*/











/*-------------------------------------------------------------------------*/


//�L�[�{�[�h�̎g�p�J�n
extern HRESULT keyboard_manager_init(HWND window_handle)
{
	//�L�[�{�[�h���͂̏������֐�
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_keyboard, nullptr)))
	{
		return E_FAIL;
	}
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(g_keyboard->CreateDevice(GUID_SysKeyboard,
		&g_keyboard_device, nullptr)))
	{
		return E_FAIL;
	}
	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(g_keyboard_device->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if (FAILED(g_keyboard_device->SetCooperativeLevel(
		window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	// �f�o�C�X���u�擾�v����
	g_keyboard_device->Acquire();
	return S_OK;
}
//�L�[�{�[�h�̑S�`�F�b�N
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
//�L�[�{�[�h�g�p�I��
extern void keyboard_manager_end()
{
	if (g_keyboard_device != nullptr)
	{
		g_keyboard_device->Unacquire();
	}
	input_define_safe_release(g_keyboard_device);
	input_define_safe_release(g_keyboard);
}

//�L�[
extern const input_button_t *keyboard_button(int code)
{
	return &g_keyboard_buttons[code];
}
/*-------------------------------------------------------------------------*/










