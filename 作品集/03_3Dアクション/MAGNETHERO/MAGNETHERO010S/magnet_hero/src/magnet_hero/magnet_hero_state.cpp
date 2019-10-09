//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

//親ヘッダー
#include "magnet_hero_state.h"
//その他のヘッダー
#include "magnet_hero_state\title\title.h"
#include "magnet_hero_state\game_main\game_main.h"
#include "magnet_hero_state\ending\ending.h"


static magnet_hero_state_t g_first_state = mhs_none;


//状態ごとに初期化する関数を呼び出す
static void magnet_hero_state_init(magnet_hero_state_t state)
{
	switch (state)
	{
	case mhs_none:
		break;
	case mhs_title:
		title_init();
		break;
	case mhs_game_main:
		game_main_init();
		break;
	case mhs_ending:
		ending_init();
		break;
	case mhs_max:
		break;
	default:
		break;
	}
}


//状態ごとに終了する関数を呼び出す
static void magnet_hero_state_end(magnet_hero_state_t state)
{
	switch (state)
	{
	case mhs_none:
		break;
	case mhs_title:
		title_end();
		break;
	case mhs_game_main:
		game_main_end();
		break;
	case mhs_ending:
		ending_end();
		break;
	case mhs_max:
		break;
	default:
		break;
	}
}


//一番最初の状態を設定
extern void magnet_hero_state_set_first_state(magnet_hero_state_t state)
{
	g_first_state = state;
}



//状態ごとに更新する関数を呼び出す
extern void magnet_hero_state_update(magnet_hero_state_t now_state, magnet_hero_state_t *next_state)
{
	magnet_hero_state_t prev_state = now_state;
	switch (now_state)
	{
	case mhs_none:
		now_state = g_first_state;
		break;
	case mhs_title:
		title_update(&now_state);
		break;
	case mhs_game_main:
		game_main_update(&now_state);
		break;
	case mhs_ending:
		ending_update(&now_state);
		break;
	case mhs_max:
		break;
	default:
		break;
	}
	if (now_state != prev_state)
	{///状態が切り替わった
		//前の状態を終了させる
		magnet_hero_state_end(prev_state);
		//現在の状態を初期化
		magnet_hero_state_init(now_state);
		if (next_state)
		{
			*next_state = now_state;
		}
	}
}


//状態ごとに描画関数を呼び出す
extern void magnet_hero_state_draw(magnet_hero_state_t state)
{
	switch (state)
	{
	case mhs_none:
		break;
	case mhs_title:
		title_draw();
		break;
	case mhs_game_main:
		game_main_draw();
		break;
	case mhs_ending:
		ending_draw();
		break;
	case mhs_max:
		break;
	default:
		break;
	}
}
