#pragma once

//block_manを出すファイル
//block_manは、アニメーションするオブジェクトである



#include <d3dx9.h>



//block_man初期化 & 出現
extern void block_man_init();

//block_man終了
extern void block_man_end();

//block_man更新
extern void block_man_update();

//block_man描画
extern void block_man_draw(IDirect3DDevice9 *device);




