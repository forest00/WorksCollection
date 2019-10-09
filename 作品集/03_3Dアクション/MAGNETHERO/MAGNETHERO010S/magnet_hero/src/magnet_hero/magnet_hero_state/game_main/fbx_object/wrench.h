#pragma once
//プレーヤーがビ~~ムを発射している間に描画されるアームを管理するファイルです
//作成者　森　仁志


#include <d3dx9.h>

//指定した位置に指定した角度でアームを描画する
#if 0
extern void wrench_draw(const D3DXVECTOR3 *position, float scale, time_t frame, float theta, float phi);
#endif

//アームの描画はビルボード描画に変更されました、それに伴い角度を受け取らないようになりました
extern void ball_draw(const D3DXVECTOR3 *position, float scale);

