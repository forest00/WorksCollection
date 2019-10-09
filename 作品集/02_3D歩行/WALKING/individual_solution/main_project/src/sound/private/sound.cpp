
//親ヘッダー
#include "..\public\sound.h"
//その他のヘッダー
#include <assert.h>
#include "..\public\sound_index.h"
#include "..\private\dslib.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\file_path_provider\public\file_path_provider.h"
#include "..\..\calc\public\calc.h"
#include "..\..\common_def\public\common_def.h"



//楽曲ファイル名と再生方法のデータベース構造体
struct load_data_t
{
	char *file_name;
	DWORD flag;
};

//楽曲ファイルを読み込むときに必要なデータ
static load_data_t g_load_database[(int)(sound_index__max)] = {};

//現在ストリーム再生されている楽曲のindex
static sound_index_t g_playing_stream_music = sound_index__none;

//楽曲のハンドル
static int g_handles[(int)(sound_index__max)];

//ファイル名が正しいかチェック
static bool is_correct_file_name(const char *file_name, size_t chars_num)
{
	size_t length = strnlen_s(file_name, chars_num);
	//文字数が少なすぎる
	if (length < 4)
	{
		return false;
	}
	//この行に書かれているのはコメントである
	if (file_name[0] == '/' && file_name[1] == '/')
	{
		return false;
	}
	//拡張子が".wav"じゃない
	if (file_name[length - 4] != '.' ||
		file_name[length - 3] != 'w' ||
		file_name[length - 2] != 'a' ||
		file_name[length - 1] != 'v')
	{
		return false;
	}
	return true;
}

//楽曲ファイル名をファイルから決定する
static void filename_difintion(const char *file_name)
{
	//ファイルを開く
	FILE *f = nullptr;
	fopen_s(&f, file_name, "rb");
	if (f != nullptr)
	{
		//最初の一個はnull
		int i = 0;
		g_load_database[i].file_name = nullptr;
		i++;
		while (!feof(f))
		{
			//とりあえず領域確保
			if (g_load_database[i].file_name == nullptr)
			{
				g_load_database[i].file_name = (char*)malloc(sizeof(char) * FILENAME_MAX);
				assert(g_load_database[i].file_name != nullptr);
			}
			//ファイルから読む
			fgets(g_load_database[i].file_name, FILENAME_MAX, f);
			change_target_to_null(g_load_database[i].file_name, FILENAME_MAX, "\r\n");
			{
				char *found = strchr(g_load_database[i].file_name, (int)(','));
				if (found != nullptr)
				{
					int flag = 0;
					sscanf_s(found, ",%d", &flag);
					if (flag)
					{
						g_load_database[i].flag = DSLF_FLG_STREAM;
					}
					else
					{
						g_load_database[i].flag = DSLF_FLG_STATIC;
					}
				}
			}
			change_target_to_null(g_load_database[i].file_name, FILENAME_MAX, " ,");

			//読み込んだものをチェック
			//正しいファイル名でなければ、読み込み直し
			if (is_correct_file_name(g_load_database[i].file_name, FILENAME_MAX) == false)
			{
				continue;
			}
			//読み込んだものは正しいファイル名であるため、そのまま次へ進む
			i++;
			if (i >= (int)(sound_index__max))
			{
				break;
			}
		}
		fclose(f);
	}
}

//全楽曲を読み込む
static void load_all()
{
	static bool yet = true;
	if (yet)
	{
		yet = false;
		filename_difintion(wav_file_path());
		g_handles[(int)(sound_index__none)] = -1;
		for (int i = (int)(sound_index__none) + 1; i < (int)(sound_index__max); i++)
		{
			g_handles[i] = DSoundLoadFile((LPSTR)g_load_database[i].file_name, -1, g_load_database[i].flag);
		}
	}
}

//解放
static void release()
{
	for (int i = 0; i < (int)(sound_index__max); i++)
	{
		safe_free(g_load_database[i].file_name);
	}
}

//ストリーム再生する、ただし、すでに再生されていれば、何もしない
static void play_stream(sound_index_t index)
{
	//最後にストリーム再生されたものを覚えておく
	const auto &last_play = g_playing_stream_music;
	if (last_play != index)
	{
		DSoundStop(g_handles[(int)(last_play)]);
		DSoundPlay(g_handles[(int)(index)], true);
	}
	g_playing_stream_music = index;
}

















































//初期化
extern int sound_manager_init(HWND window_handle)
{
	if (DSoundInit(window_handle, nullptr) == FALSE)
	{
		return -1;
	}
	load_all();
	return 0;
}
//終了
extern void sound_manager_end()
{
	release();
	DSoundRelease();
}

//楽曲を再生
extern void sound_play(sound_index_t index)
{
#ifdef SOUND_OFF
	//音を鳴らさない
	return;
#endif
	int _index = (int)(index);
	switch (g_load_database[_index].flag)
	{
	case DSLF_FLG_DEFAULT:
	case DSLF_FLG_STATIC:
		//普通の再生(多分)
		DSoundIsStop(g_handles[_index]);
		DSoundStop(g_handles[_index]);
		DSoundPlay(g_handles[_index], false);
		break;
	case DSLF_FLG_STREAM:
		//ストリーム再生
		play_stream(index);
		break;
	default:
		break;
	}
}




