
//親ヘッダー
#include "..\public\x_file.h"
//その他のヘッダー
#include <strsafe.h>
#include "x_file_database.h"
#include "x_file_drawer.h"


//xファイル使用開始
extern void x_file_init(IDirect3DDevice9 *device)
{
	//データベースを用意
	x_file_database_init(device);
}

//xファイル使用終了
extern void x_file_end()
{
	//データベースを破棄
	x_file_database_end();
}


//モデル情報をデータベースから取ってくる
extern void x_file_t_load_from_database(x_file_t *strage, x_file_type_t type)
{
	x_file_database_get(strage, type);
}
extern void x_file_t_load_from_database(x_file_t *strage, size_t type)
{
	x_file_database_get(strage, type);
}


//モデルを描画
extern void x_file_t_draw(IDirect3DDevice9 *device, const x_file_t *x_file)
{
	x_file_drawer_draw(device, x_file);
}
