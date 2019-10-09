//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

//親ヘッダー
#include "sound.h"
//その他のヘッダー
#include "detail\sound_table.h"
#include "..\mylib\mylib.h"
#include "..\amlib\dslib.h"

//デバッグ用ヘッダー
#include <assert.h>


static int g_channel[(size_t)(si_max)] = {};
static sound_index_t g_stream_index = si_none;


static int sound_load(const char *file_name, DWORD flag)
{
	//ファイルの存在チェック
	if (mylib_file_is_exist(file_name) == false)
	{
		//ファイルは存在しない
		return -1;
	}
	//ファイルは存在するので読み込む
	return DSoundLoadFile((LPSTR)(file_name), -1, flag);
}


//初期化
extern void sound_init(HWND window_handle)
{
	//dslibの初期化
	DSoundInit(window_handle, nullptr);
	//テーブルを用意
	sound_table_create();
	//テーブルの情報を元に楽曲を読み込む
	g_channel[0] = -1;
	for (size_t i = 1; i < (size_t)(si_max); i++)
	{
		const sound_table_data_t *data = sound_table_get((sound_index_t)(i));
		DWORD flag = DSLF_FLG_DEFAULT;
		if (data->_stream_play == sp_on)
		{
			flag = DSLF_FLG_STREAM;
		}
		else
		{
			flag = DSLF_FLG_STATIC;
		}
		g_channel[i] = sound_load(data->_file_name, flag);
	}
}

//終了
extern void sound_end()
{
	//dslibの解放
	DSoundRelease();
	//テーブルを破棄
	sound_table_destroy();
}

//楽曲を再生
extern void sound_play(sound_index_t index)
{
	//基本情報取得
	const sound_table_data_t *data = sound_table_get(index);
	int channel = g_channel[(size_t)(index)];
	if (data->_stream_play == sp_on)
	{
		g_stream_index = index;
	}
	//すでに再生されているかチェック
	if (DSoundIsStop(channel) == false)
	{
		//止める
		DSoundStop(channel);
	}
	//楽曲再生
	DSoundPlay(channel, data->_loop_play == lp_on);
}

//楽曲が再生中かチェック
extern bool sound_is_play(sound_index_t index)
{
	//チャンネル取得
	int channel = g_channel[(size_t)(index)];
	//楽曲再生チェック
	return DSoundIsStop(channel) == false;
}

//ストリーム再生中の楽曲を取得
extern sound_index_t sound_get_playing_stream()
{
	return g_stream_index;
}

//楽曲を停止
extern void sound_stop(sound_index_t index)
{
	//チャンネル取得
	int channel = g_channel[(size_t)(index)];
	//楽曲停止
	DSoundStop(channel);
}

