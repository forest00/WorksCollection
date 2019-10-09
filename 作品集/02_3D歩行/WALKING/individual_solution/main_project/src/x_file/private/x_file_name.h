#pragma once

#include "..\public\x_file_type.h"


//xファイルのパスがまとめて書かれたファイルの名前
extern const char *x_file_path_unit();

//xファイルのパスを読み込む
//引数にはx_model_file_path_unit()の返り値をそのまま渡す
extern void x_file_path_read(const char *file_name);

//タイプに対応したファイル名を返す
extern const char *x_file_path_get(x_file_type_t type);
extern const char *x_file_path_get(size_t type);
