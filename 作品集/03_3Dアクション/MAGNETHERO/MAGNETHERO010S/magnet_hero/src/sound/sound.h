//	
//	�~������uMagnet Hero�v
//	
//	�����Đ����Ǘ�����Ƃ���ł�
//	
//	�܂��א搶����z�z���ꂽ"dslib"���g�p���Ă��܂�
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/07
//	
#pragma once

#include <Windows.h>
#include "detail\sound_index.h"

//������
extern void sound_init(HWND window_handle);

//�I��
extern void sound_end();

//�y�Ȃ��Đ�
extern void sound_play(sound_index_t index);

//�y�Ȃ��Đ������`�F�b�N
extern bool sound_is_play(sound_index_t index);

//�X�g���[���Đ����̊y�Ȃ��擾
extern sound_index_t sound_get_playing_stream();

//�y�Ȃ��~
extern void sound_stop(sound_index_t index);

