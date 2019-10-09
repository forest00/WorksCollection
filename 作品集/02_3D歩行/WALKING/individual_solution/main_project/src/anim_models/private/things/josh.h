#pragma once

//joshを出すファイル
//joshは、アニメーションするオブジェクトである



#include <d3dx9.h>



//josh初期化 & 出現
extern void josh_init();

//josh終了
extern void josh_end();

//josh更新
extern void josh_update();

//josh描画
extern void josh_draw(IDirect3DDevice9 *device);




