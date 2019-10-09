#pragma once



//初期化
//ループが始まる前に一回だけ呼ぶ
extern void loop_timer_init();

//記録
//ループ中に毎回呼ぶ
extern void loop_timer_record();

//ループにかかった時間を得る
extern double loop_timer_get();



