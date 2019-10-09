//�e�w�b�_�[
#include "mylib.h"
//���̑��̃w�b�_�[
#include <assert.h>
#include <stdio.h>
#include "..\input\input.h"
#include "..\input\virtual_input.h"
#include "..\sound\sound.h"
#include "..\shader_use\diffuse_only_shader.h"
#include "..\shader_use\texture_shader.h"
#include "..\shader_use\simple_shader.h"
#include "..\shader_use\post_wave.h"

#if 1
// DirectX�p���֐��G�C���A�X��`
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;
#define _CRT_SECURE_NO_DEPRECATE								// ���`���̊֐��g�p
#endif

/*----------------------------------------------------------------------------------------------------------------------*/
//�}�N����`��

//���_FVF
#define _mylib_vertex_fvf				(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define _mylib_window_class_name		("mylib_window_class")
/*----------------------------------------------------------------------------------------------------------------------*/
//�^��`��

//���_
struct mylib_vertex_t
{
	D3DXVECTOR3 _positon;
	D3DCOLOR _diffuse;
	D3DXVECTOR2 _uv;
};

//�O���[�o���ϐ�
struct mylib_global_t
{
	//�������֐��Ɉ����Ƃ��ēn���ꂽ�C���X�^���X�n���h��
	HINSTANCE _instance;
	//�쐬�����E�B���h�E�̃n���h��
	HWND _wnd;
	//���݃t���X�N���[�����[�h��
	bool _is_full_mode;
	//�E�B���h�E�N���X
	WNDCLASSEX _window_class_ex;
	//���b�Z�[�W�����p
	MSG _msg;
	//Direct3D
	IDirect3D9 *_d3d;
	//Direct3D�̃f�o�C�X
	IDirect3DDevice9 *_d3d_device;
	//���݂̃u�����h���[�h
	mylib_blend_mode_t _blend_mode;
	//�o�b�N�o�b�t�@�ۑ��p
	IDirect3DSurface9 *_back_buffer;
	//Z�o�b�t�@�ۑ��p
	IDirect3DSurface9 *_z_buffer;
	//�ꎞ�I�ɏ������ރe�N�X�`��
	IDirect3DTexture9 *_screen_texture;
	//�ꎞ�I�ɏ������ރf�v�X�o�b�t�@
	IDirect3DSurface9 *_screen_depth_stencil;
	//Z�e�X�g���s����
	bool _z_check;
	//Z�o�b�t�@�ւ̏������݂����邩
	bool _z_write;
	//���C�e�B���O�����邩
	bool _lighting;
	//�J�����O�ݒ�
	D3DCULL _cull_mode;
	//�ǂݍ��񂾃e�N�X�`���̐�
	int _texture_count;
	//�ǂݍ��񂾃e�N�X�`��
	IDirect3DTexture9 *_textures[_mylib_texture_handle_max];
	//�ǂݍ��񂾃e�N�X�`���̏ڍ׏��
	D3DXIMAGE_INFO _img_info[_mylib_texture_handle_max];
	//��ʂ̃A�X�y�N�g��
	float _aspect;
	//���b�Z�[�W�������s�����񐔂��L�^
	time_t _frame_count;
	//MS�S�V�b�N�����̃f�t�H���g(��{�I�Ƀf�o�b�O�p)
	ID3DXFont *_ms_gothic_default;
	//���z�̓���
	virtual_input_t _vi_repeat;
	virtual_input_t _vi_trigger;
	virtual_input_t _vi_release;
};
static mylib_global_t *g = nullptr;
/*----------------------------------------------------------------------------------------------------------------------*/
//���[�J���֐���

//�O���[�o���ϐ��̏�����
static void mylib_global_init(HINSTANCE instance)
{
	g = (mylib_global_t*)calloc(1, sizeof(*g));
	g->_instance = instance;
	g->_blend_mode = mbm_none;
}

//�O���[�o���ϐ��̏�����
static void mylib_global_end()
{
	if (g)
	{
		free(g);
		g = nullptr;
	}
}

// �E�C���h�E�̈ړ��ʒu�̌Œ�
static BOOL window_move_lock(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCLBUTTONDOWN)
	{
		if (wParam == HTCAPTION)
		{
			SetForegroundWindow(hWnd);
			return TRUE;
		}
	}
	UNREFERENCED_PARAMETER(lParam);
	return FALSE;
}

//�E�B���h�E�v���V�[�W���֐�
static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (window_move_lock(hWnd, msg, wParam, lParam))
	{
		return 0;
	}
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ�Ƃ�
		PostQuitMessage(0);
		return 0;
	case WM_PAINT://�`�施�߂����s���ꂽ�Ƃ�
		ValidateRect(hWnd, nullptr);
		return 0;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE://ESC�L�[�������ꂽ�Ƃ�
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_CLOSE://�I������ׂ��^�C�~���O
				  //�}�E�X�����
		mylib_confine_mouse_cursor(nullptr);
		//���b�Z�[�W�𑗐M
		SendMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_SIZE://�E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ�
	{
		//�A�X�y�N�g����v�Z����
		WORD width = LOWORD(lParam);
		WORD height = HIWORD(lParam);
		//�A�X�y�N�g����o����
		g->_aspect = (float)(width) / (float)(height);
		//�}�E�X�̈ړ��𐧌����Ȃ���
		RECT window_rect;
		GetWindowRect(hWnd, &window_rect);
		mylib_confine_mouse_cursor(&window_rect);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//�E�B���h�E�N���X�̓o�^������
static void mylib_register_window_class(HINSTANCE instance, const char *icon_filename)
{
	g->_window_class_ex.cbSize = sizeof(WNDCLASSEX);
	g->_window_class_ex.style = CS_CLASSDC;
	g->_window_class_ex.lpfnWndProc = window_procedure;
	g->_window_class_ex.cbClsExtra = 0L;
	g->_window_class_ex.cbWndExtra = 0L;
	g->_window_class_ex.hInstance = instance;//GetModuleHandle(nullptr);
	g->_window_class_ex.hIcon = (HICON)LoadImage(nullptr, icon_filename, IMAGE_ICON, 0, 0, LR_SHARED | LR_LOADFROMFILE);
	g->_window_class_ex.hCursor = nullptr;
	g->_window_class_ex.hbrBackground = nullptr;
	g->_window_class_ex.lpszMenuName = nullptr;
	g->_window_class_ex.lpszClassName = _mylib_window_class_name;
	g->_window_class_ex.hIconSm = g->_window_class_ex.hIcon;
	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&g->_window_class_ex);
}

