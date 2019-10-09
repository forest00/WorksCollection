

//�e�w�b�_�[
#include "x_file_database.h"
//���̑��̃w�b�_�[
#include "x_file_name.h"
#include "x_file_loader.h"


static x_file_t g_x_model_database[x_file_type__max];




//�g�p����x�t�@�C����S���ǂݍ���
extern void x_file_database_init(IDirect3DDevice9 *device)
{
	x_file_path_read(x_file_path_unit());
	for (size_t i = 0; i < (size_t)(x_file_type__max); i++)
	{
		x_file_loader_load_from_x_file(&g_x_model_database[i], device, x_file_path_get(i));
	}
}

//�ǂݍ���x�t�@�C�������
extern void x_file_database_end()
{
	for (size_t i = 0; i < (size_t)(x_file_type__max); i++)
	{
		x_file_loader_release(&g_x_model_database[i]);
	}
}

//�ǂݍ��ݍς݂�x�t�@�C�����擾
extern void x_file_database_get(x_file_t *strage, x_file_type_t type)
{
	*strage = g_x_model_database[(size_t)(type)];
}
extern void x_file_database_get(x_file_t *strage, size_t type)
{
	*strage = g_x_model_database[type];
}
