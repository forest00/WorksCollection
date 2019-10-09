#pragma once

/*

デバイスの初期化や、xファイルの読み込みなどをサポートするライブラリです

主要な公開関数は、"lib_" というキーワードから始まります

*/


#include <Windows.h>
#include <d3dx9.h>
#include <strsafe.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dxguid.lib")


//?
extern inline const char *window_class_name();


//画面のアスペクト比を取得
extern double lib_get_aspect();
//ウィンドウの生成&初期化
//第一引数に生成したウィンドウのハンドルを格納します
extern HRESULT lib_d3d_window_init(HWND *created_window,LPCSTR title, int width, int height);


//レイを視認できるようにレイをレンダリングする
void render_ray(IDirect3DDevice9 *device, D3DXVECTOR3 vStart, D3DXVECTOR3 vDir);
