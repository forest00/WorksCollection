#pragma once
//
//ステージのUI
//
//作成者　森仁志
//履歴
//2019/02/05...ファイルを作成
//2019/02/05...初期化、描画、終了関数を追加

//DirectX
#include <d3dx9.h>

//初期化
extern void stage_ui_init();

//終了
extern void stage_ui_end();

//更新
extern void stage_ui_update();

//描画
extern void stage_ui_draw(time_t goal_frame, time_t goal_limit_frame);

