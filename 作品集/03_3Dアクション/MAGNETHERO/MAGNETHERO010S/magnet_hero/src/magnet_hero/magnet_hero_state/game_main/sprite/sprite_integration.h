#pragma once
//ゲームのメイン部分で使うすべてのスプライトを統合するファイルです
//作成者　森仁志
//履歴
//2019/01/31...ファイルを作成
//同日...初期化、更新、描画、終了関数を記述
//同日...メッセージ発行関数を記述

//DirectXのヘッダー
#include <d3dx9.h>


//初期化
extern void sprite_integration_init();

//終了
extern void sprite_integration_end();

//更新
extern void sprite_integration_update();

//描画
extern void sprite_integration_draw();

//びりびり
extern void sprite_integration_bibibi_fire(int base, int random_range);

//QTE開始
extern void sprite_integration_qte_start();

//QTE成功判定
extern bool sprite_integration_qte_judge();

//QTEタイムアウトチェック
extern bool sprite_integration_qte_timeout();
