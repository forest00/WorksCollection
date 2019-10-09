#pragma once

#include "..\public\x_file_structure.h"

//xファイルからメッシュ情報とテクスチャ情報を読み込む
extern void x_file_loader_load_from_x_file(x_file_t *strage, IDirect3DDevice9 *device, const char *file_name);

//読み込んだモデルを解放
extern void x_file_loader_release(x_file_t *x_file);
