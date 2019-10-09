/*------------------------------------------------*/
//                                                
//ここでは、さまざまな初期化関数、更新関数を呼び出します
//メッセージ処理も行います                          
//また、ダイレクト3Dのデバイスを介して、ビュー変換とプロジェクション変換も行います
//                                                
//                                                
//  作成者  森　仁志                                
//                                                
//               最終更新     2018/11/26/18:32     
/*------------------------------------------------*/



//親ヘッダー
#include "game_system.h"
//その他のヘッダー
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
///ローカル関数ここから

//ビュー変換&プロジェクション変換(描画前に呼ぶ)
static void view_and_projection_transformation()
{
	//ビュー変換
	matrix4x4 matView;
	vector3_t eye_pos;
	vector3_t look_pos;
	vector3_t up_dir;
	camera_get_eye_pos(&eye_pos);
	camera_get_look_pos(&look_pos);
	camera_get_up_dir(&up_dir);
	D3DXMatrixLookAtLH(&matView, &eye_pos, &look_pos, &up_dir);
	d3d_device()->SetTransform(D3DTS_VIEW, &matView);
	//プロジェクション変換
	matrix4x4 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, M_PI_4, 1.0f, 1.0f, 100.0f);
	d3d_device()->SetTransform(D3DTS_PROJECTION, &matProj);
}

///ローカル関数ここまで
/*-------------------------------------------------------*/


//システム部分の初期化
extern int game_system_init()
{
	//ウィンドウの生成&表示
	HWND created_window;
	if (FAILED(lib_d3d_window_init(&created_window, "lib_test", 1280, 720)))
	{
		MessageBox(0, "ウィンドウの初期化に失敗しました", "", MB_OK);
		UnregisterClass(window_class_name(), GetModuleHandle(nullptr));
		return -1;
	}

	//ダイレクト3Dの初期化
	if (FAILED(d3d_init(created_window)))
	{
		MessageBox(0, "ダイレクト3Dの初期化に失敗しました", "", MB_OK);
		return -1;
	}

	//入力の初期化
	if (input_init(created_window) != 0)
	{
		MessageBox(0, "入力の初期化に失敗しました", "", MB_OK);
		return -1;
	}

	//.xファイル周りの初期化
	x_object_init(d3d_device());

	//楽曲ファイル周りの初期化
	if (sound_manager_init(created_window) != 0)
	{
		MessageBox(0, "サウンドの初期化に失敗しました", "", MB_OK);
		return -1;
	}

	//フォントの初期化
	font_init(d3d_device());



	return 0;
}

//メッセージ処理
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

//システム部分の更新
extern void game_system_update()
{
	//入力の更新
	input_update();
}



//描画の開始
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
		//アンビエント光をセット
		light_set_ambient(d3d_device(), 0xffffffff);
		//ディレクショナル光のセット
		light_set_directional(d3d_device());
		//ビュー変換&プロジェクション変換
		view_and_projection_transformation();
		return 0;
	}
	else
	{
		return -1;
	}
}



//描画の終了(この関数は、対になるbegin関数が成功している状態じゃないと、呼んじゃダメ)
extern void game_system_draw_end()
{
	// End the scene
	d3d_device()->EndScene();
	// Present the backbuffer contents to the display
	d3d_device()->Present(NULL, NULL, NULL, NULL);
}

//システム部分の終了
extern void game_system_end()
{
	//xファイル周りの終了
	x_object_end();

	//サウンド周りの終了
	sound_manager_end();

	//フォント終了
	font_end();


	//...?
	UnregisterClass(window_class_name(), GetModuleHandle(nullptr));
}

