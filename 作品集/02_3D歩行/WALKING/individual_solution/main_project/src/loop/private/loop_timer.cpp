
//親ヘッダー
#include "..\public\loop_timer.h"
//その他のヘッダー
#include <Windows.h>

static DWORD g_loop_timer_last;

static double g_loop_timer_time;



//初期化
//ループが始まる前に一回だけ呼ぶ
extern void loop_timer_init()
{
	g_loop_timer_last = timeGetTime();
	g_loop_timer_time = 0.0;
}

//記録
//ループ中に毎回呼ぶ
extern void loop_timer_record()
{
	DWORD now = timeGetTime();
	g_loop_timer_time = (double)(now - g_loop_timer_last) / 1000.0f;
	g_loop_timer_last = now;
}


//ループにかかった時間を得る
extern double loop_timer_get()
{
	return g_loop_timer_time;
}




