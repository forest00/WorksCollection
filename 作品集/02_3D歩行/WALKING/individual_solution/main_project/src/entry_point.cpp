


#include <Windows.h>
#include "game\public\game.h"

//エントリーポイント
int WINAPI WinMain(HINSTANCE instance_handle, HINSTANCE, LPSTR, int)
{

	//ゲーム全体の初期化
	//失敗したら何もせず関数を抜ける
	if (game_init() != 0)
	{
		MessageBox(0, "初期化失敗です", "", MB_OK);
		return -1;
	}


	//ゲームループ
	while (game_shoud_continue())
	{
		game_update();
		game_draw();
	}

	//ゲーム全体の終了
	game_end();

	return 0;
}




