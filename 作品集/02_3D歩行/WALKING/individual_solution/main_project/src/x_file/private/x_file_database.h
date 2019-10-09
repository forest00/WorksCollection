#pragma once

#include "..\public\x_file.h"
#include "..\public\x_file_type.h"


//使用するxファイルを全部読み込む
extern void x_file_database_init(IDirect3DDevice9 *device);

//読み込んだxファイルを解放
extern void x_file_database_end();

//読み込み済みのxファイルを取得
extern void x_file_database_get(x_file_t *strage, x_file_type_t type);
extern void x_file_database_get(x_file_t *strage, size_t type);

