#pragma once

//アニメーションするロボットを出すファイル



#include <d3dx9.h>



//ロボット初期化 & 出現
extern void robot_init();

//ロボット終了
extern void robot_end();

//ロボット更新
extern void robot_update();

//ロボット描画
extern void robot_draw(IDirect3DDevice9 *device);




