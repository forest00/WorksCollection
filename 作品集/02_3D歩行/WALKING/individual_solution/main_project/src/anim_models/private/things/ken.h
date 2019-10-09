#pragma once

//kenを出すファイル
//kenは、アニメーションするオブジェクトである



#include <d3dx9.h>



//ken初期化 & 出現
extern void ken_init();

//ken終了
extern void ken_end();

//ken更新
extern void ken_update();

//ken描画
extern void ken_draw(IDirect3DDevice9 *device);




