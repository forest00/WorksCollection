#pragma once


//ゲームのメイン部分の処理をする関数群です
//これらの関数を呼び出す前に、"game_system" から始まる適切な関数を呼び出す必要があります


//ゲーム部分の初期化
extern void game_base_init();

//ゲーム部分の更新
extern void game_base_update();

//ゲーム部分の描画
extern void game_base_draw();

//ゲーム部分の終了
extern void game_base_end();



