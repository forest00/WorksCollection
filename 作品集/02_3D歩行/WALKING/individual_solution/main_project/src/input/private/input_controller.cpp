


//�e�w�b�_�[
#include "input_controller.h"
//���̑��̃w�b�_�[
#include "..\public\input_controller_code.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\common_def\public\common_def.h"


//�f�o�b�O�p�ꎞ�g�p�w�b�_�[
#include "..\..\font\public\font.h"



/*------------------------------------------------------------------------------*/
//�R���g���[���[����
//direct input�̕ϐ�
static LPDIRECTINPUT8 g_controller = nullptr;
static LPDIRECTINPUTDEVICE8 g_controller_device = nullptr;
static DIJOYSTATE2 g_controller_state = {};
//�{�^���̔z��
static input_button_t g_controller_buttons[input_controller_code__max];
static input_button_t g_controller_cross[input_controller_cross_code__max];
/*------------------------------------------------------------------------------*/





/*------------------------------------------------------------------------------*/
extern const double c_stick_inclination = 100.0;
/*------------------------------------------------------------------------------*/






/*-------------------------------------------------------------------------*/
//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��R�[���o�b�N�֐�
extern BOOL CALLBACK controller_callback_enum_objects(const DIDEVICEOBJECTINSTANCE* didoi, VOID *context)
{
	if (didoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = didoi->dwType;
		diprg.lMin = -c_stick_inclination;
		diprg.lMax = c_stick_inclination;
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
		&g_controller_device, NULL)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}




//�R���g���[���[���͂̏������֐�
extern HRESULT controller_manager_init(HWND window_handle)
{
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_controller, NULL)))
	{
		return E_FAIL;
	}
	//���p�\�ȃR���g���[���[�̗񋓊֐������s
	if (FAILED(g_controller->EnumDevices(DI8DEVCLASS_GAMECTRL, controller_callback_enum_joysticks,
		NULL, DIEDFL_ATTACHEDONLY)))
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
		NULL, DIDFT_ALL)))
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
		for (int i = 0; i < (int)(input_controller_code__max); i++)
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
		/*-------------------------------------------------------------*/
#ifdef _DEBUG
#if 0
		//�f�o�b�O�p����
		{
			int font_pos_y = 0;
			int height = 16;
			static char str[0x100];
			{
				int in = g_controller_state.lARx;
				sprintf_s(str, 0x100, "lARx : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lARy;
				sprintf_s(str, 0x100, "lARy : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lARz;
				sprintf_s(str, 0x100, "lARz : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lAX;
				sprintf_s(str, 0x100, "lAX : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lAY;
				sprintf_s(str, 0x100, "lAY : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lAZ;
				sprintf_s(str, 0x100, "lAZ : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lFRx;
				sprintf_s(str, 0x100, "lFRx : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lFRy;
				sprintf_s(str, 0x100, "lFRy : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lFRz;
				sprintf_s(str, 0x100, "lFRz : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lFX;
				sprintf_s(str, 0x100, "lFX : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lFY;
				sprintf_s(str, 0x100, "lFY : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lFZ;
				sprintf_s(str, 0x100, "lFZ : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lRx;
				sprintf_s(str, 0x100, "lRx : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lRy;
				sprintf_s(str, 0x100, "lRy : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lRz;
				sprintf_s(str, 0x100, "lRz : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lVRx;
				sprintf_s(str, 0x100, "lVRx : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lVRy;
				sprintf_s(str, 0x100, "lVRy : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lVRz;
				sprintf_s(str, 0x100, "lVRz : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lVX;
				sprintf_s(str, 0x100, "lVX : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lVY;
				sprintf_s(str, 0x100, "lVY : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lVZ;
				sprintf_s(str, 0x100, "lVZ : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lX;
				sprintf_s(str, 0x100, "lX : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lY;
				sprintf_s(str, 0x100, "lY : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				int in = g_controller_state.lZ;
				sprintf_s(str, 0x100, "lZ : %d", in);
				font_draw_buffer(str, 0, font_pos_y += height, height);
			}
			{
				font_pos_y += height;
				for (int i = 0; i < 10;i++)
				{
					int in = g_controller_state.rgbButtons[i];
					sprintf_s(str, 0x100, "btn%d:%d,", i, in);
					font_draw_buffer(str, i * 80, font_pos_y, height);
				}
				font_pos_y += height;
				for (int i = 10; i < 20;i++)
				{
					int in = g_controller_state.rgbButtons[i];
					sprintf_s(str, 0x100, "btn[%d]:%d,", i, in);
					font_draw_buffer(str, (i - 10) * 80, font_pos_y, height);
				}
				font_pos_y += height;
			}
			{
				for (int i = 0; i < 4;i++)
				{
					int in = g_controller_state.rgdwPOV[i];
					sprintf_s(str, 0x100, "rgdwPOV[%d]:%d,", i, in);
					font_draw_buffer(str, i * 200, font_pos_y, height);
				}
				font_pos_y += height;
			}
			{
				for (int i = 0; i < 2;i++)
				{
					int in = g_controller_state.rglASlider[i];
					sprintf_s(str, 0x100, "rglASlider[%d]:%d,", i, in);
					font_draw_buffer(str, i * 200, font_pos_y, height);
				}
				font_pos_y += height;
			}
			{
				for (int i = 0; i < 2;i++)
				{
					int in = g_controller_state.rglFSlider[i];
					sprintf_s(str, 0x100, "rglFSlider[%d]:%d,", i, in);
					font_draw_buffer(str, i * 200, font_pos_y, height);
				}
				font_pos_y += height;
			}
			{
				for (int i = 0; i < 2;i++)
				{
					int in = g_controller_state.rglSlider[i];
					sprintf_s(str, 0x100, "rglSlider[%d]:%d,", i, in);
					font_draw_buffer(str, i * 200, font_pos_y, height);
				}
				font_pos_y += height;
			}
			{
				for (int i = 0; i < 2;i++)
				{
					int in = g_controller_state.rglVSlider[i];
					sprintf_s(str, 0x100, "rglVSlider[%d]:%d,", i, in);
					font_draw_buffer(str, i * 200, font_pos_y, height);
				}
				font_pos_y += height;
			}
		}
#endif
#endif
		/*-------------------------------------------------------------*/
	}
}
//�R���g���[���[�̎g�p�I��
extern void controller_manager_end()
{
	if (g_controller_device)
	{
		g_controller_device->Unacquire();
	}
	safe_release(g_controller_device);
	safe_release(g_controller);
}
//�f�o�C�X
extern const LPDIRECTINPUTDEVICE8 *controller_device()
{
	return &g_controller_device;
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
//(ZL�������Ɛ��̕����AZR�ŕ��̕����A�ǂ����������Ă����0�A�͈͂�99~-100)
extern int controller_ZL_ZR()
{
	return g_controller_state.lZ;
}
//�X�e�B�b�N���͂̍ő�l
extern double controller_stick_inclination()
{
	return (double)(c_stick_inclination);
}
/*-------------------------------------------------------------------------*/






