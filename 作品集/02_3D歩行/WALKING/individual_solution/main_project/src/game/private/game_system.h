#pragma once


//ゲームのシステム部分の初期化や更新などをする関数群です



//システム部分の初期化
extern int game_system_init();

//メッセージ処理
extern bool process_message();

//システム部分の更新
extern void game_system_update();

//描画の開始
extern int game_system_draw_begin();

//描画の終了(この関数は、対になるbegin関数が成功している状態じゃないと、呼んじゃダメ)
extern void game_system_draw_end();

//システム部分の終了
extern void game_system_end();