//�E�B���h�E�N���X�̓o�^����������
static void mylib_unregister_window_class()
{
	//�E�B���h�E�N���X�̓o�^����
	UnregisterClass(_mylib_window_class_name, g->_window_class_ex.hInstance);
}

//�E�B���h�E���쐬����
static void mylib_create_window(HINSTANCE instance, const char *window_title, bool full, int x, int y, int width, int height, HWND *created)
{

	if (full)
	{
		*created = CreateWindow(
			_mylib_window_class_name,
			window_title,
			WS_POPUP,
			0,
			0,
			width,
			height,
			nullptr,
			nullptr,
			instance,
			nullptr
		);
		//�f�X�N�g�b�v�̃T�C�Y�擾
		RECT desktop_rect;
		GetWindowRect(GetDesktopWindow(), &desktop_rect);
		// �E�B���h�E�S�̂̃T�C�Y
		RECT window_rect;
		GetWindowRect(*created, &window_rect);
		// �N���C�A���g�̈�̃T�C�Y
		RECT client_rect;
		GetClientRect(*created, &client_rect);
		int new_width = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left) + desktop_rect.right;
		int new_height = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top) + desktop_rect.bottom;
		SetWindowPos(*created, nullptr, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		DWORD window_style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
		RECT window_rect = { x, y, width, height };
		AdjustWindowRect(&window_rect, window_style, false);
		*created = CreateWindow(
			_mylib_window_class_name,
			window_title,
			window_style,
			window_rect.left,
			window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			GetDesktopWindow(),
			nullptr,
			instance,
			nullptr
		);
	}
}

//Direct3D�̏�����
static HRESULT mylib_d3d_init()
{
	//d3d9�̍쐬
	g->_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	//g->_d3d = Direct3DCreate9(D3D9b_SDK_VERSION);
	if (g->_d3d == nullptr)
	{
		MessageBox(0, "D3D9���쐬�ł��܂���", "", MB_OK);
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS D3DPresentParam;
	ZeroMemory(&D3DPresentParam, sizeof(D3DPresentParam));
	D3DPresentParam.Windowed = true;
	D3DPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPresentParam.BackBufferFormat = D3DFMT_UNKNOWN;
	//D3DPresentParam.BackBufferWidth = 0x800;
	//D3DPresentParam.BackBufferHeight = 0x800;
	D3DPresentParam.BackBufferCount = 1;
	D3DPresentParam.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3DPresentParam.MultiSampleQuality = 0;
	D3DPresentParam.hDeviceWindow = NULL;
	D3DPresentParam.Flags = 0;
	D3DPresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	D3DPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//�X�e���V���o�b�t�@���I��
	D3DPresentParam.EnableAutoDepthStencil = true;
	D3DPresentParam.AutoDepthStencilFormat = D3DFMT_D16;
	//�N���G�C�g*4
	D3DDEVTYPE device_type[4] = {
		D3DDEVTYPE_HAL,
		D3DDEVTYPE_HAL,
		D3DDEVTYPE_REF,
		D3DDEVTYPE_REF,
	};
	long vertex_processing[4] = {
		D3DCREATE_HARDWARE_VERTEXPROCESSING,// | D3DCREATE_MULTITHREADED,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,// | D3DCREATE_MULTITHREADED,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,// | D3DCREATE_MULTITHREADED,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,// | D3DCREATE_MULTITHREADED,
	};
	for (int i = 0;i < 4;i++)
	{
		if (SUCCEEDED(g->_d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			device_type[i],
			g->_wnd,
			vertex_processing[i],
			&D3DPresentParam,
			&g->_d3d_device
		)))
		{
			break;
		}
		else if (i == 4 - 1)
		{
			//4��S�����s����
			return E_FAIL;
		}
	}
	//Z�e�X�g���I��
	mylib_set_z_check(true);
	//Z�o�b�t�@�ւ̏������݂��I��
	mylib_set_z_write(true);
	return S_OK;
}

//Direct3D�̏I��
static void mylib_d3d_end()
{

	if (g->_d3d_device)
	{
		g->_d3d_device->Release();
		g->_d3d_device = nullptr;
	}

	if (g->_d3d)
	{
		g->_d3d->Release();
		g->_d3d = nullptr;
	}
}

//�V�F�[�_�[��S��������
static int mylib_shader_init_all()
{
	if (diffuse_only_shader_init() != 0)
	{
		_mylib_error_box("failed diffuse_only_shader_init()");
		return -1;
	}
	if (texture_shader_init() != 0)
	{
		_mylib_error_box("failed texture_shader_init()");
		return -1;
	}
	if (texture_shader2_init() != 0)
	{
		_mylib_error_box("failed texture_shader2_init()");
		return -1;
	}
	if (simple_shader_init() != 0)
	{
		_mylib_error_box("failed simple_shader_init()");
		return -1;
	}
	if (simple_shader_max_alpha_init() != 0)
	{
		_mylib_error_box("failed simple_shader_max_alpha_init()");
		return -1;
	}
	if (post_wave_init() != 0)
	{
		_mylib_error_box("failed post_wave_init()");
		return -1;
	}
	return 0;
}

