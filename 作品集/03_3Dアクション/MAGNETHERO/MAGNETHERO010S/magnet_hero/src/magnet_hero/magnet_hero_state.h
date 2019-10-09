#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	ゲームの状態を管理するところです
//	
//	ここでは、状態ごとに異なる関数を呼び出す作業をします
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	



#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


//それぞれの状態
enum magnet_hero_state_t
{
	mhs_none,				//ステートなし
	mhs_title,				//タイトル
	mhs_game_main,			//メインゲーム部分
	mhs_ending,				//エンディング
	mhs_ended,				//終了させるべき
	mhs_max,				//ステートの最大数
};

/*
*@fn magnet_hero_state_set_first_state
*@brief 一番最初の状態を設定
*@param state 最初の状態として設定される値
*/
extern void magnet_hero_state_set_first_state(magnet_hero_state_t state);


/*
*@fnmagnet_hero_state_update
*@brief 状態ごとに更新する関数を呼び出す
*@param now_state 現在の状態
*@param next_state 次の状態を格納する場所
*@detail now_stateがmagnet_hero_state_t_noneの場合、magnet_hero_state_set_first_stateで設定された状態にする
*/
extern void magnet_hero_state_update(magnet_hero_state_t now_state, magnet_hero_state_t *next_state);

/*
*@fnmagnet_hero_state_update
*@brief 状態ごとに描画関数を呼び出す
*@param state 状態
*/
extern void magnet_hero_state_draw(magnet_hero_state_t state);


