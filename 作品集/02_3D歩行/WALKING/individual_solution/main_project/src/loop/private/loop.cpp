
//親ヘッダー
#include "..\public\loop.h"
//その他のヘッダー
#include "..\public\loop_timer.h"


//ループにかかった時間を取得する
extern double loop_time()
{
	return loop_timer_get();
}

