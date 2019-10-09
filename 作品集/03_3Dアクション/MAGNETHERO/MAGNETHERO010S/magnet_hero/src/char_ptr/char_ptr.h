#pragma once
//char型のポインタを操作する関数が書かれたヘッダーファイルです
//作成者　森　仁志

extern void create_string_from_file(const char *file_name, char **string, size_t *string_size);

//与えられた文字列の完全なクローンを作る
extern void clone_string(const char *string, size_t string_size, char **cloned, size_t *cloned_size);

//文字列を解放する
extern void destroy_string(char **string);

//printfと同じ書式に従って文字列作成
extern char *format(const char *format, ...);