//�V�F�[�_�[��S���I��
static void mylib_shader_end_all()
{
	diffuse_only_shader_end();
	texture_shader_end();
	texture_shader2_end();
	simple_shader_end();
	simple_shader_max_alpha_end();
	post_wave_end();
}
/*----------------------------------------------------------------------------------------------------------------------*/
extern int mylib_init(
	HINSTANCE instance,
	int full_opt,
	int x, int y,
	int width,
	int height,
	const char *window_title,
	const char *icon_filename
)
{
	//�t�@�C�����O���[�o���ϐ��̏�����
	mylib_global_init(instance);
	//�t���X�N���[���ɂ��܂���?�̓���
	int user_answer = 0;
	//�N�����I�v�V�����ɂ���ĕ���
	switch (full_opt)
	{
	case 0:
		user_answer = MessageBox(nullptr, "�t���X�N���[���ɂ��܂���?", "�m�F", MB_YESNOCANCEL);
		break;
	case 1:
		user_answer = IDYES;
		break;
	case 2:
		user_answer = IDNO;
		break;
	default:
		user_answer = IDCANCEL;
		break;
	}
	if (user_answer == IDCANCEL)
	{
		return -1;
	}
	//�E�B���h�E�N���X�̓o�^
	mylib_register_window_class(instance, icon_filename);
	//�E�B���h�E�̍쐬
	g->_is_full_mode = user_answer == IDYES;
	mylib_create_window(instance, window_title, g->_is_full_mode, x, y, width, height, &g->_wnd);
	//�E�B���h�E��������
	ShowWindow(g->_wnd, SW_SHOWDEFAULT);
	UpdateWindow(g->_wnd);//?
	SetFocus(g->_wnd);//�t�H�[�J�X���Z�b�g
	//�_�C���N�g3D�̏�����
	if (FAILED(mylib_d3d_init()))
	{
		_mylib_error_box("Direct3D�̏������Ɏ��s���܂���");
		return -2;
	}
	//���͂̏�����
	int input_init_result = input_init(g->_wnd);
	if (input_init_result == -1 || input_init_result == -2)
	{
		_mylib_error_box("���͂̏������Ɏ��s���܂���");
		return -2;
	}
	//�����Đ��@�\������
	sound_init(g->_wnd);
	//�V�F�[�_�[������
	if (mylib_shader_init_all() != 0)
	{
		_mylib_error_box("�V�F�[�_�[�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");
		return -2;
	}
	//�t�H���g�̐��� MS�S�V�b�N
	if (FAILED(D3DXCreateFont(
		g->_d3d_device,				//�f�o�C�X
		32,							//��������
		0,							//������
		FW_NORMAL,					//�t�H���g�X�^�C��
		1,							//�~�b�v�}�b�v�̃��x��
		false,						//�Α̂ɂ��邩�ǂ���
		SHIFTJIS_CHARSET,			//�����Z�b�g
		OUT_TT_ONLY_PRECIS,			//�o�͐��x
		ANTIALIASED_QUALITY,		//�o�͕i��
		FF_DONTCARE,				//�t�H���g�s�b�`�ƃt�@�~��
		"�l�r �S�V�b�N",				//�t�H���g��
		&g->_ms_gothic_default					//�t�H���g�|�C���^
	)))
	{
		_mylib_error_box("�t�H���g�쐬�Ɏ��s���܂���");
		return -2;
	}
	//�`��p�̃e�N�X�`���쐬(��x�e�N�X�`���ɂ��ׂĕ`�悵����Ńo�b�N�o�b�t�@�ɕ`�悷�邽��)
	mylib_create_texture_can_render_target(&g->_screen_texture, 0x800, 0x800);
	mylib_create_depth_stencil(&g->_screen_depth_stencil, 0x800, 0x800);
	//�o�b�N�o�b�t�@��Z�o�b�t�@���Q�b�g���Ă���
	mylib_get_render_target(&g->_back_buffer);
	mylib_get_depth_stencil(&g->_z_buffer);
	return 0;
}

extern void mylib_end()
{
	//�V�F�[�_�[�I���
	mylib_shader_end_all();
	//���͏I���
	input_end();
	//�����Đ��@�\�I���
	sound_end();
	//MS�S�V�b�N���
	_mylib_safe_release(g->_ms_gothic_default);
	//Direct3D�̏I��
	mylib_d3d_end();
	//�E�B���h�E�N���X�̓o�^����
	mylib_unregister_window_class();
	//�O���[�o���ϐ��̃��������
	mylib_global_end();
}

extern bool mylib_process_message()
{
	//�J�E���g�A�b�v
	g->_frame_count++;
	//�V�F�[�_�[�Ɏ��Ԃ̏���n��	
	simple_shader_set_time(mylib_frame_to_second(mylib_get_frame_count(), 60.0f));
	post_wave_set_now(mylib_frame_to_second(mylib_get_frame_count(), 60.0f));
	int event_num = 0;
	//���b�Z�[�W���[�v
	while (PeekMessage(&g->_msg, nullptr, 0, 0, PM_NOREMOVE))
	{
		BOOL get_result = GetMessage(&g->_msg, nullptr, 0, 0);
		if (get_result == 0 || get_result == -1)
		{
			return false;
		}
		TranslateMessage(&g->_msg);
		DispatchMessage(&g->_msg);
		event_num++;
		if (event_num >= 5)
		{
			break;
		}
	}
	return true;
}

extern time_t mylib_get_frame_count()
{
	return g->_frame_count;
}

extern HINSTANCE mylib_get_instance()
{
	return g->_instance;
}

extern HWND mylib_get_window()
{
	return g->_wnd;
}

extern IDirect3D9 *mylib_get_d3d()
{
	return g->_d3d;
}

extern IDirect3DDevice9 *mylib_get_d3d_device()
{
	return g->_d3d_device;
}

extern bool mylib_is_full_mode()
{
	return g->_is_full_mode;
}

extern void mylib_input_update()
{
	input_update();
	virtual_input_update(&g->_vi_repeat, &g->_vi_trigger, &g->_vi_release);
}

extern const virtual_input_t *mylib_get_virtual_input_repeat()
{
	return &g->_vi_repeat;
}

extern const virtual_input_t *mylib_get_virtual_input_trigger()
{
	return &g->_vi_trigger;
}

extern const virtual_input_t *mylib_get_virtual_input_release()
{
	return &g->_vi_release;
}

extern float mylib_get_aspect()
{
	return g->_aspect;
}

extern void mylib_calc_view(D3DXMATRIX *view, const D3DXVECTOR3 *eye, const D3DXVECTOR3 *at, const D3DXVECTOR3 *up)
{
	D3DXMatrixLookAtLH(view, eye, at, up);
}

extern void mylib_set_view(const D3DXMATRIX *view)
{
	g->_d3d_device->SetTransform(D3DTS_VIEW, view);
}

extern void mylib_get_view(D3DXMATRIX *view)
{
	g->_d3d_device->GetTransform(D3DTS_VIEW, view);
}

extern void mylib_calc_projection(D3DXMATRIX *projection, float fov_y, float near_clip, float far_clip)
{
	D3DXMatrixPerspectiveFovLH(projection, fov_y, mylib_get_aspect(), near_clip, far_clip);
}

extern void mylib_set_projection(const D3DXMATRIX *projection)
{
	g->_d3d_device->SetTransform(D3DTS_PROJECTION, projection);
}

extern void mylib_get_projection(D3DXMATRIX *projection)
{
	g->_d3d_device->GetTransform(D3DTS_PROJECTION, projection);
}

extern void mylib_clear(int red, int green, int blue)
{
	g->_d3d_device->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(red, green, blue),
		1.0f,
		0
	);
}

