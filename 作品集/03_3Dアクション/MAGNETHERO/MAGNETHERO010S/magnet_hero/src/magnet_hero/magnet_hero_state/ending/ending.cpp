//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	

//親ヘッダー
#include "ending.h"
//その他のヘッダー
#include "..\..\..\mylib\mylib.h"
#include "..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\input\input.h"
#include "..\..\..\movie\avi.h"

//再生する動画ファイル
#define _movie_filename			(L"res/movie/ed.avi")

//グローバル変数
struct global_t
{
	bool _movie_played;
};
static global_t g;

extern void ending_init()
{
	g._movie_played = false;
}
extern void ending_end()
{

}
extern void ending_update(magnet_hero_state_t *next_state)
{
	//magnet_hero_state_t now_state = mhs_ending;
	///入力の取得
	//const virtual_input_t *trigger = mylib_get_virtual_input_trigger();
	//ムービー再生はまだ行っていないか
	if (g._movie_played == false)
	{
		//とりあえずムービー再生
		play_avi(_movie_filename);
		//再生が終わったのでフラグ切り替え
		g._movie_played = true;
	}
	else
	{
		//if (trigger->_a)
		{
			*next_state = mhs_title;
		}
	}
}
extern void ending_draw()
{
	/*
	//頂点を用意
	static D3DXVECTOR3 position[4];
	position[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	position[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	position[2] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	position[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	//UV座標を用意
	static D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//描画
	mylib_draw_board_2d(
		&position[0], &uv[0], D3DCOLOR_ARGB(255, 255, 255, 255),
		&position[1], &uv[1], D3DCOLOR_ARGB(255, 255, 255, 255),
		&position[2], &uv[2], D3DCOLOR_ARGB(255, 255, 255, 255),
		&position[3], &uv[3], D3DCOLOR_ARGB(255, 255, 255, 255),
		texture_manager_get(ti_person0_komiya), mbm_none
	);
	*/
}
