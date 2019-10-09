//親ヘッダー
#include "mylib.h"
//その他のヘッダー
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
// DirectX用旧関数エイリアス定義
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;
#define _CRT_SECURE_NO_DEPRECATE								// 旧形式の関数使用
#endif

/*----------------------------------------------------------------------------------------------------------------------*/
//マクロ定義域

//頂点FVF
#define _mylib_vertex_fvf				(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define _mylib_window_class_name		("mylib_window_class")
/*----------------------------------------------------------------------------------------------------------------------*/
//型定義域

//頂点
struct mylib_vertex_t
{
	D3DXVECTOR3 _positon;
	D3DCOLOR _diffuse;
	D3DXVECTOR2 _uv;
};

//グローバル変数
struct mylib_global_t
{
	//初期化関数に引数として渡されたインスタンスハンドル
	HINSTANCE _instance;
	//作成したウィンドウのハンドル
	HWND _wnd;
	//現在フルスクリーンモードか
	bool _is_full_mode;
	//ウィンドウクラス
	WNDCLASSEX _window_class_ex;
	//メッセージ処理用
	MSG _msg;
	//Direct3D
	IDirect3D9 *_d3d;
	//Direct3Dのデバイス
	IDirect3DDevice9 *_d3d_device;
	//現在のブレンドモード
	mylib_blend_mode_t _blend_mode;
	//バックバッファ保存用
	IDirect3DSurface9 *_back_buffer;
	//Zバッファ保存用
	IDirect3DSurface9 *_z_buffer;
	//一時的に書き込むテクスチャ
	IDirect3DTexture9 *_screen_texture;
	//一時的に書き込むデプスバッファ
	IDirect3DSurface9 *_screen_depth_stencil;
	//Zテストを行うか
	bool _z_check;
	//Zバッファへの書き込みをするか
	bool _z_write;
	//ライティングをするか
	bool _lighting;
	//カリング設定
	D3DCULL _cull_mode;
	//読み込んだテクスチャの数
	int _texture_count;
	//読み込んだテクスチャ
	IDirect3DTexture9 *_textures[_mylib_texture_handle_max];
	//読み込んだテクスチャの詳細情報
	D3DXIMAGE_INFO _img_info[_mylib_texture_handle_max];
	//画面のアスペクト比
	float _aspect;
	//メッセージ処理を行った回数を記録
	time_t _frame_count;
	//MSゴシック文字のデフォルト(基本的にデバッグ用)
	ID3DXFont *_ms_gothic_default;
	//仮想の入力
	virtual_input_t _vi_repeat;
	virtual_input_t _vi_trigger;
	virtual_input_t _vi_release;
};
static mylib_global_t *g = nullptr;
/*----------------------------------------------------------------------------------------------------------------------*/
//ローカル関数域

//グローバル変数の初期化
static void mylib_global_init(HINSTANCE instance)
{
	g = (mylib_global_t*)calloc(1, sizeof(*g));
	g->_instance = instance;
	g->_blend_mode = mbm_none;
}

//グローバル変数の初期化
static void mylib_global_end()
{
	if (g)
	{
		free(g);
		g = nullptr;
	}
}

// ウインドウの移動位置の固定
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

//ウィンドウプロシージャ関数
static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (window_move_lock(hWnd, msg, wParam, lParam))
	{
		return 0;
	}
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄されたとき
		PostQuitMessage(0);
		return 0;
	case WM_PAINT://描画命令が発行されたとき
		ValidateRect(hWnd, nullptr);
		return 0;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE://ESCキーが押されたとき
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_CLOSE://終了するべきタイミング
				  //マウスを解放
		mylib_confine_mouse_cursor(nullptr);
		//メッセージを送信
		SendMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_SIZE://ウィンドウサイズが変更されたとき
	{
		//アスペクト比を計算する
		WORD width = LOWORD(lParam);
		WORD height = HIWORD(lParam);
		//アスペクト比を覚える
		g->_aspect = (float)(width) / (float)(height);
		//マウスの移動を制限しなおす
		RECT window_rect;
		GetWindowRect(hWnd, &window_rect);
		mylib_confine_mouse_cursor(&window_rect);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウクラスの登録をする
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
	//ウィンドウクラスの登録
	RegisterClassEx(&g->_window_class_ex);
}

