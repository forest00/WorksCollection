#pragma once


#include <d3dx9.h>








//環境光の設定
extern void light_set_ambient(IDirect3DDevice9 *device, int ambient);

//ディレクショナル光の設定
extern void light_set_directional(IDirect3DDevice9 *device);







