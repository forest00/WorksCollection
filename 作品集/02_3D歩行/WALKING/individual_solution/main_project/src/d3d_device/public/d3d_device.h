#pragma once


#include <d3dx9.h>
//#include <strsafe.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//D3Dインターフェースへのポインタを取得
extern LPDIRECT3D9 d3d();
//D3Dデバイスを取得
extern IDirect3DDevice9 *d3d_device();



//D3Dの初期化
extern HRESULT d3d_init(HWND window_handle);

//D3Dの終了
extern void d3d_end();
