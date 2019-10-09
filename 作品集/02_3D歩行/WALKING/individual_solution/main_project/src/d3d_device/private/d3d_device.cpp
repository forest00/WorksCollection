
//親ヘッダー
#include "..\public\d3d_device.h"
//その他のヘッダー
#include "..\..\common_def\public\common_def.h"


//画面全体の初期化用
static LPDIRECT3D9 g_d3d = nullptr;
static IDirect3DDevice9 *g_d3d_device = nullptr;


//D3Dインターフェースへのポインタを取得
extern LPDIRECT3D9 d3d()
{
	return g_d3d;
}
//D3Dデバイスを取得
extern IDirect3DDevice9 *d3d_device()
{
	return g_d3d_device;
}



//D3Dの初期化
extern HRESULT d3d_init(HWND window_handle)
{
	//d3d9の作成
	g_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_d3d == nullptr)
	{
		MessageBox(0, "D3D9を作成できません", "", MB_OK);
		return E_FAIL;
	}

	//d3dデバイスの作成(可能なら、T&Lを使用)
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
				MessageBox(0, "デバイスの生成に失敗しました", "", MB_OK);
				return E_FAIL;
			}
		}
	}

	//zバッファをオン
	g_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
	//ウィンドウを見せる
	ShowWindow(window_handle, SW_SHOWDEFAULT);

	return S_OK;
}

//D3Dの終了
extern void d3d_end()
{
	safe_release(g_d3d_device);
	safe_release(g_d3d);
}