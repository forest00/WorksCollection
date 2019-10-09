

//親ヘッダー
#include "..\public\my_3d_lib.h"
//その他のヘッダー
#include "..\..\common_def\public\common_def.h"


#if 1
// DirectX用旧関数エイリアス定義
#define _CRT_SECURE_NO_DEPRECATE								// 旧形式の関数使用
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;
#endif



///ファイル内グローバル変数
/*------------------------------*/

//画面(ウィンドウ)のアスペクト比を記録する
static double g_aspect = 1.0f;



/*------------------------------*/



///ローカル関数
/*------------------------------*/
//リソースの解放
static void cleanup_d3d()
{

}



// ウィンドウプロシージャ関数
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



///グローバル関数
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
	//ウィンドウ構造体作成
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

	//ウィンドウ作成
	HWND window_handle = CreateWindow(class_name, title, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, window_class.hInstance, NULL);


	//ビューポートアスペクトの記録
	g_aspect = (double)(width) / (double)(height);

	if (created_window)
	{
		*created_window = window_handle;
	}


	return S_OK;
}


//レイを視認できるようにレイをレンダリングする
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
	//レイのマテリアル設定　（白に設定）
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.a = 255;
	mtrl.Diffuse.r = 255;
	mtrl.Diffuse.g = 255;
	mtrl.Diffuse.b = 255;
	mtrl.Ambient = mtrl.Diffuse;
	device->SetMaterial(&mtrl);
	//レイのレンダリング
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vPnt, sizeof(D3DXVECTOR3));
}


