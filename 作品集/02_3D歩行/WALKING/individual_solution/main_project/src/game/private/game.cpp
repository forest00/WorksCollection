
//自分の親ヘッダー
#include "..\public\game.h"
//その他のヘッダー
#include "..\..\math_with_defines\math_with_defines.h"
#include "game_system.h"
#include "game_base.h"
#include "..\..\loop\public\loop_timer.h"








/*---------------------------------------------------------------------------------------------*/
extern int game_init()
{
	//システム部分の初期化
	if (game_system_init() != 0)
	{
		return -1;
	}

	//ゲーム部分の初期化
	game_base_init();

	//時間の計測用
	loop_timer_init();

	return 0;
}



extern bool game_shoud_continue()
{
	//メッセージ処理をする
	return process_message();
}


extern void game_update()
{
	//システム部分の更新
	game_system_update();

	//ゲーム部分の更新
	game_base_update();

	//時間の計測用
	loop_timer_record();
	
}



extern void game_draw()
{
	//game_system_draw_end();
	

	//描画の準備(失敗したら何もせず終了)
	if (game_system_draw_begin() != 0)
	{
		return;
	}

	//ゲーム部分の描画
	game_base_draw();

	//描画終わり
	game_system_draw_end();

}


extern void game_end()
{

	//ゲーム部分の終了
	game_base_end();


	//システム部分の終了
	game_system_end();

}
/*---------------------------------------------------------------------------------------------*/


