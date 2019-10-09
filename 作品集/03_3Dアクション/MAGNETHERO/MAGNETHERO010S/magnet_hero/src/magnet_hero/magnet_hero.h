#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	ゲームのメイン処理が記述された部分です
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	
//	最終更新		2018/12/07
//	


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


//ゲームの初期化
extern int magnet_hero_init();

//ゲームの終了
extern void magnet_hero_end();

//ゲームの更新
extern void magnet_hero_update();

//ゲームの描画
extern void magnet_hero_draw();

//ゲームを終了させるべき
extern bool magnet_hero_ended();

