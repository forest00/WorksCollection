
#include "test_enemy\test_enemy_manager.h"

//敵キャラに関する操作はすべてここにある関数の中で行う

//初期化
extern void enemy_init();

//更新
extern void enemy_update();

//描画
extern void enemy_draw(IDirect3DDevice9 *device);

//終了
extern void enemy_end();

extern test_enemy_manager_t *enemy_get_test_enemy_manager();
