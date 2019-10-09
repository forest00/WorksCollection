//親ヘッダー
#include "icon_file_name.h"
//その他のヘッダー
#include "..\file_data\file_data.h"

static const datum_t *g_data = nullptr;
static size_t g_data_num= 0;


//アイコンのパスを読み込む関数です
static void icon_file_name_load()
{
	if (g_data == nullptr)
	{
		g_data = file_data_get(fdi_icon_file_name, &g_data_num);
	}
}

//アイコンのパスを取得する関数です
extern const char *icon_file_name_get()
{
	icon_file_name_load();
	if (g_data_num)
	{
		return g_data[0]._String;
	}
	return "";
}

