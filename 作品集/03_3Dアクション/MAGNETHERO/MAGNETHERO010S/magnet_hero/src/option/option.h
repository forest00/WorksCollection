#pragma once
//ゲーム内のオプションを管理するところです
//オプションは
//・エイムアシストON/OFF切り替え
//・マウス感度の変更
//以上の二つです
//これはゲーム中いつでも切り替えられるように作らなくてはいけません
//作成者：森　仁志



//ゲーム開始時に呼ぶ
extern void option_init();

//ゲーム終了時に呼ぶ
extern void option_end();

//更新：毎フレーム呼ぶ
extern void option_update();

//描画：毎フレーム呼ぶ
extern void option_draw();

//エイムアシストをする
extern bool option_get_do_aim_assist();

//マウス感度(0~1)
extern float option_get_mouse_sensitivity();

