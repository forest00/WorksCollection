
//親ヘッダー
#include "..\public\flatpo_manager.h"
//その他のヘッダー
#include "..\public\flatpo_texture_loader.h"
#include "flatpo_drawer.h"
#include "flatpo_test.h"

//板ポリの初期化
extern void flatpo_manager_init(IDirect3DDevice9 *device)
{
	flatpo_texture_loader_load_all(device);
	flatpo_tester_all_init();
}

//板ポリの終了
extern void flatpo_manager_end()
{
	flatpo_tester_all_end();
	flatpo_texture_loader_destroy_all();
}

//板ポリの更新
extern void flatpo_manager_update()
{
	flatpo_tester_all_update();
}

//板ポリの描画
extern void flatpo_manager_draw(IDirect3DDevice9 *device, double aspect)
{
	//板ポリ描画開始
	flatpo_begin_scene(device, aspect);
	//テスト用の板ポリを描画
	flatpo_tester_all_draw(device);
	//板ポリ描画終了
	flatpo_end_scene();
}

