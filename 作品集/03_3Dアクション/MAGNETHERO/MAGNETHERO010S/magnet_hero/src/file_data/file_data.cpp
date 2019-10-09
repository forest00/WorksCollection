//親ヘッダー
#include "file_data.h"
//その他のヘッダー
#include <assert.h>


//ファイル名の一覧です
static const char *g_file_names[(size_t)(fdi_max)] = {
	"",
	"res/txt/database/camera_database.txt",
	"res/txt/filename/icon_file_name.txt",
	"res/txt/database/init_state.txt",
	"res/txt/database/player_database.txt",
	"res/txt/filename/player_fbx_file_database.txt",
	"res/txt/filename/sound_file_database.txt",
	"res/txt/filename/sky_sphere_fbx_file.txt",
	"res/txt/filename/texture_file_database.txt",
	"res/txt/database/window_startup_option.txt",
	"res/txt/filename/magnetic_fbx_file.txt",
	"res/txt/filename/stage1_fbx_file_database.txt",
	"res/txt/filename/stage2_fbx_file_database.txt",
	"res/txt/filename/stage3_fbx_file_database.txt",
	"res/txt/filename/stage4_fbx_file_database.txt",
	"res/txt/database/stage1_event.txt",
	"res/txt/database/stage2_event.txt",
	"res/txt/database/stage3_event.txt",
	"res/txt/database/qte_config.txt",
};

//読み込んだデータを格納する変数です
static datum_t *g_data[(size_t)(fdi_max)];
static size_t g_data_num[(size_t)(fdi_max)];

//ファイル名を取得する関数です
extern const char *file_data_name(file_data_id_t id)
{
	assert((fdi_none <= id) && (id < fdi_max));
	return g_file_names[(size_t)(id)];
}

//読み込む関数です
extern void file_data_load_all()
{
	g_data[0] = nullptr;
	g_data_num[0] = 0;
	for (size_t i = 1;i < (size_t)(fdi_max);i++)
	{
		CreateVariableArrayFromFile(g_file_names[i], &g_data[i], &g_data_num[i]);
	}
}

//破棄する関数です
extern void file_data_destroy_all()
{
	for (size_t i = 1;i < (size_t)(fdi_max);i++)
	{
		DestroyVariableArray(&g_data[i], &g_data_num[i]);
	}
}

//読み込んだデータを取得する関数です
extern const datum_t *file_data_get(file_data_id_t id, size_t *data_num)
{
	assert((fdi_none < id) && (id < fdi_max));
	if (data_num)
	{
		*data_num = g_data_num[(size_t)(id)];
	}
	return g_data[(size_t)(id)];
}
