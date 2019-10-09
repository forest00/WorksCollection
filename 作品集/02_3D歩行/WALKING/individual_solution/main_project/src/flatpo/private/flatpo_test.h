//	
//	板ポリをテストするためのファイルです
//	
//	作成者			森	仁志
//	作成日			2018/12/03
//	最終更新日		2018/12/03
//	

#pragma once
#include <d3dx9.h>


//板ポリテスト用オブジェクトを初期化
extern void flatpo_tester_all_init();

//板ポリテスト用オブジェクトを破棄
extern void flatpo_tester_all_end();

//板ポリテスト用オブジェクトを更新
extern void flatpo_tester_all_update();

//板ポリテスト用オブジェクトを描画
extern void flatpo_tester_all_draw(IDirect3DDevice9 *device);

