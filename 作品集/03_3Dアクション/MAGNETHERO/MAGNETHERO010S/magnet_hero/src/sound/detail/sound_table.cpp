//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	

//�e�w�b�_�[
#include "sound_table.h"
//���̑��̃w�b�_�[
#include <Windows.h>
#include "..\..\char_ptr\char_ptr.h"
#include "..\..\file_data\file_data.h"

static sound_table_data_t g_table[(size_t)(si_max)];

//�e�[�u�������
extern void sound_table_create()
{
	size_t datacount = 0;
	const datum_t *data = file_data_get(fdi_sound_file_database, nullptr);
	for (size_t i = 1; i < (size_t)(si_max); i++)
	{
		clone_string(data[datacount]._String, data[datacount]._StringSize, &g_table[i]._file_name, nullptr);
		g_table[i]._stream_play = (stream_play_t)(data[datacount + 1]._Int == 0);
		g_table[i]._loop_play = (loop_play_t)(data[datacount + 2]._Int == 0);
		datacount += 3;
	}
}

//�e�[�u����j������
extern void sound_table_destroy()
{
	for (size_t i = 1; i < (size_t)(si_max); i++)
	{
		destroy_string(&g_table[i]._file_name);
	}
}

//�C���f�b�N�X��p���ăe�[�u������f�[�^�𓾂�
extern const sound_table_data_t *sound_table_get(sound_index_t index)
{
	return &g_table[(size_t)(index)];
}

