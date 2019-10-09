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
#include "..\option\option.h"
#include "..\texture_manager\texture_manager.h"
#include "..\fbx_table\fbx_table.h"
#include "..\file_data\file_data.h"
#include "..\magnet_hero\magnet_hero_state\game_main\stage_event\stage_index_saver.h"
#include "..\magnet_hero\magnet_hero_state\game_main\stage_event\stage_data.h"


//グローバル変数
struct global_t
{
	magnet_hero_state_t _now_state;
};
static global_t g;


//ゲームの初期化
extern int magnet_hero_init()
{
	////マルチレンダリングできるかチェック
	//{
	//	D3DCAPS9 caps;
	//	mylib_get_d3d_device()->GetDeviceCaps(&caps);
	//	DWORD RT = caps.NumSimultaneousRTs;
	//	if (RT <= 1)
	//	{
	//		MessageBox(nullptr, "マルチレンダリングターゲットがサポートされていません", "サポート外エラー", MB_OK);
	//		return -1;
	//	}
	//}
	//背景青消去
	mylib_clear(0, 100, 200);
	//背景色のみを見せる
	mylib_present();
	//テクスチャの読み込み
	texture_manager_init();
	//FBXファイルの読み込みテーブル初期化
	fbx_table_init();
	//最初にプレーヤーだけ読み込む
	fbx_table_load(fri_player);
	//ステートをnoneにする
	g._now_state = mhs_none;
	//ファイルから最初のステートを読み込み
	const datum_t *data = file_data_get(fdi_init_state, nullptr);
	//最初のステートをセット
	magnet_hero_state_set_first_state((magnet_hero_state_t)((data++)->_Int));
	//ステージ情報初期化
	stage_data_init();
	//ステージクリア情報初期化
	stage_clear_data_init();
	//オプションの初期化
	option_init();
	return 0;
}

//ゲームの終了
extern void magnet_hero_end()
{
	//オプションの終了
	option_end();
	//FBX破棄
	fbx_table_destroy();
	//FBX読み込みテーブルの終了
	fbx_table_end();
}

//ゲームの更新
extern void magnet_hero_update()
{
	//ゲーム部分の更新
	magnet_hero_state_update(g._now_state, &g._now_state);
	//オプションの更新
	option_update();
}

//ゲームの描画
extern void magnet_hero_draw()
{
	//レンダーターゲットの切り替え
	mylib_set_render_target_manage(1);
	//背景青消去
	mylib_clear(0, 100, 200);
	//シーン開始
	if (mylib_begin_scene() == 0)
	{
		///※ここに描画処理を書く
		//ゲームの描画
		magnet_hero_state_draw(g._now_state);
		//オプションの描画
		option_draw();
		//シーン終了
		mylib_end_scene();
	}
	//レンダーターゲットを元に戻す
	mylib_set_render_target_manage(0);
	//背景青消去
	mylib_clear(0, 100, 200);
	//シーン開始
	if (mylib_begin_scene() == 0)
	{
		//本描画
		D3DXVECTOR3 position[4];
		D3DXVECTOR2 uv[4];
		position[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		position[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		position[2] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		position[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		uv[0] = D3DXVECTOR2(0.0f, 0.0f);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f);
		mylib_draw_board_2d_wave(nullptr,
			&position[0], &uv[0], D3DCOLOR_ARGB(255, 255, 255, 255),
			&position[1], &uv[1], D3DCOLOR_ARGB(255, 255, 255, 255),
			&position[2], &uv[2], D3DCOLOR_ARGB(255, 255, 255, 255),
			&position[3], &uv[3], D3DCOLOR_ARGB(255, 255, 255, 255),
			mylib_get_rendered_texture()
		);
		//デバッグ用文字描画
		mylib_output_debug_string();
		//シーン終了
		mylib_end_scene();
	}
	//バックバッファと切り替え
	mylib_present();
}

extern bool magnet_hero_ended()
{
	return g._now_state == mhs_ended;
}