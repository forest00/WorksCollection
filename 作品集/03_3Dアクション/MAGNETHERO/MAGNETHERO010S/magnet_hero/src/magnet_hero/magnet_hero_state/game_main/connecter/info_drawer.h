#pragma once
//プレーヤーの影などを描画するところです
//作成者　森　仁志
//履歴
//2019/02/04...ファイルを作成
//2019/02/04...プレーヤーの影を描画する関数を追加

//DirectXのヘッダー
#include <d3dx9.h>

//プレーヤーの影を描画する
extern void drawer_player_shadow_draw();

//プレーヤーの飛ばした手を描画する
extern void drawer_player_magnet_hand();