extern int mylib_begin_scene()
{
	if (FAILED(g->_d3d_device->BeginScene()))
	{
		return -1;
	}
	return 0;
}

extern void mylib_end_scene()
{
	g->_d3d_device->EndScene();
}

extern void mylib_present()
{
	g->_d3d_device->Present(nullptr, nullptr, nullptr, nullptr);
}

extern void mylib_create_texture_can_render_target(IDirect3DTexture9 **texture, int width, int height)
{
	g->_d3d_device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, texture, nullptr);
}

extern void mylib_create_depth_stencil(IDirect3DSurface9 **surface, int width, int height)
{
	g->_d3d_device->CreateDepthStencilSurface(width, height, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, surface, nullptr);
}

extern void mylib_set_render_target_surface(IDirect3DSurface9 *surface)
{
	g->_d3d_device->SetRenderTarget(0, surface);
}

extern void mylib_set_render_target_texture(IDirect3DTexture9 *texture)
{
	//�T�[�t�F�X���擾
	IDirect3DSurface9 *surface = nullptr;
	texture->GetSurfaceLevel(0, &surface);
	//�؂�ւ���
	mylib_set_render_target_surface(surface);
	//�T�[�t�F�X�����
	_mylib_safe_release(surface);
}

extern void mylib_set_depth_stencil(IDirect3DSurface9 *surface)
{
	g->_d3d_device->SetDepthStencilSurface(surface);
}

extern void mylib_set_render_target_manage(int target)
{
	if (target == 0)
	{
		//���炩���ߎ擾���Ă��������̂ɐ؂�ւ�
		mylib_set_render_target_surface(g->_back_buffer);
		mylib_set_depth_stencil(g->_z_buffer);
	}
	else if (target == 1)
	{
		//�쐬���Ă��������̂ɐ؂�ւ�
		mylib_set_render_target_texture(g->_screen_texture);
		mylib_set_depth_stencil(g->_screen_depth_stencil);
	}
}

extern void mylib_get_render_target(IDirect3DSurface9 **surface)
{
	g->_d3d_device->GetRenderTarget(0, surface);
}

extern void mylib_get_depth_stencil(IDirect3DSurface9 **surface)
{
	g->_d3d_device->GetDepthStencilSurface(surface);
}

extern IDirect3DTexture9 *mylib_get_rendered_texture()
{
	return g->_screen_texture;
}

extern void mylib_set_fvf(DWORD fvf)
{
	g->_d3d_device->SetFVF(fvf);
}

extern void mylib_set_blend_mode(mylib_blend_mode_t mode)
{
	assert(mode >= mbm_none && mode <= mbm_max);
	g->_blend_mode = mode;
	switch (mode)
	{
	case mbm_none:
		g->_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		g->_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		break;
	case mbm_alpha:
		g->_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		g->_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		g->_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g->_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case mbm_add:
		g->_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		g->_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		g->_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g->_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case mbm_factor:
		g->_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		g->_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		g->_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		g->_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
		break;
	}
}

extern void mylib_get_blend_mode(mylib_blend_mode_t *mode)
{
	assert(mode != nullptr);
	*mode = g->_blend_mode;
}

extern void mylib_set_z_write(bool value)
{
	g->_z_write = value;
	g->_d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, value);
}

extern void mylib_get_z_write(bool *value)
{
	*value = g->_z_write;
}

extern void mylib_set_z_check(bool value)
{
	g->_z_check = value;
	g->_d3d_device->SetRenderState(D3DRS_ZENABLE, value);
}

extern void mylib_get_z_check(bool *value)
{
	*value = g->_z_check;
}

extern void mylib_set_cull_mode(D3DCULL mode)
{
	g->_cull_mode = mode;
	g->_d3d_device->SetRenderState(D3DRS_CULLMODE, (DWORD)(mode));
}

extern void mylib_get_cull_mode(D3DCULL *mode)
{
	*mode = g->_cull_mode;
}

extern void mylib_set_lighting(bool value)
{
	g->_lighting = value;
	g->_d3d_device->SetRenderState(D3DRS_LIGHTING, value);
}

extern void mylib_get_lighting(bool *value)
{
	*value = g->_lighting;
}

extern int mylib_shader_open(const char* filename, ID3DXEffect **shader)
{
	//�V�F�[�_�[��ǂݍ���
	ID3DXBuffer *compile_errors = nullptr;
	if (FAILED(D3DXCreateEffectFromFile(
		g->_d3d_device,
		filename,
		nullptr,
		nullptr,
		0,
		nullptr,
		shader,
		&compile_errors
	)))
	{
		//�ǂݍ��ݎ��s
		//�G���[����\��
		mylib_shader_error_message(compile_errors);
		return -1;
	}
	//�G���[�����̂Ă�
	_mylib_safe_release(compile_errors);
	return 0;
}

extern void mylib_shader_error_message(ID3DXBuffer *compile_errors)
{
	if (compile_errors)
	{
		// �R���p�C���G���[�Ȃ�G���[���b�Z�[�W�o��
		char *error_info = (char *)(compile_errors->GetBufferPointer());
		for (DWORD i = 0; i < compile_errors->GetBufferSize();i++)
		{
			if (error_info[i] == ':')
			{
				error_info[i] = '\n';
			}
		}
		MessageBox(nullptr, error_info, "shader compile error", MB_OK);
	}
	else
	{
		// �t�@�C���G���[�Ȃ烁�b�Z�[�W�o��
		MessageBox(nullptr, "Can't open file\n", "shader compile error", MB_OK);
	}
}

extern void mylib_load_texture(const char *filename, int *handle, D3DXIMAGE_INFO *image_info, int *error_code)
{
	if (mylib_file_is_exist(filename) == false)
	{
		//�t�@�C����������Ȃ�����
		if (error_code)
		{
			//�G���[�R�[�h��`����
			*error_code = -1;
		}
		*handle = -1;
		//�I���
		return;
	}
	int index = g->_texture_count++;
	if (FAILED(D3DXCreateTextureFromFileEx(
		g->_d3d_device,
		filename,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0x00000000,
		&g->_img_info[index],
		nullptr,
		&g->_textures[index]
	)))
	{
		//�ǂݍ��݂Ɏ��s����
		if (error_code)
		{
			//�G���[�R�[�h��`����
			*error_code = -2;
		}
		*handle = -1;
		return;
	}
	if (image_info)
	{
		*image_info = g->_img_info[index];
	}
	//�ǂݍ��݂ł���
	if (error_code)
	{
		//�G���[�R�[�h��0
		*error_code = 0;
	}
	*handle = index;
}

