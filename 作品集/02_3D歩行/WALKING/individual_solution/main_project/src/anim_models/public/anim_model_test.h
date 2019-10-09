//	
//	アニメーション付きモデル(以下モデル)の動作テスト用のファイルです
//	
//	ここにある関数はすべて"anim_model_"キーワードから始まります
//	
//	作成者			森	仁志
//	作成日			2018/12/03
//	最終更新日		2018/12/03
//	

#pragma once

#include <d3dx9.h>

//Xファイルを読み込み、全てのモデルを初期化し、ためしにいくつか出現させる
extern void anim_model_tester_all_init(IDirect3DDevice9 *device);

//全てのモデルを破棄する
extern void anim_model_tester_all_end();

//全てのモデルを更新する
extern void anim_model_tester_all_update();

//全てのモデルを描画する
extern void anim_model_tester_all_draw(IDirect3DDevice9 *device);



//プレイヤーとして使うモデルの位置を得る
void anim_model_get_the_position_of_the_model_to_use_as_a_player(D3DXVECTOR3 *out);
