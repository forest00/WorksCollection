//�e�w�b�_�[
#include "char_ptr.h"
//���̑��̃w�b�_�[
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define g_string_buffer_size		0x100
static char g_string_buffer[g_string_buffer_size];

//�t�@�C�����o�C�i���ŊJ���āA�t�@�C�����̏��𕶎���Ɋi�[����
extern void create_string_from_file(const char *file_name, char **string, size_t *string_size)
{
	//�t�@�C�����J��
	FILE *file = nullptr;
	fopen_s(&file, file_name, "rb");
	//�J���Ȃ�������I��
	if (file == nullptr)
	{
		return;
	}
	//�t�@�C���̃T�C�Y�𒲂ׂ�
	fseek(file, 0, SEEK_END);
	size_t file_length = (size_t)ftell(file);
	size_t _string_size = file_length + 1;
	size_t string_bytes = sizeof(char) * _string_size;
	fseek(file, 0, SEEK_SET);
	//�T�C�Y�̕������������m��
	*string = (char*)malloc(string_bytes);
	//�������m�ۂɎ��s������I��
	if (*string == nullptr)
	{
		return;
	}
	//�t�@�C�����̏���ǂݍ���
	fread_s((void*)(*string), string_bytes, sizeof(char), file_length, file);
	(*string)[file_length] = '\0';
	//�T�C�Y��������
	if (string_size)
	{
		*string_size = _string_size;
	}
	//�t�@�C������ďI��
	fclose(file);
}

//�^����ꂽ������̊��S�ȃN���[�������
extern void clone_string(const char *string, size_t string_size, char **cloned, size_t *cloned_size)
{
	//�T�C�Y�𒲂ׂ�
	size_t string_length = strnlen_s(string, string_size);
	size_t _cloned_size = string_length + 1;
	size_t cloned_bytes = sizeof(char) * _cloned_size;
	//�������m��
	*cloned = (char *)malloc(cloned_bytes);
	//�������m�ۂɎ��s������I��
	if (*cloned == nullptr)
	{
		if (cloned_size)
		{
			*cloned_size = 0;
		}
		return;
	}
	//������̓��e���R�s�[
	strncpy_s(*cloned, cloned_bytes, string, string_size);
	//�T�C�Y��������
	if (cloned_size)
	{
		*cloned_size = _cloned_size;
	}
}

//��������������
extern void destroy_string(char **string)
{
	//null���n���ꂽ�牽�����Ȃ�
	if (string == nullptr)
	{
		return;
	}
	//���
	free(*string);
	//null����
	*string = nullptr;
}

//printf�Ɠ��������ɏ]���ĕ�����쐬
extern char *format(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	vsnprintf(g_string_buffer, g_string_buffer_size, format, va);
	va_end(va);
	return g_string_buffer;
}