//	
//	冬期制作「Magnet Hero」
//	
//	テクスチャを読み込む場所です
//	
//	内部のテーブルに基づいて読み込みを行います
//	
//	読み込んだものを破棄する役割も兼ねてます
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	
#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include "texuture_id.h"


extern void texture_manager_init();

extern IDirect3DTexture9 *texture_manager_get(texture_id_t texture_manager_id);

extern int texture_manager_get_width(texture_id_t texture_manager_id);

extern int texture_manager_get_height(texture_id_t texture_manager_id);
