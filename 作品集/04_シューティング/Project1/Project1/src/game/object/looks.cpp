#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "looks.h"
#include "primitive/screen.h"

static const char *filename_list[LooksFileNum] = {
	//�t�@�C���ꗗ�̃R�s�y��
	LooksFilename_Player,
	LooksFilename_Enemy,
	LooksFilename_Bullet,
};

static Looks load_data[LooksFileNum];

static int id_pair_int[(int)(LooksID_Max)];

static void SetNullCharToCRLF(char *str, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '\r' || str[i] == '\n')
		{
			str[i] = '\0';
			break;
		}
	}
}

static void Load(Looks *data, const char *filename)
{
	data->body = nullptr;
	data->length = nullptr;
	data->max_length = 0;
	data->lines = 0;
	FILE *file = nullptr;
	fopen_s(&file, filename, "rb");
	if (file != nullptr)
	{
		char tmp[LooksLineMax];
		//�s���𒲂ׂ�
		int lines = 0;
		while (true)
		{
			if (fgets(tmp, LooksLineMax, file) == nullptr)
			{
				break;
			}
			lines++;
		}
		data->lines = lines;
		//�ʒu�\���q��擪�ɖ߂�
		fseek(file, 0, SEEK_SET);
		//�s���������������m��
		data->body = (char **)malloc(sizeof(char *) * lines);
		data->length = (int *)malloc(sizeof(int) * lines);
		//��s����
		for (int i = 0; i < lines; i++)
		{
			//�ǂݍ���
			fgets(tmp, LooksLineMax, file);
			//���s����������
			SetNullCharToCRLF(tmp, LooksLineMax);
			//�R�s�[
			data->body[i] = _strdup(tmp);
			//�������v�Z
			data->length[i] = (int)(strlen(tmp));
			//�ő�̒������v�Z
			data->max_length = max(data->max_length, data->length[i]);
		}
		fclose(file);
	}
}
static void Destroy(Looks *data)
{
	for (int i = 0; i < data->lines; i++)
	{
		free(data->body[i]);
	}
	free(data->body);
	free(data->length);
}
extern void InitLooks()
{
	for (int i = 0; i < LooksFileNum; i++)
	{
		Load(&load_data[i], filename_list[i]);
		id_pair_int[i + 1] = i;
	}
}
extern void EndLooks()
{
	for (int i = 0; i < LooksFileNum; i++)
	{
		Destroy(&load_data[i]);
	}
}
extern const Looks *GetLooks(LooksID id)
{
	if (id == LooksID_None)
	{
		return nullptr;
	}
	return &load_data[id_pair_int[(int)(id)]];
}
extern Rect CalcLooksRect(const Looks *looks, Vector2D pos)
{
	Rect rect;
	//�����ڂɏ��������ʂ𓾂���悤��int�^�L���X�g
	rect.left = (int)(pos.x);
	rect.top = (int)(pos.y);
	rect.right = rect.left + looks->max_length;
	rect.bottom = rect.top + looks->lines;
	return rect;
}
extern void DrawLooks(const Looks *looks, Vector2D pos)
{
	for (int i = 0; i < looks->lines; i++)
	{
		int x = (int)(pos.x);
		int y = (int)(pos.y + i);
		const char *s = looks->body[i];
		WriteScreen(x, y, s);
	}
}
