

//楽曲を読み込むためのデータ
struct sound_table_data_t
{
	const char *_file_name;		//ファイル名
	int _cahnnel;				//読み込むチャンネル
	bool _stream;				//ストリーム再生する
	bool _repeat;				//繰り返し再生する
};

