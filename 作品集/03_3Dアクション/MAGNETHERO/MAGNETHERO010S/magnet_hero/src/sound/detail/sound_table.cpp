//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	

//親ヘッダー
#include "sound_table.h"
//その他のヘッダー
#include <Windows.h>
#include "..\..\char_ptr\char_ptr.h"
#include "..\..\file_data\file_data.h"

static sound_table_data_t g_table[(size_t)(si_max)];

//テーブルを作る
extern void sound_table_create()
{
	size_t datacount = 0;
	const datum_t *data = file_data_get(fdi_sound_file_database, nullptr);
	for (size_t i = 1; i < (size_t)(si_max); i++)
	{
		clone_string(data[datacount]._String, data[datacount]._StringSize, &g_table[i]._file_name, nullptr);
		g_table[i]._stream_play = (stream_play_t)(data[datacount + 1]._Int == 0);
		g_table[i]._loop_play = (loop_play_t)(data[datacount + 2]._Int == 0);
		datacount += 3;
	}
}

//テーブルを破棄する
extern void sound_table_destroy()
{
	for (size_t i = 1; i < (size_t)(si_max); i++)
	{
		destroy_string(&g_table[i]._file_name);
	}
}

//インデックスを用いてテーブルからデータを得る
extern const sound_table_data_t *sound_table_get(sound_index_t index)
{
	return &g_table[(size_t)(index)];
}