extern IDirect3DTexture9 *mylib_get_texture(int handle, D3DXIMAGE_INFO *image_info)
{
	if (handle < 0 || handle >= _mylib_texture_handle_max)
	{
		return nullptr;
	}
	if (image_info)
	{
		*image_info = g->_img_info[handle];
	}
	return g->_textures[handle];
}

extern void mylib_destroy_texture()
{
	for (int i = 0; i < g->_texture_count; i++)
	{
		_mylib_safe_release(g->_textures[i]);
	}
	g->_texture_count = 0;
}

extern void mylib_hide_mouse_cursor()
{
	while (ShowCursor(false) > 0)
	{

	}
}

extern void mylib_confine_mouse_cursor(RECT *region)
{
	ClipCursor(region);
}

extern bool mylib_file_is_exist(const char *file_name)
{
	//�t�@�C���̑��݃`�F�b�N
	FILE *file = nullptr;
	fopen_s(&file, file_name, "rb");
	if (file == nullptr)
	{
		return false;
	}
	fclose(file);
	return true;
}

extern float mylib_frame_to_second(time_t frame, float fps)
{
	return (float)(frame) / fps;
}

extern void mylib_world_transform(D3DXMATRIX *world, const D3DXVECTOR3 *position, const D3DXVECTOR3 *rotation, const D3DXVECTOR3 *scale)
{
	D3DXMATRIX matrix_scale;			//�X�P�[�����O�s��
	D3DXMATRIX matrix_rotation;			//��]�s��
	D3DXMATRIX matrix_translation;		//���s�ړ��s��
	D3DXMatrixIdentity(world);
	D3DXMatrixTranslation(&matrix_translation, position->x, position->y, position->z);
	D3DXMatrixRotationYawPitchRoll(&matrix_rotation, rotation->y, rotation->x, rotation->z);
	D3DXMatrixScaling(&matrix_scale, scale->x, scale->y, scale->z);
	//���[���h�ϊ�
#if 1
	//���[���h�s�񁁃X�P�[�����O�~��]�~�ړ�
	* world = matrix_scale * matrix_rotation * matrix_translation;
#else
	D3DXMatrixMultiply(world, world, &matrix_scale);
	D3DXMatrixMultiply(world, world, &matrix_rotation);
	D3DXMatrixMultiply(world, world, &matrix_translation);
#endif
}

extern void mylib_make_billboard_matrix(D3DXMATRIX *out, const D3DXMATRIX *in)
{
	// ��]�����̓]�u�s��쐬
	out->_11 = in->_11;
	out->_12 = in->_21;
	out->_13 = in->_31;
	out->_14 = 0.0f;

	out->_21 = in->_12;
	out->_22 = in->_22;
	out->_23 = in->_32;
	out->_24 = 0.0f;

	out->_31 = in->_13;
	out->_32 = in->_23;
	out->_33 = in->_33;
	out->_34 = 0.0f;

	out->_41 = 0.0f;
	out->_42 = 0.0f;
	out->_43 = 0.0f;
	out->_44 = 1.0f;
}

extern void mylib_get_vpv(D3DXMATRIX *vpv)
{
	//�r���[�s��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�v���W�F�N�V�����s��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//�r���[�|�[�g�s��
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	//viewport._11 = 1.0f;
	//viewport._22 = 1.0f;
	//viewport._41 = 0.0f;
	//viewport._42 = 0.0f;
	//���[���h���W����X�N���[�����W�ւ̕ϊ��s��
	*vpv = view * projection * viewport;
}

extern void mylib_to_world_pos(D3DXVECTOR3 *world_pos, const D3DXVECTOR3 *screen_pos)
{
#if 1
	//���[���h���W����X�N���[�����W�ւ̕ϊ��s��
	D3DXMATRIX vpv;
	mylib_get_vpv(&vpv);
	//�X�N���[�����W���烏�[���h���W�ւ̕ϊ��s��
	D3DXMATRIX inv_vpv;
	D3DXMatrixInverse(&inv_vpv, nullptr, &vpv);
	//�ϊ�
	D3DXVec3TransformCoord(world_pos, screen_pos, &inv_vpv);
#endif
#if 0
	D3DXMATRIX vpv;
	D3DXMATRIX inv_vpv;
	//�r���[�s��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�v���W�F�N�V�����s��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//�r���[�|�[�g�s��
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = 1.0f;
	viewport._22 = 1.0f;
	//���[���h���W����X�N���[�����W�ւ̕ϊ��s��
	vpv = view * projection * viewport;
	//�X�N���[�����W���烏�[���h���W�ւ̕ϊ��s��
	D3DXMatrixInverse(&inv_vpv, nullptr, &vpv);
	//�ϊ�
	D3DXVec3TransformCoord(world_pos, screen_pos, &inv_vpv);
#endif
#if 0
	//�r���[�s��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�v���W�F�N�V�����s��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//�r���[�|�[�g�s��
	D3DXMATRIX viewport_mat;
	D3DXMatrixIdentity(&viewport_mat);
	viewport_mat._11 = 0.5f;
	viewport_mat._22 = -0.5f;
	viewport_mat._41 = 0.5f;
	viewport_mat._42 = 0.5f;
	//�r���[�s��̋t�s��
	D3DXMATRIX inv_view;
	D3DXMatrixInverse(&inv_view, nullptr, &view);
	//�v���W�F�N�V�����s��̋t�s��
	D3DXMATRIX inv_projection;
	D3DXMatrixInverse(&inv_projection, nullptr, &projection);
	//�r���[�|�[�g�s��̋t�s��
	D3DXMATRIX inv_viewport;
	D3DXMatrixInverse(&inv_viewport, nullptr, &viewport);
	//�X�N���[�����W���烏�[���h���W�ւ̕ϊ��s��
	D3DXMATRIX screen_to_world_mat = inv_viewport_mat * inv_projection * inv_view;
	//�ϊ�
	D3DXVec3TransformCoord(world_pos, screen_pos, &screen_to_world_mat);
#endif
}