//ウィンドウクラスの登録解除をする
static void mylib_unregister_window_class()
{
	//ウィンドウクラスの登録解除
	UnregisterClass(_mylib_window_class_name, g->_window_class_ex.hInstance);
}

//ウィンドウを作成する
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
		//デスクトップのサイズ取得
		RECT desktop_rect;
		GetWindowRect(GetDesktopWindow(), &desktop_rect);
		// ウィンドウ全体のサイズ
		RECT window_rect;
		GetWindowRect(*created, &window_rect);
		// クライアント領域のサイズ
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

//Direct3Dの初期化
static HRESULT mylib_d3d_init()
{
	//d3d9の作成
	g->_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	//g->_d3d = Direct3DCreate9(D3D9b_SDK_VERSION);
	if (g->_d3d == nullptr)
	{
		MessageBox(0, "D3D9を作成できません", "", MB_OK);
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
	//ステンシルバッファをオン
	D3DPresentParam.EnableAutoDepthStencil = true;
	D3DPresentParam.AutoDepthStencilFormat = D3DFMT_D16;
	//クリエイト*4
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
			//4回全部失敗した
			return E_FAIL;
		}
	}
	//Zテストをオン
	mylib_set_z_check(true);
	//Zバッファへの書き込みをオン
	mylib_set_z_write(true);
	return S_OK;
}

//Direct3Dの終了
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

//シェーダーを全部初期化
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

//シェーダーを全部終了
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
	//ファイル内グローバル変数の初期化
	mylib_global_init(instance);
	//フルスクリーンにしますか?の答え
	int user_answer = 0;
	//起動時オプションによって分岐
	switch (full_opt)
	{
	case 0:
		user_answer = MessageBox(nullptr, "フルスクリーンにしますか?", "確認", MB_YESNOCANCEL);
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
	//ウィンドウクラスの登録
	mylib_register_window_class(instance, icon_filename);
	//ウィンドウの作成
	g->_is_full_mode = user_answer == IDYES;
	mylib_create_window(instance, window_title, g->_is_full_mode, x, y, width, height, &g->_wnd);
	//ウィンドウを見せる
	ShowWindow(g->_wnd, SW_SHOWDEFAULT);
	UpdateWindow(g->_wnd);//?
	SetFocus(g->_wnd);//フォーカスをセット
	//ダイレクト3Dの初期化
	if (FAILED(mylib_d3d_init()))
	{
		_mylib_error_box("Direct3Dの初期化に失敗しました");
		return -2;
	}
	//入力の初期化
	int input_init_result = input_init(g->_wnd);
	if (input_init_result == -1 || input_init_result == -2)
	{
		_mylib_error_box("入力の初期化に失敗しました");
		return -2;
	}
	//音声再生機能初期化
	sound_init(g->_wnd);
	//シェーダー初期化
	if (mylib_shader_init_all() != 0)
	{
		_mylib_error_box("シェーダーファイルの読み込みに失敗しました");
		return -2;
	}
	//フォントの生成 MSゴシック
	if (FAILED(D3DXCreateFont(
		g->_d3d_device,				//デバイス
		32,							//文字高さ
		0,							//文字幅
		FW_NORMAL,					//フォントスタイル
		1,							//ミップマップのレベル
		false,						//斜体にするかどうか
		SHIFTJIS_CHARSET,			//文字セット
		OUT_TT_ONLY_PRECIS,			//出力精度
		ANTIALIASED_QUALITY,		//出力品質
		FF_DONTCARE,				//フォントピッチとファミリ
		"ＭＳ ゴシック",				//フォント名
		&g->_ms_gothic_default					//フォントポインタ
	)))
	{
		_mylib_error_box("フォント作成に失敗しました");
		return -2;
	}
	//描画用のテクスチャ作成(一度テクスチャにすべて描画した後でバックバッファに描画するため)
	mylib_create_texture_can_render_target(&g->_screen_texture, 0x800, 0x800);
	mylib_create_depth_stencil(&g->_screen_depth_stencil, 0x800, 0x800);
	//バックバッファとZバッファをゲットしておく
	mylib_get_render_target(&g->_back_buffer);
	mylib_get_depth_stencil(&g->_z_buffer);
	return 0;
}

