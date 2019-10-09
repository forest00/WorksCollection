//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/13
//	

//親ヘッダー
#include "magnet_hero.h"
//その他のヘッダー
#include "magnet_hero_state.h"
#include "..\mylib\mylib.h"
#include "..\texture_manager\texture_manager.h"
#include "..\fbx_table\fbx_table.h"
#include "..\common_count\common_count_advance.h"
#include "..\draw_setup\light.h"
#include "..\draw_setup\view_and_projection_transformation.h"
#include "..\file_data\file_data.h"
#include "..\framework\magnet_hero_state\game_main\stage_event\stage_index_saver.h"
#include "..\framework\magnet_hero_state\game_main\stage_event\stage_data.h"
#include "..\window\icon_file_name.h"
#include "..\window\startup_option.h"


//グローバル変数
struct global_t
{
	magnet_hero_state_t _now_state;
};
static global_t g;


//ゲームの初期化
extern int magnet_hero_init()
{
	//テキストファイルの読み込み
	file_data_load_all();
	//ライブラリの初期化
	if (mylib_init(
		hInstance,
		startup_option_get_screen_mode(),
		startup_option_get_left(),
		startup_option_get_top(),
		startup_option_get_width(),
		startup_option_get_height(),
		startup_option_get_window_title(),
		icon_file_name_get()
	) != 0)
	{
		MessageBox(nullptr, "初期化に失敗しました", "エラー", MB_OK);
		mylib_end();
		return E_FAIL;
	}
	//マウスを隠す
	if (startup_option_get_hide_mouse())
	{
		mylib_hide_mouse_cursor();
	}
	//背景青消去
	mylib_clear(0, 100, 200);
	//背景色のみを見せる
	mylib_present();
	//テクスチャの読み込み
	texture_manager_init();
	//FBXファイルの読み込み
	fbx_table_load();
	//ステートをnoneにする
	g._now_state = mhs_none;
	//ファイルから最初のステートを読み込み
	const datum_t *data = file_data_get(fdi_init_state, nullptr);
	//最初のステートをセット
	magnet_hero_state_set_first_state((magnet_hero_state_t)(data[0]._Int));
	//ステージ情報初期化
	stage_data_init();
	//ステージクリア情報初期化
	stage_clear_data_init();
	return 0;
}

//ゲームの終了
extern void magnet_hero_end()
{
	//FBX破棄
	fbx_table_destroy();
}

//ゲームの更新
extern void magnet_hero_update()
{
	//入力の更新
	mylib_input_update();
	//ゲーム部分の更新
	magnet_hero_state_update(mylib_get_d3d_device(), g._now_state, &g._now_state);
	//環境光の設定
	light_set_ambient(mylib_get_d3d_device(), D3DCOLOR_XRGB(100, 100, 100));
	//ディレクショナル光の設定
	light_set_directional(mylib_get_d3d_device());
	//ビュー変換&プロジェクション変換
	view_and_projection_transformation(mylib_get_d3d_device());
	//更新回数を記録
	common_count_advance();
}

//ゲームの描画
extern void magnet_hero_draw()
{
	//レンダーターゲットの切り替え
	mylib_change_render_target(1);
	//背景青消去
	mylib_clear(0, 100, 200);
	//シーン開始
	if (mylib_begin_scene() == 0)
	{
		//※ここに描画処理を書く
		magnet_hero_state_draw(mylib_get_d3d_device(), g._now_state);
		//シーン終了
		mylib_end_scene();
	}
	//レンダーターゲットを元に戻す
	mylib_change_render_target(0);
	//背景青消去
	mylib_clear(0, 100, 200);
	//シーン開始
	if (mylib_begin_scene() == 0)
	{
		//本描画
		mylib_draw_board(mylib_get_d3d_device(),
			&D3DXVECTOR3(-1.0f, 1.0f, 0.0f), &D3DXVECTOR2(0.0f, 0.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR3(1.0f, 1.0f, 0.0f), &D3DXVECTOR2(1.0f, 0.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR3(-1.0f, -1.0f, 0.0f), &D3DXVECTOR2(0.0f, 1.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR3(1.0f, -1.0f, 0.0f), &D3DXVECTOR2(1.0f, 1.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			mylib_get_rendered_texture(), mbm_none);
		//シーン終了
		mylib_end_scene();
	}
	//バックバッファと切り替え
	mylib_present();
}