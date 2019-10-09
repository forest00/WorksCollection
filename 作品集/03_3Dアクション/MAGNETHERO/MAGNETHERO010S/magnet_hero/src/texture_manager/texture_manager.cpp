//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	


//親ヘッダー
#include "texture_manager.h"
//その他のヘッダー
#include <stdio.h>
#include "..\mylib\mylib.h"
#include "..\file_data\file_data.h"


//グローバル変数
struct global_t
{
	//読み込んだテクスチャのハンドル
	int _texture_handles[(size_t)(ti_max)];
	//読み込んだテクスチャのサイズ
	int _size_x[(size_t)(ti_max)];
	int _size_y[(size_t)(ti_max)];
};

static global_t g;

//テクスチャを全部読み込む
extern void texture_manager_init()
{
	//テクスチャファイル名を取得
	const datum_t *data = file_data_get(fdi_texture_file_databse, nullptr);
	//最初のテクスチャは存在しないので-1
	g._texture_handles[0] = -1;
	g._size_x[0] = 0;
	g._size_y[0] = 0;
	//テクスチャの数だけfor分を回す
	for (size_t i = 1; i < (size_t)(ti_max); i++)
	{
		//読み込む
		const char * filename = data[i - 1]._String;
		int error_code;
		D3DXIMAGE_INFO image_info;
		mylib_load_texture(filename, &g._texture_handles[i], &image_info, &error_code);
		if (error_code != 0)
		{
			MessageBox(nullptr, "テクスチャの読み込みに失敗しました", "読み込みエラー", MB_OK);
		}
		g._size_x[i] = (int)(image_info.Width);
		g._size_y[i] = (int)(image_info.Height);
	}
}

extern IDirect3DTexture9 *texture_manager_get(texture_id_t texture_manager_id)
{
	return mylib_get_texture(g._texture_handles[(size_t)(texture_manager_id)], nullptr);
}

extern int texture_manager_get_width(texture_id_t texture_manager_id)
{
	return g._size_x[(size_t)(texture_manager_id)];
}

extern int texture_manager_get_height(texture_id_t texture_manager_id)
{
	return g._size_y[(size_t)(texture_manager_id)];
}