#pragma once
//びりびりeffectを出します
//スプライトの描画機能が使用できる状態になっていることが使用の条件です
//作成者　森　仁志

//DirectX
#include <d3dx9.h>

//初期化
extern void bibibi_init();

//終了
extern void bibibi_end();

//更新
extern void bibibi_update();

//描画
extern void bibibi_draw();

//メッセージ発行
extern void bibibi_fire(int base, int random_range);

