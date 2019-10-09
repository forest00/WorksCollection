//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	

//�e�w�b�_�[
#include "sound.h"
//���̑��̃w�b�_�[
#include "detail\sound_table.h"
#include "..\mylib\mylib.h"
#include "..\amlib\dslib.h"

//�f�o�b�O�p�w�b�_�[
#include <assert.h>


static int g_channel[(size_t)(si_max)] = {};
static sound_index_t g_stream_index = si_none;


static int sound_load(const char *file_name, DWORD flag)
{
	//�t�@�C���̑��݃`�F�b�N
	if (mylib_file_is_exist(file_name) == false)
	{
		//�t�@�C���͑��݂��Ȃ�
		return -1;
	}
	//�t�@�C���͑��݂���̂œǂݍ���
	return DSoundLoadFile((LPSTR)(file_name), -1, flag);
}


//������
extern void sound_init(HWND window_handle)
{
	//dslib�̏�����
	DSoundInit(window_handle, nullptr);
	//�e�[�u����p��
	sound_table_create();
	//�e�[�u���̏������Ɋy�Ȃ�ǂݍ���
	g_channel[0] = -1;
	for (size_t i = 1; i < (size_t)(si_max); i++)
	{
		const sound_table_data_t *data = sound_table_get((sound_index_t)(i));
		DWORD flag = DSLF_FLG_DEFAULT;
		if (data->_stream_play == sp_on)
		{
			flag = DSLF_FLG_STREAM;
		}
		else
		{
			flag = DSLF_FLG_STATIC;
		}
		g_channel[i] = sound_load(data->_file_name, flag);
	}
}

//�I��
extern void sound_end()
{
	//dslib�̉��
	DSoundRelease();
	//�e�[�u����j��
	sound_table_destroy();
}

//�y�Ȃ��Đ�
extern void sound_play(sound_index_t index)
{
	//��{���擾
	const sound_table_data_t *data = sound_table_get(index);
	int channel = g_channel[(size_t)(index)];
	if (data->_stream_play == sp_on)
	{
		g_stream_index = index;
	}
	//���łɍĐ�����Ă��邩�`�F�b�N
	if (DSoundIsStop(channel) == false)
	{
		//�~�߂�
		DSoundStop(channel);
	}
	//�y�ȍĐ�
	DSoundPlay(channel, data->_loop_play == lp_on);
}

//�y�Ȃ��Đ������`�F�b�N
extern bool sound_is_play(sound_index_t index)
{
	//�`�����l���擾
	int channel = g_channel[(size_t)(index)];
	//�y�ȍĐ��`�F�b�N
	return DSoundIsStop(channel) == false;
}

//�X�g���[���Đ����̊y�Ȃ��擾
extern sound_index_t sound_get_playing_stream()
{
	return g_stream_index;
}

//�y�Ȃ��~
extern void sound_stop(sound_index_t index)
{
	//�`�����l���擾
	int channel = g_channel[(size_t)(index)];
	//�y�Ȓ�~
	DSoundStop(channel);
}