extern void mylib_to_screen_pos(D3DXVECTOR3 *screen_pos, const D3DXVECTOR3 *world_pos)
{
#if 1
	//���[���h���W����X�N���[�����W�ւ̕ϊ��s��
	D3DXMATRIX vpv;
	mylib_get_vpv(&vpv);
	//�ϊ�
	D3DXVec3TransformCoord(screen_pos, world_pos, &vpv);
#endif
#if 0
	D3DXMATRIX vpv;
	//�r���[�s��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�v���W�F�N�V�����s��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//�r���[�|�[�g�s��
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = 1.0f;
	viewport._22 = 1.0f;
	//���[���h���W����X�N���[�����W�ւ̕ϊ��s��
	vpv = view * projection * viewport;
	//�ϊ�
	D3DXVec3TransformCoord(screen_pos, world_pos, &vpv);
#endif
#if 0
	//���[���h�s��
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	//�r���[�s��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�v���W�F�N�V�����s��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//�r���[�|�[�g�s��
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = 0.5f;
	viewport._22 = -0.5f;
	viewport._41 = 0.5f;
	viewport._42 = 0.5f;
	//���[���h���W����X�N���[�����W�ւ̕ϊ��s��
	D3DXMATRIX wvpv;
	wvpv = world * view * projection * viewport;
	D3DXVec3TransformCoord(screen_pos, world_pos, &wvpv);
#endif
}

extern const char *mylib_sprintf_1024(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	static char work[0x401];
	vsnprintf(work, 0x400, format, va);
	va_end(va);
	return work;
}

extern float mylib_now_sec()
{
	return (float)(timeGetTime()) / 1000.0f;
}

extern void mylib_wait_sec(float sec)
{
	float start = mylib_now_sec();
	while ((mylib_now_sec() - start) < sec)
	{
	}
}

extern void mylib_draw_string(int x, int y, const char *string, D3DCOLOR color)
{
	RECT rect;
	SetRect(&rect, x, y, 0, 0);			//�`��ʒu	
	g->_ms_gothic_default->DrawText(
		nullptr,						//�X�v���C�g�|�C���^(�����Ă��ǂ�)
		string,							//�`�敶��
		-1,								//������(-1�őS��)
		&rect,							//�`��͈�
		DT_LEFT | DT_NOCLIP,			//�t�H�[�}�b�g
		color							//�F
	);
}

extern void mylib_draw_ray(const D3DXVECTOR3 *start_position, const D3DXVECTOR3 *vector, DWORD color)
{
	//���C�e�B���O�̐ݒ���擾&�I�t��
	bool lighting;
	mylib_get_lighting(&lighting);
	mylib_set_lighting(false);
	//�n�_�ƏI�_��p��
	D3DXVECTOR3 ray_point[2];
	ray_point[0] = *start_position;
	ray_point[1] = ray_point[0] + *vector;
	//FVF��ݒ�
	mylib_set_fvf(D3DFVF_XYZ);
	//�r���[�s��ƃv���W�F�N�V�����s��
	D3DXMATRIX view;
	D3DXMATRIX projection;
	mylib_get_view(&view);
	mylib_get_projection(&projection);
	//�F
	D3DCOLORVALUE diffuse;
	diffuse.a = (float)((color >> 0x18) & 0xff) / (float)(0xff);
	diffuse.r = (float)((color >> 0x10) & 0xff) / (float)(0xff);
	diffuse.g = (float)((color >> 0x08) & 0xff) / (float)(0xff);
	diffuse.b = (float)((color >> 0x00) & 0xff) / (float)(0xff);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX wvp = view * projection;
	D3DXVECTOR4 vec4diffuse = D3DXVECTOR4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	diffuse_only_shader_set_wvp(&wvp);
	diffuse_only_shader_set_diffuse(&vec4diffuse);
	//�V�F�[�_�[���I��
	diffuse_only_shader_begin_scene(dosp_alpha_blend);
	//�`��
	g->_d3d_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, ray_point, sizeof(D3DXVECTOR3));
	//�V�F�[�_�[�I�t
	diffuse_only_shader_end_scene();
	//���C�e�B���O�̐ݒ��߂�
	mylib_set_lighting(lighting);
}

extern void mylib_draw_sphere(const D3DXVECTOR3 *position, float radius, DWORD color)
{
	//���C�e�B���O�̐ݒ���擾&�I�t��
	bool lighting;
	mylib_get_lighting(&lighting);
	mylib_set_lighting(false);
	//���b�V�����쐬
	ID3DXMesh *creature = nullptr;
	D3DXCreateSphere(g->_d3d_device, radius, (int)(radius * 16.0f), (int)(radius * 8.0f), &creature, nullptr);
	//���b�V�����쐬�ł��Ȃ������ꍇ�͉������Ȃ�
	if (creature == nullptr)
	{
		return;
	}
	//���[���h�s��
	D3DXMATRIX world = {};
	D3DXMatrixTranslation(&world, position->x, position->y, position->z);
	//�r���[�s��ƃv���W�F�N�V�����s��
	D3DXMATRIX view;
	D3DXMATRIX projection;
	g->_d3d_device->GetTransform(D3DTS_VIEW, &view);
	g->_d3d_device->GetTransform(D3DTS_PROJECTION, &projection);
	//�F
	D3DCOLORVALUE diffuse;
	diffuse.a = (float)((color >> 0x18) & 0xff) / (float)(0xff);
	diffuse.r = (float)((color >> 0x10) & 0xff) / (float)(0xff);
	diffuse.g = (float)((color >> 0x08) & 0xff) / (float)(0xff);
	diffuse.b = (float)((color >> 0x00) & 0xff) / (float)(0xff);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX wvp = world * view * projection;
	D3DXVECTOR4 vec4diffuse = D3DXVECTOR4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	diffuse_only_shader_set_wvp(&wvp);
	diffuse_only_shader_set_diffuse(&vec4diffuse);
	//�V�F�[�_�[���I��
	diffuse_only_shader_begin_scene(dosp_alpha_blend);
	//�`��
	creature->DrawSubset(0);
	//�V�F�[�_�[�I�t
	diffuse_only_shader_end_scene();
	//���C�e�B���O�̐ݒ��߂�
	mylib_set_lighting(lighting);
	//���b�V���͗v��Ȃ��Ȃ�̂ŉ��
	creature->Release();
}

