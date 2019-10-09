
//板ポリに張り付けるテクスチャを読み込む場所です
//ついでに破棄もします
//ここで読み込んだテクスチャ以外は、板ポリに張り付けられません


#pragma once
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//テクスチャの種類
enum flatpo_texture_id_t
{
	flatpo_texture_id_none,						//テクスチャなし
	flatpo_texture_id_test_amg_logo,			//テスト用テクスチャ
	//flatpo_texture_id_fafnyls_head,			//ファフニールの頭
	//flatpo_texture_id_fafnyls_tail,			//ファフニールのしっぽ
	//flatpo_texture_id_,						//
	//flatpo_texture_id_,						//
	//flatpo_texture_id_,						//
	flatpo_texture_id_max,						//全テクスチャの数
};





//テクスチャを全部読み込む
extern void flatpo_texture_loader_load_all(IDirect3DDevice9 *device);

//テクスチャを全部破棄
extern void flatpo_texture_loader_destroy_all();

//idに対応するテクスチャを得る
extern LPDIRECT3DTEXTURE9 flatpo_texture_loader_get(flatpo_texture_id_t texture_id);
