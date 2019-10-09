#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "looks.h"
#include "primitive/screen.h"

static const char *filename_list[LooksFileNum] = {
	//ファイル一覧のコピペ↓
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
		//行数を調べる
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
		//位置表示子を先頭に戻す
		fseek(file, 0, SEEK_SET);
		//行数分だけメモリ確保
		data->body = (char **)malloc(sizeof(char *) * lines);
		data->length = (int *)malloc(sizeof(int) * lines);
		//一行ずつ
		for (int i = 0; i < lines; i++)
		{
			//読み込む
			fgets(tmp, LooksLineMax, file);
			//改行文字を消す
			SetNullCharToCRLF(tmp, LooksLineMax);
			//コピー
			data->body[i] = _strdup(tmp);
			//長さを計算
			data->length[i] = (int)(strlen(tmp));
			//最大の長さを計算
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
	//見た目に準じた結果を得られるようにint型キャスト
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
