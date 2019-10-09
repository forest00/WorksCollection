//親ヘッダー
#include "sprite_integration.h"
//その他のヘッダー
#include "aiming_cursor.h"
#include "bibibi.h"
#include "qte.h"

//初期化
extern void sprite_integration_init()
{
	aiming_cursor_init();
	bibibi_init();
	qte_init();
}

//終了
extern void sprite_integration_end()
{
	aiming_cursor_end();
	bibibi_end();
	qte_end();
}

//更新
extern void sprite_integration_update()
{
	aiming_cursor_update();
	bibibi_update();
	qte_update();
}

//描画
extern void sprite_integration_draw()
{
	//照準カーソル描画
	aiming_cursor_draw();
	//びりびりeffect描画
	bibibi_draw();
	//QTEガイド描画
	qte_draw();
}

//びりびり
extern void sprite_integration_bibibi_fire(int base, int random_range)
{
	bibibi_fire(base, random_range);
}

//QTE開始
extern void sprite_integration_qte_start()
{
	qte_start();
}

//QTE成功判定
extern bool sprite_integration_qte_judge()
{
	return qte_judge();
}

//QTEタイムアウトチェック
extern bool sprite_integration_qte_timeout()
{
	return qte_timeout();
}
