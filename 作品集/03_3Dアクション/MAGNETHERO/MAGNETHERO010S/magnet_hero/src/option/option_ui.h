#pragma once
//ゲーム内のオプションのUIを管理するところです
//オプションは
//・エイムアシストON/OFF切り替え
//・マウス感度の変更
//以上の二つです
//これはゲーム中いつでも切り替えられるように作られます
//それに対応したUIにすること
//作成者：森　仁志

//初期化
extern void option_ui_init();

//終了
extern void option_ui_end();

//更新
extern void option_ui_update();

//描画
extern void option_ui_draw();


//エイムアシストのON/OFFが切り換えられたら呼ぶ
extern void option_ui_aim_assist(bool on_off);


//マウス感度が変更されたら呼ぶ
extern void option_ui_mouse_sensitivity(float t);

