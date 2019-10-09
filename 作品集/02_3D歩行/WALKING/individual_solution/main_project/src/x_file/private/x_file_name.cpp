
//親ヘッダー
#include "x_file_name.h"
//その他のヘッダー
#include <stdio.h>
#include <stdlib.h>
#include <strsafe.h>
#include <assert.h>


//ファイル名を格納しておく配列
static char g_file_names[x_file_type__max][FILENAME_MAX] = {};


//ファイル名が正しいかチェック
static bool _x_file_is_correct_file_name(const char *file_name, size_t chars_num)
{
	size_t length = strnlen_s(file_name, chars_num);
	if (length < 2)
	{
		//文字数が少なすぎる
		return false;
	}
	if (file_name[0] == '/' && file_name[1] == '/')
	{
		//この行に書かれているのはコメントである
		return false;
	}
	if (file_name[length - 2] != '.' ||
		file_name[length - 1] != 'x')
	{
		//拡張子が".x"じゃない
		return false;
	}
	return true;
}

//xファイルのパスがまとめて書かれたファイルの名前
extern const char *x_file_path_unit()
{
	return "res//txt//path//x_model//file_path.txt";
}

//xファイルのパスを読み込む
//引数にはx_model_file_path_unit()の返り値をそのまま渡す
extern void x_file_path_read(const char *file_name)
{
	//ファイルを開く
	FILE *f = nullptr;
	fopen_s(&f, file_name, "rb");
	if (f != nullptr)
	{
		//最初の一個はnull
		int i = 0;
		g_file_names[i][0] = '\0';
		i++;
		while (!feof(f))
		{
			//ファイルから1行読む
			fgets(g_file_names[i], FILENAME_MAX, f);
			char *found = strpbrk(g_file_names[i], "\r\n");
			if (found)
			{
				*found = '\0';
			}
			//読み込んだものをチェック
			//.xファイルでなければ、読み込み直し
			if (_x_file_is_correct_file_name(g_file_names[i], FILENAME_MAX) == false)
			{
				continue;
			}
			//読み込んだものは.xファイルの名前であるため、そのまま次へ進む
			i++;
			if (i >= x_file_type__max)
			{
				break;
			}
		}
		fclose(f);
	}
}

//タイプに対応したファイル名を返す
extern const char *x_file_path_get(x_file_type_t type)
{
	return x_file_path_get((size_t)(type));
}
extern const char *x_file_path_get(size_t type)
{
	return g_file_names[type];
}

