#pragma once

#include <stdarg.h>
#include <Windows.h>


//printf�̏����ɏ]�����b�Z�[�W�𐶐��������ƁA���ʂɃ��b�Z�[�W�{�b�N�X���o���܂�
//�����w��ł��܂�
//�����ł�MessageBox���Ăяo���A���̕Ԃ��l�����̂܂ܕԂ��܂�
int message_box(HWND window_handle, long box_type, long beep_type, const char *format, ...);


