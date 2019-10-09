#pragma once
//ゲーム起動時のウィンドウサイズや位置などのオプションです
//テキストファイルから読み込みます
//作成者　森　仁志
//更新履歴
//2019/01/22...ファイル作成
//同日...関数を追加

//起動時にスクリーンモードをどうするかの指定です
//0ならダイアログを出して確認しなくてはいけません
//1ならフルスクリーンモードで起動しなくてはいけません
//2ならウィンドウモードで起動しなくてはいけません
//それ以外の時は起動してはいけません
extern int startup_option_get_screen_mode();

//ウィンドウの左初期位置です
extern int startup_option_get_left();

//ウィンドウの上初期位置です
extern int startup_option_get_top();

//ウィンドウの初期横幅です
extern int startup_option_get_width();

//ウィンドウの初期縦幅です
extern int startup_option_get_height();

//マウスを隠すかどうかを指定するオプションです
//trueならマウスを隠さなくてはいけません
extern bool startup_option_get_hide_mouse();

//ウィンドウタイトル名です
extern const char *startup_option_get_window_title();