extern void mylib_end()
{
	//シェーダー終わり
	mylib_shader_end_all();
	//入力終わり
	input_end();
	//音声再生機能終わり
	sound_end();
	//MSゴシック解放
	_mylib_safe_release(g->_ms_gothic_default);
	//Direct3Dの終了
	mylib_d3d_end();
	//ウィンドウクラスの登録解除
	mylib_unregister_window_class();
	//グローバル変数のメモリ解放
	mylib_global_end();
}

extern bool mylib_process_message()
{
	//カウントアップ
	g->_frame_count++;
	//シェーダーに時間の情報を渡す	
	simple_shader_set_time(mylib_frame_to_second(mylib_get_frame_count(), 60.0f));
	post_wave_set_now(mylib_frame_to_second(mylib_get_frame_count(), 60.0f));
	int event_num = 0;
	//メッセージループ
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
	//サーフェスを取得
	IDirect3DSurface9 *surface = nullptr;
	texture->GetSurfaceLevel(0, &surface);
	//切り替える
	mylib_set_render_target_surface(surface);
	//サーフェスを解放
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
		//あらかじめ取得しておいたものに切り替え
		mylib_set_render_target_surface(g->_back_buffer);
		mylib_set_depth_stencil(g->_z_buffer);
	}
	else if (target == 1)
	{
		//作成しておいたものに切り替え
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
	//シェーダーを読み込む
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
		//読み込み失敗
		//エラー情報を表示
		mylib_shader_error_message(compile_errors);
		return -1;
	}
	//エラー情報を捨てる
	_mylib_safe_release(compile_errors);
	return 0;
}

extern void mylib_shader_error_message(ID3DXBuffer *compile_errors)
{
	if (compile_errors)
	{
		// コンパイルエラーならエラーメッセージ出力
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
		// ファイルエラーならメッセージ出力
		MessageBox(nullptr, "Can't open file\n", "shader compile error", MB_OK);
	}
}

