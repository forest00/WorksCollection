#pragma once
//照準カーソルを出します
//スプライトの描画機能が使用できる状態になっていることが使用の条件です
//作成者　森　仁志
//履歴
//2019/01/31...ファイルを作成
//同日...照準カーソル初期化、更新、描画、終了関数を記述

//DirectX
#include <d3dx9.h>

//初期化
extern void aiming_cursor_init();

//終了
extern void aiming_cursor_end();

//更新
extern void aiming_cursor_update();

//描画
extern void aiming_cursor_draw();
