/*------------------------------------------------*/
//                                                
//�����ł́A���܂��܂ȏ������֐��A�X�V�֐����Ăяo���܂�
//���b�Z�[�W�������s���܂�                          
//�܂��A�_�C���N�g3D�̃f�o�C�X����āA�r���[�ϊ��ƃv���W�F�N�V�����ϊ����s���܂�
//                                                
//                                                
//  �쐬��  �X�@�m�u                                
//                                                
//               �ŏI�X�V     2018/11/26/18:32     
/*------------------------------------------------*/



//�e�w�b�_�[
#include "game_system.h"
//���̑��̃w�b�_�[
#include "..\..\math_with_defines\math_with_defines.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\d3d_device\public\d3d_device.h"
#include "..\..\input\public\input.h"
#include "..\..\x_file\public\x_file.h"
#include "..\..\sound\public\sound.h"
#include "..\..\font\public\font.h"
#include "..\..\camera\public\camera.h"
#include "..\..\light\public\light.h"
#include "..\..\x_object\public\x_object.h"



/*-------------------------------------------------------*/
///���[�J���֐���������

//�r���[�ϊ�&�v���W�F�N�V�����ϊ�(�`��O�ɌĂ�)
static void view_and_projection_transformation()
{
	//�r���[�ϊ�
	matrix4x4 matView;
	vector3_t eye_pos;
	vector3_t look_pos;
	vector3_t up_dir;
	camera_get_eye_pos(&eye_pos);
	camera_get_look_pos(&look_pos);
	camera_get_up_dir(&up_dir);
	D3DXMatrixLookAtLH(&matView, &eye_pos, &look_pos, &up_dir);
	d3d_device()->SetTransform(D3DTS_VIEW, &matView);
	//�v���W�F�N�V�����ϊ�
	matrix4x4 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, M_PI_4, 1.0f, 1.0f, 100.0f);
	d3d_device()->SetTransform(D3DTS_PROJECTION, &matProj);
}

///���[�J���֐������܂�
/*-------------------------------------------------------*/


//�V�X�e�������̏�����
extern int game_system_init()
{
	//�E�B���h�E�̐���&�\��
	HWND created_window;
	if (FAILED(lib_d3d_window_init(&created_window, "lib_test", 1280, 720)))
	{
		MessageBox(0, "�E�B���h�E�̏������Ɏ��s���܂���", "", MB_OK);
		UnregisterClass(window_class_name(), GetModuleHandle(nullptr));
		return -1;
	}

	//�_�C���N�g3D�̏�����
	if (FAILED(d3d_init(created_window)))
	{
		MessageBox(0, "�_�C���N�g3D�̏������Ɏ��s���܂���", "", MB_OK);
		return -1;
	}

	//���͂̏�����
	if (input_init(created_window) != 0)
	{
		MessageBox(0, "���͂̏������Ɏ��s���܂���", "", MB_OK);
		return -1;
	}

	//.x�t�@�C������̏�����
	x_object_init(d3d_device());

	//�y�ȃt�@�C������̏�����
	if (sound_manager_init(created_window) != 0)
	{
		MessageBox(0, "�T�E���h�̏������Ɏ��s���܂���", "", MB_OK);
		return -1;
	}

	//�t�H���g�̏�����
	font_init(d3d_device());



	return 0;
}

//���b�Z�[�W����
extern bool process_message()
{
	static MSG _message = {};
	if (_message.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		if (PeekMessage(&_message, nullptr, 0l, 0l, PM_REMOVE))
		{
			TranslateMessage(&_message);
			DispatchMessage(&_message);
		}
		return true;
	}
}

//�V�X�e�������̍X�V
extern void game_system_update()
{
	//���͂̍X�V
	input_update();
}



//�`��̊J�n
extern int game_system_draw_begin()
{
	// Clear the backbuffer and the zbuffer
	//lib_get_d3d_device()
	d3d_device()->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255),
		1.0f,
		0
	);
	// Begin the scene
	if (SUCCEEDED(d3d_device()->BeginScene()))
	{
		//�A���r�G���g�����Z�b�g
		light_set_ambient(d3d_device(), 0xffffffff);
		//�f�B���N�V���i�����̃Z�b�g
		light_set_directional(d3d_device());
		//�r���[�ϊ�&�v���W�F�N�V�����ϊ�
		view_and_projection_transformation();
		return 0;
	}
	else
	{
		return -1;
	}
}



//�`��̏I��(���̊֐��́A�΂ɂȂ�begin�֐����������Ă����Ԃ���Ȃ��ƁA�Ă񂶂�_��)
extern void game_system_draw_end()
{
	// End the scene
	d3d_device()->EndScene();
	// Present the backbuffer contents to the display
	d3d_device()->Present(NULL, NULL, NULL, NULL);
}

//�V�X�e�������̏I��
extern void game_system_end()
{
	//x�t�@�C������̏I��
	x_object_end();

	//�T�E���h����̏I��
	sound_manager_end();

	//�t�H���g�I��
	font_end();


	//...?
	UnregisterClass(window_class_name(), GetModuleHandle(nullptr));
}