extern void mylib_load_texture(const char *filename, int *handle, D3DXIMAGE_INFO *image_info, int *error_code)
{
	if (mylib_file_is_exist(filename) == false)
	{
		//ファイルが見つからなかった
		if (error_code)
		{
			//エラーコードを伝える
			*error_code = -1;
		}
		*handle = -1;
		//終わり
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
		//読み込みに失敗した
		if (error_code)
		{
			//エラーコードを伝える
			*error_code = -2;
		}
		*handle = -1;
		return;
	}
	if (image_info)
	{
		*image_info = g->_img_info[index];
	}
	//読み込みできた
	if (error_code)
	{
		//エラーコードは0
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
	//ファイルの存在チェック
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
	D3DXMATRIX matrix_scale;			//スケーリング行列
	D3DXMATRIX matrix_rotation;			//回転行列
	D3DXMATRIX matrix_translation;		//平行移動行列
	D3DXMatrixIdentity(world);
	D3DXMatrixTranslation(&matrix_translation, position->x, position->y, position->z);
	D3DXMatrixRotationYawPitchRoll(&matrix_rotation, rotation->y, rotation->x, rotation->z);
	D3DXMatrixScaling(&matrix_scale, scale->x, scale->y, scale->z);
	//ワールド変換
#if 1
	//ワールド行列＝スケーリング×回転×移動
	* world = matrix_scale * matrix_rotation * matrix_translation;
#else
	D3DXMatrixMultiply(world, world, &matrix_scale);
	D3DXMatrixMultiply(world, world, &matrix_rotation);
	D3DXMatrixMultiply(world, world, &matrix_translation);
#endif
}

extern void mylib_make_billboard_matrix(D3DXMATRIX *out, const D3DXMATRIX *in)
{
	// 回転部分の転置行列作成
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
	//ビュー行列
	D3DXMATRIX view;
	mylib_get_view(&view);
	//プロジェクション行列
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//ビューポート行列
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	//viewport._11 = 1.0f;
	//viewport._22 = 1.0f;
	//viewport._41 = 0.0f;
	//viewport._42 = 0.0f;
	//ワールド座標からスクリーン座標への変換行列
	*vpv = view * projection * viewport;
}

extern void mylib_to_world_pos(D3DXVECTOR3 *world_pos, const D3DXVECTOR3 *screen_pos)
{
#if 1
	//ワールド座標からスクリーン座標への変換行列
	D3DXMATRIX vpv;
	mylib_get_vpv(&vpv);
	//スクリーン座標からワールド座標への変換行列
	D3DXMATRIX inv_vpv;
	D3DXMatrixInverse(&inv_vpv, nullptr, &vpv);
	//変換
	D3DXVec3TransformCoord(world_pos, screen_pos, &inv_vpv);
#endif
#if 0
	D3DXMATRIX vpv;
	D3DXMATRIX inv_vpv;
	//ビュー行列
	D3DXMATRIX view;
	mylib_get_view(&view);
	//プロジェクション行列
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//ビューポート行列
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = 1.0f;
	viewport._22 = 1.0f;
	//ワールド座標からスクリーン座標への変換行列
	vpv = view * projection * viewport;
	//スクリーン座標からワールド座標への変換行列
	D3DXMatrixInverse(&inv_vpv, nullptr, &vpv);
	//変換
	D3DXVec3TransformCoord(world_pos, screen_pos, &inv_vpv);
#endif
#if 0
	//ビュー行列
	D3DXMATRIX view;
	mylib_get_view(&view);
	//プロジェクション行列
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//ビューポート行列
	D3DXMATRIX viewport_mat;
	D3DXMatrixIdentity(&viewport_mat);
	viewport_mat._11 = 0.5f;
	viewport_mat._22 = -0.5f;
	viewport_mat._41 = 0.5f;
	viewport_mat._42 = 0.5f;
	//ビュー行列の逆行列
	D3DXMATRIX inv_view;
	D3DXMatrixInverse(&inv_view, nullptr, &view);
	//プロジェクション行列の逆行列
	D3DXMATRIX inv_projection;
	D3DXMatrixInverse(&inv_projection, nullptr, &projection);
	//ビューポート行列の逆行列
	D3DXMATRIX inv_viewport;
	D3DXMatrixInverse(&inv_viewport, nullptr, &viewport);
	//スクリーン座標からワールド座標への変換行列
	D3DXMATRIX screen_to_world_mat = inv_viewport_mat * inv_projection * inv_view;
	//変換
	D3DXVec3TransformCoord(world_pos, screen_pos, &screen_to_world_mat);
#endif
}

extern void mylib_to_screen_pos(D3DXVECTOR3 *screen_pos, const D3DXVECTOR3 *world_pos)
{
#if 1
	//ワールド座標からスクリーン座標への変換行列
	D3DXMATRIX vpv;
	mylib_get_vpv(&vpv);
	//変換
	D3DXVec3TransformCoord(screen_pos, world_pos, &vpv);
#endif
#if 0
	D3DXMATRIX vpv;
	//ビュー行列
	D3DXMATRIX view;
	mylib_get_view(&view);
	//プロジェクション行列
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//ビューポート行列
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = 1.0f;
	viewport._22 = 1.0f;
	//ワールド座標からスクリーン座標への変換行列
	vpv = view * projection * viewport;
	//変換
	D3DXVec3TransformCoord(screen_pos, world_pos, &vpv);
#endif
#if 0
	//ワールド行列
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	//ビュー行列
	D3DXMATRIX view;
	mylib_get_view(&view);
	//プロジェクション行列
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	//ビューポート行列
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = 0.5f;
	viewport._22 = -0.5f;
	viewport._41 = 0.5f;
	viewport._42 = 0.5f;
	//ワールド座標からスクリーン座標への変換行列
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
	SetRect(&rect, x, y, 0, 0);			//描画位置	
	g->_ms_gothic_default->DrawText(
		nullptr,						//スプライトポインタ(無くても良し)
		string,							//描画文字
		-1,								//文字数(-1で全部)
		&rect,							//描画範囲
		DT_LEFT | DT_NOCLIP,			//フォーマット
		color							//色
	);
}

extern void mylib_draw_ray(const D3DXVECTOR3 *start_position, const D3DXVECTOR3 *vector, DWORD color)
{
	//ライティングの設定を取得&オフに
	bool lighting;
	mylib_get_lighting(&lighting);
	mylib_set_lighting(false);
	//始点と終点を用意
	D3DXVECTOR3 ray_point[2];
	ray_point[0] = *start_position;
	ray_point[1] = ray_point[0] + *vector;
	//FVFを設定
	mylib_set_fvf(D3DFVF_XYZ);
	//ビュー行列とプロジェクション行列
	D3DXMATRIX view;
	D3DXMATRIX projection;
	mylib_get_view(&view);
	mylib_get_projection(&projection);
	//色
	D3DCOLORVALUE diffuse;
	diffuse.a = (float)((color >> 0x18) & 0xff) / (float)(0xff);
	diffuse.r = (float)((color >> 0x10) & 0xff) / (float)(0xff);
	diffuse.g = (float)((color >> 0x08) & 0xff) / (float)(0xff);
	diffuse.b = (float)((color >> 0x00) & 0xff) / (float)(0xff);
	//シェーダーに情報を渡す
	D3DXMATRIX wvp = view * projection;
	D3DXVECTOR4 vec4diffuse = D3DXVECTOR4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	diffuse_only_shader_set_wvp(&wvp);
	diffuse_only_shader_set_diffuse(&vec4diffuse);
	//シェーダーをオン
	diffuse_only_shader_begin_scene(dosp_alpha_blend);
	//描画
	g->_d3d_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, ray_point, sizeof(D3DXVECTOR3));
	//シェーダーオフ
	diffuse_only_shader_end_scene();
	//ライティングの設定を戻す
	mylib_set_lighting(lighting);
}

