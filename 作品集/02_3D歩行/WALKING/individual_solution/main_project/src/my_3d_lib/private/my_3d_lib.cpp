

//�e�w�b�_�[
#include "..\public\my_3d_lib.h"
//���̑��̃w�b�_�[
#include "..\..\common_def\public\common_def.h"


#if 1
// DirectX�p���֐��G�C���A�X��`
#define _CRT_SECURE_NO_DEPRECATE								// ���`���̊֐��g�p
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;
#endif



///�t�@�C�����O���[�o���ϐ�
/*------------------------------*/

//���(�E�B���h�E)�̃A�X�y�N�g����L�^����
static double g_aspect = 1.0f;



/*------------------------------*/



///���[�J���֐�
/*------------------------------*/
//���\�[�X�̉��
static void cleanup_d3d()
{

}



// �E�B���h�E�v���V�[�W���֐�
static LRESULT CALLBACK WndProc(HWND window_handle, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
	{
		int width = lParam & 0xffff;
		int height = (lParam >> 16) & 0xffff;
		g_aspect = width / height;
		break;
	}
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_CLOSE:
		SendMessage(window_handle, WM_DESTROY, 0, 0);
		break;
	default:
		return DefWindowProc(window_handle, iMsg, wParam, lParam);
		break;
	}
	return DefWindowProc(window_handle, iMsg, wParam, lParam);
}



/*------------------------------*/



///�O���[�o���֐�
/*------------------------------*/
extern inline const char *window_class_name()
{
	return "D3D Window struct";
}
extern double lib_get_aspect()
{
	return g_aspect;
}
extern HRESULT lib_d3d_window_init(HWND *created_window, LPCSTR title, int width, int height)
{
	//�E�B���h�E�\���̍쐬
	const char *class_name = window_class_name();
	WNDCLASSEX  window_class;
	window_class.cbSize = sizeof(window_class);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandle(nullptr);
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	//window_class.hCursor = LoadCursor(NULL, IDC_CROSS);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = class_name;
	window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&window_class);

	//�E�B���h�E�쐬
	HWND window_handle = CreateWindow(class_name, title, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, window_class.hInstance, NULL);


	//�r���[�|�[�g�A�X�y�N�g�̋L�^
	g_aspect = (double)(width) / (double)(height);

	if (created_window)
	{
		*created_window = window_handle;
	}


	return S_OK;
}


//���C�����F�ł���悤�Ƀ��C�������_�����O����
void render_ray(IDirect3DDevice9 *device, D3DXVECTOR3 vStart, D3DXVECTOR3 vDir)
{
	device->SetFVF(D3DFVF_XYZ);
	D3DXVECTOR3 vPnt[2];
	vPnt[0] = vStart;
	vPnt[1] = vDir * 100;
	vPnt[1].y = vPnt[0].y;

	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	device->SetTransform(D3DTS_WORLD, &mWorld);
	//���C�̃}�e���A���ݒ�@�i���ɐݒ�j
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.a = 255;
	mtrl.Diffuse.r = 255;
	mtrl.Diffuse.g = 255;
	mtrl.Diffuse.b = 255;
	mtrl.Ambient = mtrl.Diffuse;
	device->SetMaterial(&mtrl);
	//���C�̃����_�����O
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vPnt, sizeof(D3DXVECTOR3));
}