extern void mylib_draw_board_3d(
	const D3DXMATRIX *world,
	const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture, mylib_blend_mode_t blend_mode
)
{
	//���_�����܂Ƃ߂�
	mylib_vertex_t vertices[4];
	vertices[0] = { {-1.0f, 1.0f,0.0f},color0,*uv0 };
	vertices[1] = { { 1.0f, 1.0f,0.0f},color1,*uv1 };
	vertices[2] = { {-1.0f,-1.0f,0.0f},color2,*uv2 };
	vertices[3] = { { 1.0f,-1.0f,0.0f},color3,*uv3 };
	//���݂̃J�����O�ݒ�擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�ݒ�ύX
	mylib_set_cull_mode(D3DCULL_NONE);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX view;
	D3DXMATRIX projection;
	mylib_get_view(&view);
	mylib_get_projection(&projection);
	D3DXMATRIX wvp = (*world) * view * projection;
	texture_shader_set_wvp(&wvp);
	texture_shader_set_texture(texture);
	//�V�F�[�_�[�N��
	switch (blend_mode)
	{
	case mbm_alpha:
		//�A���t�@�u�����h
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//���Z�u�����h
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//�u�����h�Ȃ�
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//�`��
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//�V�F�[�_�[�I��
	texture_shader_end_scene();
	//�J�����O�ݒ��߂�
	mylib_set_cull_mode(cull_mode);
}

extern void mylib_draw_board_2d(
	const D3DXVECTOR3 *position0, const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR3 *position1, const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR3 *position2, const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR3 *position3, const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture, mylib_blend_mode_t blend_mode
)
{
	//���_�����܂Ƃ߂�
	mylib_vertex_t vertices[4];
	vertices[0] = { *position0,color0,*uv0 };
	vertices[1] = { *position1,color1,*uv1 };
	vertices[2] = { *position2,color2,*uv2 };
	vertices[3] = { *position3,color3,*uv3 };
	//���݂̃J�����O�ݒ�擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�ݒ�ύX
	mylib_set_cull_mode(D3DCULL_NONE);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX wvp;
	D3DXMatrixIdentity(&wvp);
	texture_shader_set_wvp(&wvp);
	texture_shader_set_texture(texture);
	//�V�F�[�_�[�N��
	switch (blend_mode)
	{
	case mbm_alpha:
		//�A���t�@�u�����h
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//���Z�u�����h
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//�u�����h�Ȃ�
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//�`��
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//�V�F�[�_�[�I��
	texture_shader_end_scene();
	//�J�����O�ݒ��߂�
	mylib_set_cull_mode(cull_mode);
}

extern void mylib_draw_board_2d_wave(IDirect3DTexture9 *mirage_texture,
	const D3DXVECTOR3 *position0, const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR3 *position1, const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR3 *position2, const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR3 *position3, const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture
)
{
	IDirect3DDevice9 *d3d_device = mylib_get_d3d_device();
	//���_�����܂Ƃ߂�
	mylib_vertex_t vertices[4];
	vertices[0] = { *position0,color0,*uv0 };
	vertices[1] = { *position1,color1,*uv1 };
	vertices[2] = { *position2,color2,*uv2 };
	vertices[3] = { *position3,color3,*uv3 };
	//���݂̃J�����O�ݒ�擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�ݒ�ύX
	mylib_set_cull_mode(D3DCULL_NONE);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX wvp;
	D3DXMatrixIdentity(&wvp);
	post_wave_set_wvp(&wvp);
	post_wave_set_texture(texture);
	post_wave_set_mirage_texture(mirage_texture);
	//�V�F�[�_�[�N��
	post_wave_begin_scene();
	//�`��
	mylib_set_fvf(_mylib_vertex_fvf);
	d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//�V�F�[�_�[�I��
	post_wave_end_scene();
	//�J�����O�ݒ��߂�
	mylib_set_cull_mode(cull_mode);
}

extern void mylib_draw_point_sprite(
	const D3DXVECTOR2 *pos,
	float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture,
	mylib_blend_mode_t blend_mode
)
{
	//���[�J�����_��p��
	mylib_vertex_t vertices[4];
	vertices[0] = { {-1.0f, 1.0f,0.0f},color,*uv0 };
	vertices[1] = { { 1.0f, 1.0f,0.0f},color,*uv1 };
	vertices[2] = { {-1.0f,-1.0f,0.0f},color,*uv2 };
	vertices[3] = { { 1.0f,-1.0f,0.0f},color,*uv3 };
	//�������烏�[���h�s��쐬
	D3DXMATRIX world;
	D3DXVECTOR3 _position = D3DXVECTOR3(pos->x, pos->y, 0.0f);
	D3DXVECTOR3 _rotation = D3DXVECTOR3(0.0f, 0.0f, rot);
	D3DXVECTOR3 _scale = D3DXVECTOR3(scale->x, scale->y, 0.0f);
	mylib_world_transform(&world, &_position, &_rotation, &_scale);
	//���݂̃J�����O�ݒ�擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�ݒ�ύX
	mylib_set_cull_mode(D3DCULL_NONE);
	//�V�F�[�_�[�ɏ���n��
	texture_shader_set_wvp(&world);
	texture_shader_set_texture(texture);
	//�V�F�[�_�[�N��
	switch (blend_mode)
	{
	case mbm_alpha:
		//�A���t�@�u�����h
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//���Z�u�����h
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//�u�����h�Ȃ�
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//�`��
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//�V�F�[�_�[�I��
	texture_shader_end_scene();
	//�J�����O�ݒ��߂�
	mylib_set_cull_mode(cull_mode);
}

extern void mylib_draw_billboard(
	const D3DXVECTOR3 *pos,
	const float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture,
	mylib_blend_mode_t blend_mode
)
{
	//���[�J�����_��p��
	mylib_vertex_t vertices[4];
	vertices[0] = { { -1.0f, 1.0f,0.0f },color,*uv0 };
	vertices[1] = { {  1.0f, 1.0f,0.0f },color,*uv1 };
	vertices[2] = { { -1.0f,-1.0f,0.0f },color,*uv2 };
	vertices[3] = { {  1.0f,-1.0f,0.0f },color,*uv3 };
	//�C���f�b�N�X����p��
	DWORD indices[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;
	//�r���[�s������o��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�r���[�s��̉�]��ł�����
	D3DXMATRIX billboard_matrix;
	mylib_make_billboard_matrix(&billboard_matrix, &view);
	//���[���h�s����쐬
	D3DXMATRIX translation;
	D3DXMATRIX rotation;
	D3DXMATRIX scaling;
	D3DXMATRIX world;
	D3DXMatrixTranslation(&translation, pos->x, pos->y, pos->z);
	D3DXMatrixScaling(&scaling, scale->x, scale->y, 0.0f);
	D3DXMatrixRotationZ(&rotation, rot);
	D3DXMatrixIdentity(&world);
	D3DXMatrixMultiply(&world, &world, &rotation);
	D3DXMatrixMultiply(&world, &world, &scaling);
	D3DXMatrixMultiply(&world, &world, &billboard_matrix);//��]�E�X�P�[�����O�̌�&&���s�ړ��̑O
	D3DXMatrixMultiply(&world, &world, &translation);
	//���݂̃J�����O�ݒ�擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�ݒ�ύX
	mylib_set_cull_mode(D3DCULL_NONE);
	//���C�e�B���O�̐ݒ�����o��
	bool lighting;
	mylib_get_lighting(&lighting);
	//���C�e�B���O�̐ݒ���I�t��
	mylib_set_lighting(false);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	D3DXMATRIX wvp = world * view * projection;
	texture_shader_set_wvp(&wvp);
	texture_shader_set_texture(texture);
	//�V�F�[�_�[�N��
	switch (blend_mode)
	{
	case mbm_alpha:
		//�A���t�@�u�����h
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//���Z�u�����h
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//�u�����h�Ȃ�
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//�`��
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,					//�`����@
		0,									//�J�n�C���f�b�N�X
		4,									//���_��
		2,									//�|���S����
		(void*)(indices),					//���_�C���f�b�N�X�f�[�^
		D3DFMT_INDEX32,						//���_�C���f�b�N�X�t�H�[�}�b�g
		vertices,							//���_�f�[�^
		sizeof(mylib_vertex_t)				//���_�̃T�C�Y
	);
	//�V�F�[�_�[�I��
	texture_shader_end_scene();
	//���C�e�B���O�̐ݒ��߂�
	mylib_set_lighting(lighting);
	//�J�����O�ݒ��߂�
	mylib_set_cull_mode(cull_mode);
}

extern void mylib_draw_billboard2(
	const D3DXVECTOR3 *pos,
	const float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture
)
{
	//���[�J�����_��p��
	mylib_vertex_t vertices[4];
	vertices[0] = { { -1.0f, 1.0f,0.0f },color,*uv0 };
	vertices[1] = { { 1.0f, 1.0f,0.0f },color,*uv1 };
	vertices[2] = { { -1.0f,-1.0f,0.0f },color,*uv2 };
	vertices[3] = { { 1.0f,-1.0f,0.0f },color,*uv3 };
	//�C���f�b�N�X����p��
	DWORD indices[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;
	//�r���[�s������o��
	D3DXMATRIX view;
	mylib_get_view(&view);
	//�r���[�s��̉�]��ł�����
	D3DXMATRIX billboard_matrix;
	mylib_make_billboard_matrix(&billboard_matrix, &view);
	//���[���h�s����쐬
	D3DXMATRIX translation;
	D3DXMATRIX rotation;
	D3DXMATRIX scaling;
	D3DXMATRIX world;
	D3DXMatrixTranslation(&translation, pos->x, pos->y, pos->z);
	D3DXMatrixScaling(&scaling, scale->x, scale->y, 0.0f);
	D3DXMatrixRotationZ(&rotation, rot);
	D3DXMatrixIdentity(&world);
	D3DXMatrixMultiply(&world, &world, &rotation);
	D3DXMatrixMultiply(&world, &world, &scaling);
	D3DXMatrixMultiply(&world, &world, &billboard_matrix);//��]�E�X�P�[�����O�̌�&&���s�ړ��̑O
	D3DXMatrixMultiply(&world, &world, &translation);
	//���݂̃J�����O�ݒ�擾
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//�J�����O�ݒ�ύX
	mylib_set_cull_mode(D3DCULL_NONE);
	//���C�e�B���O�̐ݒ�����o��
	bool lighting;
	mylib_get_lighting(&lighting);
	//���C�e�B���O�̐ݒ���I�t��
	mylib_set_lighting(false);
	//�V�F�[�_�[�ɏ���n��
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	D3DXMATRIX wvp = world * view * projection;
	texture_shader2_set_wvp(&wvp);
	texture_shader2_set_texture(texture);
	//�V�F�[�_�[�N��
	texture_shader2_begin_scene();
	//�`��
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,					//�`����@
		0,									//�J�n�C���f�b�N�X
		4,									//���_��
		2,									//�|���S����
		(void*)(indices),					//���_�C���f�b�N�X�f�[�^
		D3DFMT_INDEX32,						//���_�C���f�b�N�X�t�H�[�}�b�g
		vertices,							//���_�f�[�^
		sizeof(mylib_vertex_t)				//���_�̃T�C�Y
	);
	//�V�F�[�_�[�I��
	texture_shader2_end_scene();
	//���C�e�B���O�̐ݒ��߂�
	mylib_set_lighting(lighting);
	//�J�����O�ݒ��߂�
	mylib_set_cull_mode(cull_mode);
}

extern void mylib_error_box(const char *file, int line, const char *message, const char *title, UINT mb_mode)
{
	static char message_buf[0x1000];
	sprintf_s(message_buf, 0x1000, "file : %s\nline : %d\n\n%s\n", file, line, message);
	MessageBox(nullptr, message_buf, title, mb_mode);
}

static char *mylib_debug_string_memory(int index)
{
	if (index < 0 || index >= 0x100)
	{
		return nullptr;
	}
	static char mem[0x100][0x400];
	return mem[index];
}

static int *mylib_debug_string_count()
{
	static int count = 0;
	return &count;
}

extern void mylib_register_debug_string(const char *format, ...)
{
	int *c = mylib_debug_string_count();
	char *m = mylib_debug_string_memory(*c);
	if (m == nullptr)
	{
		return;
	}
	va_list va;
	va_start(va, format);
	vsnprintf(m, 0x400, format, va);
	va_end(va);
	(*c)++;
}

extern void mylib_output_debug_string()
{
	int *c = mylib_debug_string_count();
	for (int i = 0; i < (*c);i++)
	{
		mylib_draw_string(0 + 0, i * 32 + 0, mylib_debug_string_memory(i), D3DCOLOR_XRGB(0, 0, 0));
		mylib_draw_string(0 + 2, i * 32 + 2, mylib_debug_string_memory(i), D3DCOLOR_XRGB(0, 0, 0));
		mylib_draw_string(0 + 1, i * 32 + 1, mylib_debug_string_memory(i), D3DCOLOR_XRGB(255, 255, 255));
	}
	(*c) = 0;
}
/*----------------------------------------------------------------------------------------------------------------------*/
