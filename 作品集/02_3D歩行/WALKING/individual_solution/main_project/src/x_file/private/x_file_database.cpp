

//親ヘッダー
#include "x_file_database.h"
//その他のヘッダー
#include "x_file_name.h"
#include "x_file_loader.h"


static x_file_t g_x_model_database[x_file_type__max];




//使用するxファイルを全部読み込む
extern void x_file_database_init(IDirect3DDevice9 *device)
{
	x_file_path_read(x_file_path_unit());
	for (size_t i = 0; i < (size_t)(x_file_type__max); i++)
	{
		x_file_loader_load_from_x_file(&g_x_model_database[i], device, x_file_path_get(i));
	}
}

//読み込んだxファイルを解放
extern void x_file_database_end()
{
	for (size_t i = 0; i < (size_t)(x_file_type__max); i++)
	{
		x_file_loader_release(&g_x_model_database[i]);
	}
}

//読み込み済みのxファイルを取得
extern void x_file_database_get(x_file_t *strage, x_file_type_t type)
{
	*strage = g_x_model_database[(size_t)(type)];
}
extern void x_file_database_get(x_file_t *strage, size_t type)
{
	*strage = g_x_model_database[type];
}
