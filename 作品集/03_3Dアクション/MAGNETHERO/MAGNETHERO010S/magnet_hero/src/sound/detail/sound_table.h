#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	
#include "sound_index.h"


enum stream_play_t
{
	sp_on,
	sp_off,
};

enum loop_play_t
{
	lp_on,
	lp_off,
};

struct sound_table_data_t
{
	char *_file_name;					//ファイル名
	stream_play_t _stream_play;			//ストリーム再生するか
	loop_play_t _loop_play;				//ループ再生するか
};

//テーブルを作る
extern void sound_table_create();

//テーブルを破棄する
extern void sound_table_destroy();

//インデックスを用いてテーブルからデータを得る
extern const sound_table_data_t *sound_table_get(sound_index_t index);

