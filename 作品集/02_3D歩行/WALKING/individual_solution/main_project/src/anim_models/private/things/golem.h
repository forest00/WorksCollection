#pragma once

//golemを出すファイル
//golemは、アニメーションするオブジェクトである



#include <d3dx9.h>


//golem構造体
struct golem_t;


//golem初期化 & 出現
extern void golem_init();

//golem終了
extern void golem_end();

//golem更新
extern void golem_update();

//golem描画
extern void golem_draw(IDirect3DDevice9 *device);


//入力に応じて動くゴーレムの位置を得る
void golem_get_position_special(D3DXVECTOR3 *out);

