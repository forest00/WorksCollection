#pragma once

#include <d3dx9.h>

//xファイルを読み込んでデータを格納するための構造体
struct x_file_t
{
	LPD3DXMESH _mesh;						//メッシュ情報
	D3DMATERIAL9 *_materials;				//マテリアル情報
	LPDIRECT3DTEXTURE9 *_textures;			//テクスチャ情報
	DWORD _materials_num;					//総数
};
