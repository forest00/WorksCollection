//親ヘッダー
#include "char_ptr.h"
//その他のヘッダー
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define g_string_buffer_size		0x100
static char g_string_buffer[g_string_buffer_size];

//ファイルをバイナリで開いて、ファイル内の情報を文字列に格納する
extern void create_string_from_file(const char *file_name, char **string, size_t *string_size)
{
	//ファイルを開く
	FILE *file = nullptr;
	fopen_s(&file, file_name, "rb");
	//開けなかったら終了
	if (file == nullptr)
	{
		return;
	}
	//ファイルのサイズを調べる
	fseek(file, 0, SEEK_END);
	size_t file_length = (size_t)ftell(file);
	size_t _string_size = file_length + 1;
	size_t string_bytes = sizeof(char) * _string_size;
	fseek(file, 0, SEEK_SET);
	//サイズの分だけメモリ確保
	*string = (char*)malloc(string_bytes);
	//メモリ確保に失敗したら終了
	if (*string == nullptr)
	{
		return;
	}
	//ファイル内の情報を読み込む
	fread_s((void*)(*string), string_bytes, sizeof(char), file_length, file);
	(*string)[file_length] = '\0';
	//サイズを教える
	if (string_size)
	{
		*string_size = _string_size;
	}
	//ファイルを閉じて終了
	fclose(file);
}

//与えられた文字列の完全なクローンを作る
extern void clone_string(const char *string, size_t string_size, char **cloned, size_t *cloned_size)
{
	//サイズを調べる
	size_t string_length = strnlen_s(string, string_size);
	size_t _cloned_size = string_length + 1;
	size_t cloned_bytes = sizeof(char) * _cloned_size;
	//メモリ確保
	*cloned = (char *)malloc(cloned_bytes);
	//メモリ確保に失敗したら終了
	if (*cloned == nullptr)
	{
		if (cloned_size)
		{
			*cloned_size = 0;
		}
		return;
	}
	//文字列の内容をコピー
	strncpy_s(*cloned, cloned_bytes, string, string_size);
	//サイズを教える
	if (cloned_size)
	{
		*cloned_size = _cloned_size;
	}
}

//文字列を解放する
extern void destroy_string(char **string)
{
	//nullが渡されたら何もしない
	if (string == nullptr)
	{
		return;
	}
	//解放
	free(*string);
	//nullを代入
	*string = nullptr;
}

//printfと同じ書式に従って文字列作成
extern char *format(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	vsnprintf(g_string_buffer, g_string_buffer_size, format, va);
	va_end(va);
	return g_string_buffer;
}