//�e�w�b�_�[
#include "file_data.h"
//���̑��̃w�b�_�[
#include <assert.h>


//�t�@�C�����̈ꗗ�ł�
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

//�ǂݍ��񂾃f�[�^���i�[����ϐ��ł�
static datum_t *g_data[(size_t)(fdi_max)];
static size_t g_data_num[(size_t)(fdi_max)];

//�t�@�C�������擾����֐��ł�
extern const char *file_data_name(file_data_id_t id)
{
	assert((fdi_none <= id) && (id < fdi_max));
	return g_file_names[(size_t)(id)];
}

//�ǂݍ��ފ֐��ł�
extern void file_data_load_all()
{
	g_data[0] = nullptr;
	g_data_num[0] = 0;
	for (size_t i = 1;i < (size_t)(fdi_max);i++)
	{
		CreateVariableArrayFromFile(g_file_names[i], &g_data[i], &g_data_num[i]);
	}
}

//�j������֐��ł�
extern void file_data_destroy_all()
{
	for (size_t i = 1;i < (size_t)(fdi_max);i++)
	{
		DestroyVariableArray(&g_data[i], &g_data_num[i]);
	}
}

//�ǂݍ��񂾃f�[�^���擾����֐��ł�
extern const datum_t *file_data_get(file_data_id_t id, size_t *data_num)
{
	assert((fdi_none < id) && (id < fdi_max));
	if (data_num)
	{
		*data_num = g_data_num[(size_t)(id)];
	}
	return g_data[(size_t)(id)];
}
