#pragma once
//タイトル画面をコントロールする場所です
//作成者　森　仁志
//更新履歴
//2019/01/22...ファイルを作成
//同日...初期化、終了、更新、描画関数を作成



#include "..\..\magnet_hero_state.h"

extern void title_init();
extern void title_end();
extern void title_update(magnet_hero_state_t *next_state);
extern void title_draw();



