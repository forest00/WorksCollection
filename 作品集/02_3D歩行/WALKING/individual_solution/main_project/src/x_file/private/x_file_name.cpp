
//�e�w�b�_�[
#include "x_file_name.h"
//���̑��̃w�b�_�[
#include <stdio.h>
#include <stdlib.h>
#include <strsafe.h>
#include <assert.h>


//�t�@�C�������i�[���Ă����z��
static char g_file_names[x_file_type__max][FILENAME_MAX] = {};


//�t�@�C���������������`�F�b�N
static bool _x_file_is_correct_file_name(const char *file_name, size_t chars_num)
{
	size_t length = strnlen_s(file_name, chars_num);
	if (length < 2)
	{
		//�����������Ȃ�����
		return false;
	}
	if (file_name[0] == '/' && file_name[1] == '/')
	{
		//���̍s�ɏ�����Ă���̂̓R�����g�ł���
		return false;
	}
	if (file_name[length - 2] != '.' ||
		file_name[length - 1] != 'x')
	{
		//�g���q��".x"����Ȃ�
		return false;
	}
	return true;
}

//x�t�@�C���̃p�X���܂Ƃ߂ď����ꂽ�t�@�C���̖��O
extern const char *x_file_path_unit()
{
	return "res//txt//path//x_model//file_path.txt";
}

//x�t�@�C���̃p�X��ǂݍ���
//�����ɂ�x_model_file_path_unit()�̕Ԃ�l�����̂܂ܓn��
extern void x_file_path_read(const char *file_name)
{
	//�t�@�C�����J��
	FILE *f = nullptr;
	fopen_s(&f, file_name, "rb");
	if (f != nullptr)
	{
		//�ŏ��̈��null
		int i = 0;
		g_file_names[i][0] = '\0';
		i++;
		while (!feof(f))
		{
			//�t�@�C������1�s�ǂ�
			fgets(g_file_names[i], FILENAME_MAX, f);
			char *found = strpbrk(g_file_names[i], "\r\n");
			if (found)
			{
				*found = '\0';
			}
			//�ǂݍ��񂾂��̂��`�F�b�N
			//.x�t�@�C���łȂ���΁A�ǂݍ��ݒ���
			if (_x_file_is_correct_file_name(g_file_names[i], FILENAME_MAX) == false)
			{
				continue;
			}
			//�ǂݍ��񂾂��̂�.x�t�@�C���̖��O�ł��邽�߁A���̂܂܎��֐i��
			i++;
			if (i >= x_file_type__max)
			{
				break;
			}
		}
		fclose(f);
	}
}

//�^�C�v�ɑΉ������t�@�C������Ԃ�
extern const char *x_file_path_get(x_file_type_t type)
{
	return x_file_path_get((size_t)(type));
}
extern const char *x_file_path_get(size_t type)
{
	return g_file_names[type];
}

