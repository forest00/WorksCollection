#pragma once

//usagiを出すファイル
//usagiは、アニメーションするオブジェクトである



#include <d3dx9.h>



//usagi初期化 & 出現
extern void usagi_init();

//usagi終了
extern void usagi_end();

//usagi更新
extern void usagi_update();

//usagi描画
extern void usagi_draw(IDirect3DDevice9 *device);




