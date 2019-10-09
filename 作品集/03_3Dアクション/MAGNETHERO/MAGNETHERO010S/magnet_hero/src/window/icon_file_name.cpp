//�e�w�b�_�[
#include "icon_file_name.h"
//���̑��̃w�b�_�[
#include "..\file_data\file_data.h"

static const datum_t *g_data = nullptr;
static size_t g_data_num= 0;


//�A�C�R���̃p�X��ǂݍ��ފ֐��ł�
static void icon_file_name_load()
{
	if (g_data == nullptr)
	{
		g_data = file_data_get(fdi_icon_file_name, &g_data_num);
	}
}

//�A�C�R���̃p�X���擾����֐��ł�
extern const char *icon_file_name_get()
{
	icon_file_name_load();
	if (g_data_num)
	{
		return g_data[0]._String;
	}
	return "";
}

