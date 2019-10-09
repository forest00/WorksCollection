
//�e�w�b�_�[
#include "..\public\d3d_device.h"
//���̑��̃w�b�_�[
#include "..\..\common_def\public\common_def.h"


//��ʑS�̂̏������p
static LPDIRECT3D9 g_d3d = nullptr;
static IDirect3DDevice9 *g_d3d_device = nullptr;


//D3D�C���^�[�t�F�[�X�ւ̃|�C���^���擾
extern LPDIRECT3D9 d3d()
{
	return g_d3d;
}
//D3D�f�o�C�X���擾
extern IDirect3DDevice9 *d3d_device()
{
	return g_d3d_device;
}



//D3D�̏�����
extern HRESULT d3d_init(HWND window_handle)
{
	//d3d9�̍쐬
	g_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_d3d == nullptr)
	{
		MessageBox(0, "D3D9���쐬�ł��܂���", "", MB_OK);
		return E_FAIL;
	}

	//d3d�f�o�C�X�̍쐬(�\�Ȃ�AT&L���g�p)
	{
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		if (FAILED(g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handle,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp, &g_d3d_device)))
		{
			if (FAILED(g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handle,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_d3d_device)))
			{
				MessageBox(0, "�f�o�C�X�̐����Ɏ��s���܂���", "", MB_OK);
				return E_FAIL;
			}
		}
	}

	//z�o�b�t�@���I��
	g_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
	//�E�B���h�E��������
	ShowWindow(window_handle, SW_SHOWDEFAULT);

	return S_OK;
}

//D3D�̏I��
extern void d3d_end()
{
	safe_release(g_d3d_device);
	safe_release(g_d3d);
}