extern void mylib_draw_sphere(const D3DXVECTOR3 *position, float radius, DWORD color)
{
	//ライティングの設定を取得&オフに
	bool lighting;
	mylib_get_lighting(&lighting);
	mylib_set_lighting(false);
	//メッシュを作成
	ID3DXMesh *creature = nullptr;
	D3DXCreateSphere(g->_d3d_device, radius, (int)(radius * 16.0f), (int)(radius * 8.0f), &creature, nullptr);
	//メッシュを作成できなかった場合は何もしない
	if (creature == nullptr)
	{
		return;
	}
	//ワールド行列
	D3DXMATRIX world = {};
	D3DXMatrixTranslation(&world, position->x, position->y, position->z);
	//ビュー行列とプロジェクション行列
	D3DXMATRIX view;
	D3DXMATRIX projection;
	g->_d3d_device->GetTransform(D3DTS_VIEW, &view);
	g->_d3d_device->GetTransform(D3DTS_PROJECTION, &projection);
	//色
	D3DCOLORVALUE diffuse;
	diffuse.a = (float)((color >> 0x18) & 0xff) / (float)(0xff);
	diffuse.r = (float)((color >> 0x10) & 0xff) / (float)(0xff);
	diffuse.g = (float)((color >> 0x08) & 0xff) / (float)(0xff);
	diffuse.b = (float)((color >> 0x00) & 0xff) / (float)(0xff);
	//シェーダーに情報を渡す
	D3DXMATRIX wvp = world * view * projection;
	D3DXVECTOR4 vec4diffuse = D3DXVECTOR4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	diffuse_only_shader_set_wvp(&wvp);
	diffuse_only_shader_set_diffuse(&vec4diffuse);
	//シェーダーをオン
	diffuse_only_shader_begin_scene(dosp_alpha_blend);
	//描画
	creature->DrawSubset(0);
	//シェーダーオフ
	diffuse_only_shader_end_scene();
	//ライティングの設定を戻す
	mylib_set_lighting(lighting);
	//メッシュは要らなくなるので解放
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
	//頂点情報をまとめる
	mylib_vertex_t vertices[4];
	vertices[0] = { {-1.0f, 1.0f,0.0f},color0,*uv0 };
	vertices[1] = { { 1.0f, 1.0f,0.0f},color1,*uv1 };
	vertices[2] = { {-1.0f,-1.0f,0.0f},color2,*uv2 };
	vertices[3] = { { 1.0f,-1.0f,0.0f},color3,*uv3 };
	//現在のカリング設定取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリング設定変更
	mylib_set_cull_mode(D3DCULL_NONE);
	//シェーダーに情報を渡す
	D3DXMATRIX view;
	D3DXMATRIX projection;
	mylib_get_view(&view);
	mylib_get_projection(&projection);
	D3DXMATRIX wvp = (*world) * view * projection;
	texture_shader_set_wvp(&wvp);
	texture_shader_set_texture(texture);
	//シェーダー起動
	switch (blend_mode)
	{
	case mbm_alpha:
		//アルファブレンド
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//加算ブレンド
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//ブレンドなし
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//描画
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//シェーダー終了
	texture_shader_end_scene();
	//カリング設定を戻す
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
	//頂点情報をまとめる
	mylib_vertex_t vertices[4];
	vertices[0] = { *position0,color0,*uv0 };
	vertices[1] = { *position1,color1,*uv1 };
	vertices[2] = { *position2,color2,*uv2 };
	vertices[3] = { *position3,color3,*uv3 };
	//現在のカリング設定取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリング設定変更
	mylib_set_cull_mode(D3DCULL_NONE);
	//シェーダーに情報を渡す
	D3DXMATRIX wvp;
	D3DXMatrixIdentity(&wvp);
	texture_shader_set_wvp(&wvp);
	texture_shader_set_texture(texture);
	//シェーダー起動
	switch (blend_mode)
	{
	case mbm_alpha:
		//アルファブレンド
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//加算ブレンド
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//ブレンドなし
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//描画
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//シェーダー終了
	texture_shader_end_scene();
	//カリング設定を戻す
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
	//頂点情報をまとめる
	mylib_vertex_t vertices[4];
	vertices[0] = { *position0,color0,*uv0 };
	vertices[1] = { *position1,color1,*uv1 };
	vertices[2] = { *position2,color2,*uv2 };
	vertices[3] = { *position3,color3,*uv3 };
	//現在のカリング設定取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリング設定変更
	mylib_set_cull_mode(D3DCULL_NONE);
	//シェーダーに情報を渡す
	D3DXMATRIX wvp;
	D3DXMatrixIdentity(&wvp);
	post_wave_set_wvp(&wvp);
	post_wave_set_texture(texture);
	post_wave_set_mirage_texture(mirage_texture);
	//シェーダー起動
	post_wave_begin_scene();
	//描画
	mylib_set_fvf(_mylib_vertex_fvf);
	d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//シェーダー終了
	post_wave_end_scene();
	//カリング設定を戻す
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
	//ローカル頂点を用意
	mylib_vertex_t vertices[4];
	vertices[0] = { {-1.0f, 1.0f,0.0f},color,*uv0 };
	vertices[1] = { { 1.0f, 1.0f,0.0f},color,*uv1 };
	vertices[2] = { {-1.0f,-1.0f,0.0f},color,*uv2 };
	vertices[3] = { { 1.0f,-1.0f,0.0f},color,*uv3 };
	//引数からワールド行列作成
	D3DXMATRIX world;
	D3DXVECTOR3 _position = D3DXVECTOR3(pos->x, pos->y, 0.0f);
	D3DXVECTOR3 _rotation = D3DXVECTOR3(0.0f, 0.0f, rot);
	D3DXVECTOR3 _scale = D3DXVECTOR3(scale->x, scale->y, 0.0f);
	mylib_world_transform(&world, &_position, &_rotation, &_scale);
	//現在のカリング設定取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリング設定変更
	mylib_set_cull_mode(D3DCULL_NONE);
	//シェーダーに情報を渡す
	texture_shader_set_wvp(&world);
	texture_shader_set_texture(texture);
	//シェーダー起動
	switch (blend_mode)
	{
	case mbm_alpha:
		//アルファブレンド
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//加算ブレンド
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//ブレンドなし
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//描画
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)(vertices), sizeof(mylib_vertex_t));
	//シェーダー終了
	texture_shader_end_scene();
	//カリング設定を戻す
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
	//ローカル頂点を用意
	mylib_vertex_t vertices[4];
	vertices[0] = { { -1.0f, 1.0f,0.0f },color,*uv0 };
	vertices[1] = { {  1.0f, 1.0f,0.0f },color,*uv1 };
	vertices[2] = { { -1.0f,-1.0f,0.0f },color,*uv2 };
	vertices[3] = { {  1.0f,-1.0f,0.0f },color,*uv3 };
	//インデックス情報を用意
	DWORD indices[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;
	//ビュー行列を取り出す
	D3DXMATRIX view;
	mylib_get_view(&view);
	//ビュー行列の回転を打ち消す
	D3DXMATRIX billboard_matrix;
	mylib_make_billboard_matrix(&billboard_matrix, &view);
	//ワールド行列を作成
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
	D3DXMatrixMultiply(&world, &world, &billboard_matrix);//回転・スケーリングの後&&平行移動の前
	D3DXMatrixMultiply(&world, &world, &translation);
	//現在のカリング設定取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリング設定変更
	mylib_set_cull_mode(D3DCULL_NONE);
	//ライティングの設定を取り出す
	bool lighting;
	mylib_get_lighting(&lighting);
	//ライティングの設定をオフに
	mylib_set_lighting(false);
	//シェーダーに情報を渡す
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	D3DXMATRIX wvp = world * view * projection;
	texture_shader_set_wvp(&wvp);
	texture_shader_set_texture(texture);
	//シェーダー起動
	switch (blend_mode)
	{
	case mbm_alpha:
		//アルファブレンド
		texture_shader_begin_scene(tsp_alpha_blend);
		break;
	case mbm_add:
		//加算ブレンド
		texture_shader_begin_scene(tsp_add_blend);
		break;
	default:
		//ブレンドなし
		texture_shader_begin_scene(tsp_no_blend);
		break;
	}
	//描画
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,					//描画方法
		0,									//開始インデックス
		4,									//頂点数
		2,									//ポリゴン数
		(void*)(indices),					//頂点インデックスデータ
		D3DFMT_INDEX32,						//頂点インデックスフォーマット
		vertices,							//頂点データ
		sizeof(mylib_vertex_t)				//頂点のサイズ
	);
	//シェーダー終了
	texture_shader_end_scene();
	//ライティングの設定を戻す
	mylib_set_lighting(lighting);
	//カリング設定を戻す
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
	//ローカル頂点を用意
	mylib_vertex_t vertices[4];
	vertices[0] = { { -1.0f, 1.0f,0.0f },color,*uv0 };
	vertices[1] = { { 1.0f, 1.0f,0.0f },color,*uv1 };
	vertices[2] = { { -1.0f,-1.0f,0.0f },color,*uv2 };
	vertices[3] = { { 1.0f,-1.0f,0.0f },color,*uv3 };
	//インデックス情報を用意
	DWORD indices[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;
	//ビュー行列を取り出す
	D3DXMATRIX view;
	mylib_get_view(&view);
	//ビュー行列の回転を打ち消す
	D3DXMATRIX billboard_matrix;
	mylib_make_billboard_matrix(&billboard_matrix, &view);
	//ワールド行列を作成
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
	D3DXMatrixMultiply(&world, &world, &billboard_matrix);//回転・スケーリングの後&&平行移動の前
	D3DXMatrixMultiply(&world, &world, &translation);
	//現在のカリング設定取得
	D3DCULL cull_mode;
	mylib_get_cull_mode(&cull_mode);
	//カリング設定変更
	mylib_set_cull_mode(D3DCULL_NONE);
	//ライティングの設定を取り出す
	bool lighting;
	mylib_get_lighting(&lighting);
	//ライティングの設定をオフに
	mylib_set_lighting(false);
	//シェーダーに情報を渡す
	D3DXMATRIX projection;
	mylib_get_projection(&projection);
	D3DXMATRIX wvp = world * view * projection;
	texture_shader2_set_wvp(&wvp);
	texture_shader2_set_texture(texture);
	//シェーダー起動
	texture_shader2_begin_scene();
	//描画
	mylib_set_fvf(_mylib_vertex_fvf);
	g->_d3d_device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,					//描画方法
		0,									//開始インデックス
		4,									//頂点数
		2,									//ポリゴン数
		(void*)(indices),					//頂点インデックスデータ
		D3DFMT_INDEX32,						//頂点インデックスフォーマット
		vertices,							//頂点データ
		sizeof(mylib_vertex_t)				//頂点のサイズ
	);
	//シェーダー終了
	texture_shader2_end_scene();
	//ライティングの設定を戻す
	mylib_set_lighting(lighting);
	//カリング設定を戻す
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
