#pragma once
//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2019/01/16
//	
#include "sound_index.h"


enum stream_play_t
{
	sp_on,
	sp_off,
};

enum loop_play_t
{
	lp_on,
	lp_off,
};

struct sound_table_data_t
{
	char *_file_name;					//�t�@�C����
	stream_play_t _stream_play;			//�X�g���[���Đ����邩
	loop_play_t _loop_play;				//���[�v�Đ����邩
};

//�e�[�u�������
extern void sound_table_create();

//�e�[�u����j������
extern void sound_table_destroy();

//�C���f�b�N�X��p���ăe�[�u������f�[�^�𓾂�
extern const sound_table_data_t *sound_table_get(sound_index_t index);

