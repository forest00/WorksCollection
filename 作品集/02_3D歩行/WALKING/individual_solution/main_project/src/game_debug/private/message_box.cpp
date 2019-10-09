//�e�w�b�_�[
#include "..\public\message_box.h"
//���̑��̃w�b�_�[
#include <d3dx9.h>
#include <strsafe.h>

//printf�̏����ɏ]�����b�Z�[�W�𐶐��������ƁA���ʂɃ��b�Z�[�W�{�b�N�X���o���܂�
//�����w��ł��܂�
//�����ł�MessageBox���Ăяo���A���̕Ԃ��l�����̂܂ܕԂ��܂�
int message_box(HWND window_handle, long box_type, long beep_type, const char *format, ...)
{
	static char message[0x800];
	message[0] = '\0';
	va_list list;
	va_start(list, format);
	vsnprintf(message, 0x800, format, list);
	va_end(list);
	MessageBeep(beep_type);
	MessageBox(window_handle, message, nullptr, box_type);
}

