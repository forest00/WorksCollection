#pragma once

#include <Windows.h>
#include "sound_index.h"

//�f�o�b�O���͉�������(���邳������)
#ifdef _DEBUG
#define SOUND_OFF
#endif

//������
extern int sound_manager_init(HWND window_handle);

//�I��
extern void sound_manager_end();

//�y�Ȃ��Đ�
extern void sound_play(sound_index_t index);

