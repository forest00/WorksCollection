//	
//	全ての板ポリを管理するファイルです
//	
//	板ポリに関する処理はすべてこのファイルを通して行います
//	

#pragma once

#include <d3dx9.h>

//板ポリの初期化
extern void flatpo_manager_init(IDirect3DDevice9 *device);

//板ポリの終了
extern void flatpo_manager_end();

//板ポリの更新
extern void flatpo_manager_update();

//板ポリの描画
extern void flatpo_manager_draw(IDirect3DDevice9 *device, double aspect);

